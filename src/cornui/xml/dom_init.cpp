#include <sstream>
#include <stdexcept>
#include <format>
#include <unordered_map>
extern "C" {
#include <libxml/parser.h>
}
#include <corn/util/string_utils.h>
#include <cornui/util/css_parser.h>
#include <cornui/util/reserved.h>
#include <cornui/xml/dom.h>
#include "../util/graph.h"

namespace cornui {
    using std::filesystem::path;

    void parseXMLFile(const path& file, xmlDocPtr doc, xmlNodePtr& head) {
        head = nullptr;
        xmlNodePtr body = nullptr;

        // Root node must be <cornui>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui") != 0) {
            throw std::invalid_argument(std::format(
                    R"(File "{}" has root element with tag <{}>, but <cornui> expected.)",
                    file.string(), rootTag));
        }

        // Find <head> and <body>
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (strcmp(tag, "head") == 0) {
                    // Avoid multiple heads
                    if (head) {
                        throw std::invalid_argument(std::format(
                                R"(File "{}" has multiple <head> nodes under root.)",
                                file.string()));
                    }
                    head = xmlChild;
                } else if (strcmp(tag, "body") == 0) {
                    // Avoid multiple heads
                    if (body) {
                        throw std::invalid_argument(std::format(
                                R"(File "{}" has multiple <body> nodes under root.)",
                                file.string()));
                    }
                    body = xmlChild;
                } else {
                    throw std::invalid_argument(std::format(
                            R"(File "{}" has invalid node with tag <{}> under root.)",
                            file.string(), tag));
                }
            }
        }

        // If either not found
        if (!head && !body) {
            throw std::invalid_argument(std::format(
                    R"(File "{}" has no <head> and <body>.)",
                    file.string()));
        } else if (!head) {
            throw std::invalid_argument(std::format(
                    R"(File "{}" has no <head>.)",
                    file.string()));
        } else if (!body) {
            throw std::invalid_argument(std::format(
                    R"(File "{}" has no <body>.)",
                    file.string()));
        }
    }

    void parseXMLDefFile(
            const path& file, xmlDocPtr doc, xmlNodePtr& head, std::unordered_map<std::string, xmlNodePtr>* defTags) {

        head = nullptr;
        if (defTags) defTags->clear();

        // Root node must be <cornui-def>
        xmlNodePtr root = xmlDocGetRootElement(doc);
        const char* rootTag = reinterpret_cast<const char*>(root->name);
        if (strcmp(rootTag, "cornui-def") != 0) {
            throw std::invalid_argument(std::format(
                    R"(Def file "{}" has root element with tag <{}>, but <cornui-def> expected.)",
                    file.string(), rootTag));
        }

        // Find <head> and definitions
        for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
            if (xmlChild->type == XML_ELEMENT_NODE) {
                const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                if (strcmp(tag, "head") == 0) {
                    // Avoid multiple heads
                    if (head) {
                        throw std::invalid_argument(std::format(
                                R"(Def file "{}" has multiple <head> nodes under root.)",
                                file.string()));
                    }
                    head = xmlChild;
                } else if (tagIsReserved(tag)) {
                    throw std::invalid_argument(std::format(
                            R"(Def file "{}" redefines reserved tag <{}>.)",
                            file.string(), tag));
                } else if (defTags) {
                    // Avoid redefining the same tag
                    if (defTags->contains(tag)) {
                        throw std::invalid_argument(std::format(
                                R"(Def file "{}" has multiple definitions for tag <{}>.)",
                                file.string(), tag));
                    }
                    (*defTags)[tag] = xmlChild;
                }
            }
        }

        // If head not found (it is allowed to have no definitions)
        if (!head) {
            throw std::invalid_argument(std::format(
                    R"(Def file "{}" has no <head>.)",
                    file.string()));
        }
    }

    // Helper function
    void loadHead(
            xmlNodePtr head, const path& file, std::vector<path>* xmlList,
            std::vector<path>* cssList, std::vector<path>* jsList) {

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
                        if (isDef && xmlList) {
                            xmlList->push_back(file.parent_path() / value);
                        }
                        if (isCSS && cssList) {
                            cssList->push_back(file.parent_path() / value);
                        }
                        if (isJS && jsList) {
                            jsList->push_back(file.parent_path() / value);
                        }
                    }
                    xmlFree(xmlValue);
                }
            }
        }
    }

    void DOM::init(const path& file, std::vector<path>& jsList) {
        this->file_ = file;
        jsList.clear();
        xmlInitParser();

        // Graph representing the dependencies between XML def files.
        std::unordered_map<path, std::vector<path>> xmlGraph;
        // List of XML def files to load (in topological order)
        std::vector<path> xmlList;
        // List of CSS files to load
        std::vector<path> cssList;
        // List of JS files to run is already provided

        // helper function
#include "dom_helper.h"

        /**
         * STEP 1: Resolve Dependency
         * 1. Find all XML def files that need to be loaded
         * 2. Detect cyclic dependencies
         * 3. Sort XML def files in topological order
         */
        std::queue<path> xmlToLoad;
        std::unordered_set<path> pathsFound;
        xmlToLoad.push(this->file_);
        pathsFound.insert(this->file_);

        // Load the head of target file and check for correctness
        while (!xmlToLoad.empty()) {
            path curFile = xmlToLoad.front();
            xmlToLoad.pop();

            // Open XML file
            xmlDocPtr doc = xmlReadFile(curFile.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        R"(Cannot load file "{}".)",
                        curFile.string()));
            }

            // Load head
            xmlNodePtr head;
            if (curFile == this->file_) {
                parseXMLFile(curFile, doc, head);
            } else {
                parseXMLDefFile(curFile, doc, head, nullptr);
            }

            // Resolve file's dependencies
            std::vector<path>& xmlListTemp = xmlGraph[curFile];
            loadHead(head, curFile, &xmlListTemp, nullptr, nullptr);

            // Detect cyclic dependencies
            if (detectCycle(xmlGraph, curFile)) {
                throw std::invalid_argument(std::format(
                        R"(Cyclic dependency found for file "{}".)",
                        curFile.string()));
            }

            // Dependencies need to be analyzed
            for (const path& xmlPath : xmlListTemp) {
                if (!pathsFound.contains(xmlPath)) {
                    xmlToLoad.push(xmlPath);
                    pathsFound.insert(xmlPath);
                }
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        // Sort in topological order
        xmlGraph.erase(this->file_);
        sortInTopoOrder(xmlGraph, xmlList);

        /**
         * STEP 2: Create def prototypes
         * 1. List the CSS and JS files
         * 2. List the tag definitions
         * 3. Compute def nodes
         */
        for (const path& defFile : xmlList) {
            // Open XML file
            xmlDocPtr doc = xmlReadFile(defFile.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        R"(Cannot load file "{}".)",
                        defFile.string()));
            }

            // Parse def file's head
            xmlNodePtr head;
            std::unordered_map<std::string, xmlNodePtr> defTags;
            parseXMLDefFile(defFile, doc, head, &defTags);

            // List CSS and JS files
            loadHead(head, defFile, nullptr, &cssList, &jsList);

            // List tag definitions
            for (const auto& [tag, node] : defTags) {
                // Avoid definition collision
                if (this->defs_.contains(tag)) {
                    const std::filesystem::path& otherFile = this->defs_[tag].file;
                    if (defFile != otherFile) {
                        throw std::invalid_argument(std::format(
                                R"(Tag <{}> definition collision in def files: "{}" and "{}".)",
                                tag, otherFile.string(), defFile.string()));
                    }
                } else {
                    Def& def = this->defs_[tag];
                    def.tag = tag;
                    def.file = defFile;
                    // Compute def node
                    loadXMLBodyToNode(node, this->defs_[tag].node, this->defs_, tag, true);
                }
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        /**
         * STEP 3: Load target file
         * 1. Add to the existing list of CSS and JS files
         * 2. Use def nodes to compute body
         */
        {
            // Open XML file
            xmlDocPtr doc = xmlReadFile(this->file_.string().c_str(), nullptr, 0);
            if (doc == nullptr) {
                throw std::invalid_argument(std::format(
                        "Cannot load file \"{}\".",
                        this->file_.string()));
            }

            // Parse def file's head
            xmlNodePtr head;
            parseXMLFile(this->file_, doc, head);

            // List CSS and JS files
            loadHead(head, this->file_, &xmlList, &cssList, &jsList);

            // Find and load <body>
            xmlNodePtr root = xmlDocGetRootElement(doc);
            for (xmlNodePtr xmlChild = root->children; xmlChild; xmlChild = xmlChild->next) {
                if (xmlChild->type == XML_ELEMENT_NODE) {
                    const char* tag = reinterpret_cast<const char*>(xmlChild->name);
                    if (strcmp(tag, "body") == 0) {
                        loadXMLBodyToNode(xmlChild, this->root_, this->defs_, tag, true);
                        break;
                    }
                }
            }

            // Release XML file
            xmlFreeDoc(doc);
        }

        // Load all CSS files after XML files are loaded
        for (const path& cssFile: cssList) {
            this->cssom_.loadFromFile(cssFile);
        }

        // JS files will be loaded by UI class, no need to manage it here
    }
}
