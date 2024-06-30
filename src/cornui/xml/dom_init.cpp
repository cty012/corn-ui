#include <sstream>
#include <stdexcept>
#include <format>
#include <unordered_set>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/util/string_utils.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/reserved.h>
#include <cornui/xml/dom.h>

namespace cornui {
    void parseXMLFile(const std::filesystem::path& file, xmlDocPtr doc, xmlNodePtr& head) {
        head = nullptr;
        xmlNodePtr body = nullptr;

        // Root node must be <cornui>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui") != 0) {
            throw std::invalid_argument(
                    "File '" + file.string() + "' has root element with tag '<"
                    + rootTag + ">', but '<cornui>' expected.\n");
        }

        // Find <head> and <body>
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (strcmp(tag, "head") == 0) {
                    // Avoid multiple heads
                    if (head) {
                        throw std::invalid_argument(
                                "File '" + file.string() + "' has multiple <head> nodes under root.");
                    }
                    head = xmlChild;
                } else if (strcmp(tag, "body") == 0) {
                    // Avoid multiple heads
                    if (body) {
                        throw std::invalid_argument(
                                "File '" + file.string() + "' has multiple <body> nodes under root.");
                    }
                    body = xmlChild;
                } else {
                    throw std::invalid_argument(
                            "File '" + file.string() + "' has invalid node with tag <" + tag + "> under root");
                }
            }
        }

        // If either not found
        if (!head && !body) {
            throw std::invalid_argument("File '" + file.string() + "' has no <head> and <body>.");
        } else if (!head) {
            throw std::invalid_argument("File '" + file.string() + "' has no <head>.");
        } else if (!body) {
            throw std::invalid_argument("File '" + file.string() + "' has no <body>.");
        }
    }

    void parseXMLDefFile(
            const std::filesystem::path& file, xmlDocPtr doc, xmlNodePtr& head,
            std::unordered_set<std::string>& defTags) {

        head = nullptr;
        defTags.clear();

        // Root node must be <cornui-def>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui-def") != 0) {
            throw std::invalid_argument(
                    "Def file '" + file.string() + "' has root element with tag '<"
                    + rootTag + ">', but '<cornui-def>' expected.\n");
        }

        // Find <head> and definitions
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (strcmp(tag, "head") == 0) {
                    // Avoid multiple heads
                    if (head) {
                        throw std::invalid_argument(
                                "Def file '" + file.string() + "' has multiple <head> nodes under root.");
                    }
                    head = xmlChild;
                } else if (tagIsReserved(tag)) {
                    throw std::invalid_argument(
                            "Def file '" + file.string() + "' redefines reserved tag <" + tag + ">.");
                } else {
                    // Avoid redefining the same tag
                    if (defTags.contains(tag)) {
                        throw std::invalid_argument(
                                "Def file '" + file.string() + "' has multiple definitions for tag <" + tag + ">.");
                    }
                    defTags.insert(tag);
                }
            }
        }

        // If head not found (it is allowed to have no definitions)
        if (!head) {
            throw std::invalid_argument("Def file '" + file.string() + "' has no <head>.");
        }
    }

    // Helper function
    void loadHead(
            xmlNodePtr head, const std::filesystem::path& file, std::vector<std::filesystem::path>& xmlList,
            std::vector<std::filesystem::path>& cssList, std::vector<std::filesystem::path>& jsList) {

        for (xmlNodePtr xmlChild = head->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                // Check tag value
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                bool isDef = strcmp(tag, "def") == 0;
                bool isCSS = strcmp(tag, "style") == 0;
                bool isJS = strcmp(tag, "script") == 0;
                if (!isDef && !isCSS && !isJS) return;

                // Retrieve src
                for (xmlAttr* attr = xmlChild->properties; attr; attr = attr->next) {
                    const char* name = reinterpret_cast<const char*>(attr->name);
                    xmlChar* xmlValue = xmlNodeGetContent(attr->children);
                    const char* value = reinterpret_cast<const char*>(xmlValue);
                    if (strcmp(name, "src") == 0) {
                        if (isDef) {
                            xmlList.push_back(file.parent_path() / value);
                        } else if (isCSS) {
                            cssList.push_back(file.parent_path() / value);
                        } else if (isJS) {
                            jsList.push_back(file.parent_path() / value);
                        }
                    }
                    xmlFree(xmlValue);
                }
            }
        }
    }

    void DOM::init(const std::filesystem::path& file, std::vector<std::filesystem::path>& jsList) {
        this->file_ = file;
        xmlInitParser();

        // Stack of xml files to read
        std::vector<std::filesystem::path> xmlList;
        // List of css files to load
        std::vector<std::filesystem::path> cssList;
        // List of js files to run is already provided

        // helper functions
#include "dom_helper.h"

        // Load the head of target file and check for correctness
        {
            // Open XML file
            xmlDocPtr doc = xmlReadFile(this->file_.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        "Cannot load file '{}'.\n",
                        this->file_.string()));
            }

            // Parse target file's head
            xmlNodePtr head;
            parseXMLFile(this->file_, doc, head);

            // Load head
            loadHead(head, this->file_, xmlList, cssList, jsList);

            // Release XML file
            xmlFreeDoc(doc);
        }

        // 1st pass: load ONLY the head of the required def files and check for correctness
        for (size_t i = 0; i < xmlList.size(); i++) {
            std::filesystem::path curFile = xmlList[i];

            // Open XML file
            xmlDocPtr doc = xmlReadFile(curFile.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        "Cannot load file '{}'.\n",
                        curFile.string()));
            }

            // Parse def file's head
            xmlNodePtr head;
            std::unordered_set<std::string> defTags;
            parseXMLDefFile(curFile, doc, head, defTags);

            // Load head
            loadHead(head, curFile, xmlList, cssList, jsList);

            // Record new definitions
            for (const std::string& tag : defTags) {
                // Avoid definition collision
                if (this->defs_.contains(tag)) {
                    const std::filesystem::path& otherFile = this->defs_[tag].file;
                    throw std::invalid_argument(std::format(
                            "Tag <{}> definition collision in def files: '{}' and '{}'.\n",
                            tag, otherFile.string(), curFile.string()));
                }
                Def& def = this->defs_[tag];
                def.tag = tag;
                def.file = curFile;
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        // 2nd pass: load the def files from back to front and parse their body
        for (int i = (int)xmlList.size() - 1; i >= 0; i--) {
            std::filesystem::path curFile = xmlList[i];

            // Open XML file
            xmlDocPtr doc = xmlReadFile(curFile.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        "Cannot load file '{}'.\n",
                        curFile.string()));
            }

            // Load each definition
            xmlNodePtr root = xmlDocGetRootElement(doc);
            for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
                if (xmlChild->type == XML_ELEMENT_NODE) {
                    const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                    if (strcmp(tag, "head") != 0) {
                        loadXMLBodyToNode(xmlChild, this->defs_[tag].node, this->defs_);
                    }
                }
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        // Load the target file's body
        {
            // Open XML file
            xmlDocPtr doc = xmlReadFile(this->file_.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        "Cannot load file '{}'.\n",
                        this->file_.string()));
            }

            // Find and load <body>
            xmlNodePtr root = xmlDocGetRootElement(doc);
            for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
                if (xmlChild->type == XML_ELEMENT_NODE) {
                    const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                    if (strcmp(tag, "body") == 0) {
                        loadXMLBodyToNode(xmlChild, this->root_, this->defs_);
                    }
                }
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        // Load all CSS files after XML files are loaded
        for (const std::filesystem::path& cssFile: cssList) {
            this->cssom_.loadFromFile(cssFile);
        }

        // JS files will be loaded by UI class, no need to manage it here
    }
}
