#include <iostream>
#include <corn/media.h>
#include <cornui/dom.h>

int main() {
    corn::FontManager::instance().load(
            "noto-sans-zh", "fonts/noto-sans-zh/static/NotoSansSC-Regular.ttf");
    cornui::DOMTree tree("example.xml");
    auto contents = getOuterXML(tree.getRoot());
    printf("%s\n", reinterpret_cast<const char*>(contents.c_str()));
    return 0;
}
