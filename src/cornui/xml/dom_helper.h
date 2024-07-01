std::function<void(xmlNodePtr, DOMNode&, const std::unordered_map<std::string, Def>&)> loadXMLBodyToNode =
        [&](xmlNodePtr xmlNode, DOMNode& node, const std::unordered_map<std::string, Def>& defs) {
            if (xmlNode->type != XML_ELEMENT_NODE) {
                throw std::invalid_argument("xmlNode must have type XML_ELEMENT_NODE");
            }
            node.clear();
            node.dom_ = this;
            node.widgetID_ = 0;

            // Copy tag
            node.tag_ = reinterpret_cast<const char*>(xmlNode->name);
            bool isDefNode = !tagIsReserved(node.tag_);

            // Apply template (def node)
            if (isDefNode) {
                node = defs.at(node.tag_).node;
                node.tag_ = "widget";
            }

            // Default name
            node.name_ = "";

            // Copy name, class list, and attributes
            for (xmlAttr* attr = xmlNode->properties; attr; attr = attr->next) {
                const char* name = reinterpret_cast<const char*>(attr->name);
                xmlChar* xmlValue = xmlNodeGetContent(attr->children);
                const char* value = reinterpret_cast<const char*>(xmlValue);
                if (strcmp(name, "name") == 0) {
                    node.name_ = value;
                } else if (strcmp(name, "class") == 0) {
                    // Split the value and fill the class list
                    std::istringstream iss(value);
                    std::string token;
                    while (iss >> token) {
                        node.classList_.push_back(token);
                    }
                } else if (strcmp(name, "style") == 0) {
                    // Parse the style
                    node.style_ = parseDeclFromString(value);
                } else if (attrIsScript(name)) {
                    // Insert the "on-xxx" attribute
                    node.attributes_[name] = corn::trim(value);
                } else {
                    // Insert the attribute
                    node.attributes_[name] = value;
                }
                xmlFree(xmlValue);
            }

            // Copy text
            std::stringstream ssText;
            for (xmlNodePtr xmlChild = xmlNode->children; xmlChild; xmlChild = xmlChild->next) {
                if (xmlChild->type == XML_TEXT_NODE) {
                    ssText << (char*)xmlChild->content;
                }
            }
            std::string temp = ssText.str();
            corn::trim(temp);
            node.text_ = reinterpret_cast<const char8_t*>(corn::trim(ssText.str()).c_str());

            // Copy children (not def node)
            if (!isDefNode) {
                for (xmlNodePtr xmlChild = xmlNode->children; xmlChild; xmlChild = xmlChild->next) {
                    if (xmlChild->type == XML_ELEMENT_NODE) {
                        auto* child = new DOMNode();
                        loadXMLBodyToNode(xmlChild, *child, defs);
                        child->parent_ = &node;
                        node.children_.push_back(child);
                    }
                }
            }
        };
