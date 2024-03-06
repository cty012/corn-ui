#include <iostream>
#include <cornui/dom.h>

int main() {
    cornui::DOMTree tree("example.xml");
    auto contents = getOuterXML(tree.getRoot());
    printf("%s\n", reinterpret_cast<const char*>(contents.c_str()));
    return 0;
}
