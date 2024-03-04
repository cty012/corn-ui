#include <iostream>
extern "C" {
    #include <libxml/parser.h>
}
#include <corn/core.h>

int main() {
    xmlInitParser();
    xmlDocPtr doc = xmlReadFile("example.xml", NULL, 0);
    if (doc == nullptr) {
        printf("Cannot load file!\n");
        return 1;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (xmlStrcmp(root->name, BAD_CAST "library") != 0) {
        std::cerr << "Root element is not 'library'\n";
    } else {
        std::cout << "Root element is 'library'\n";
    }

    for (xmlNodePtr node1 = root->children; node1; node1 = node1->next) {
        if (node1->type == XML_ELEMENT_NODE && xmlStrcmp(node1->name, BAD_CAST "book") == 0) {
            for (xmlNodePtr node2 = node1->children; node2; node2 = node2->next) {
                if (node2->type == XML_ELEMENT_NODE && xmlStrcmp(node2->name, BAD_CAST "title") == 0) {
                    xmlChar* title = xmlNodeGetContent(node2);
                    if (title) {
                        printf("Book found: %s\n", title);
                        xmlFree(title);
                    }
                    break;
                }
            }
        }
    }

    xmlFreeDoc(doc);
    return 0;
}
