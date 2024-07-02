std::function<void(xmlNodePtr, DOMNode&, const std::unordered_map<std::string, Def>&, const std::string&, bool)> loadXMLBodyToNode =
        [&](xmlNodePtr xmlNode, DOMNode& node, const std::unordered_map<std::string, Def>& defs,
                const std::string& rootTag, bool isRoot) {

            if (xmlNode->type != XML_ELEMENT_NODE) {
                throw std::logic_error("xmlNode must have type XML_ELEMENT_NODE.");
            }
            node.clear();
            node.dom_ = this;
            node.widgetID_ = 0;

            // Copy tag
            node.tag_ = reinterpret_cast<const char*>(xmlNode->name);
            bool isDefNode = !tagIsReserved(node.tag_);

            if (isDefNode) {
                if (isRoot) {
                    // NEVER use templates for root nodes
                    // Instead reset their tag to widget
                    node.tag_ = "widget";
                } else {
                    // Apply template if the non-root node is a def node
                    // But make sure to avoid recursions
                    if (node.tag_ == rootTag) {
                        throw std::invalid_argument(std::format(
                                "Recursive definition for tag <{}>.",
                                rootTag));
                    } else if (defs.contains(node.tag_)) {
                        node = defs.at(node.tag_).node;
                    } else {
                        // todo: Do not use invalid argument
                        throw std::invalid_argument(std::format(
                                "Tag definition not found for <{}>.",
                                node.tag_));
                    }
                }
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
                        if (std::find(node.classList_.begin(), node.classList_.end(), token) == node.classList_.end()) {
                            node.classList_.push_back(token);
                        }
                    }
                } else if (strcmp(name, "style") == 0) {
                    // Parse the style
                    std::unordered_map<std::string, std::string> styleTemp = parseDeclFromString(value);
                    for (const auto& [key, val] : styleTemp) {
                        node.style_[key] = val;
                    }
                } else if (attrIsScript(name)) {
                    // Insert the "on-xxx" attributes
                    node.attributes_[name] = corn::trim(value);
                } else {
                    // Insert the normal attributes
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

            // Copy children
            if (isRoot || !isDefNode) {
                for (xmlNodePtr xmlChild = xmlNode->children; xmlChild; xmlChild = xmlChild->next) {
                    if (xmlChild->type == XML_ELEMENT_NODE) {
                        auto* child = new DOMNode();
                        loadXMLBodyToNode(xmlChild, *child, defs, rootTag, false);
                        child->parent_ = &node;
                        node.children_.push_back(child);
                    }
                }
            }
        };
