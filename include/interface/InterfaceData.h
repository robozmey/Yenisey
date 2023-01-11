//
// Created by vladimir on 11.01.23.
//

#ifndef YENISEY_INTERFACEDATA_H
#define YENISEY_INTERFACEDATA_H

#include <string>
#include "msdf_loader.hpp"

namespace yny {

    class InterfaceData {
    public:
        msdf_font font;
        bool text_changed = true;
        std::u32string text = U"Do you like zucchini?";

        InterfaceData();
    };

} // yny

#endif //YENISEY_INTERFACEDATA_H
