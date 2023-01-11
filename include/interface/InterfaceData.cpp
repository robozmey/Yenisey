//
// Created by vladimir on 11.01.23.
//

#include "InterfaceData.h"

namespace yny {
    InterfaceData::InterfaceData() {
        const std::string project_root = PROJECT_ROOT;
        const std::string font_path = project_root + "/font/Comic Sans MS-msdf.json";

        font = load_msdf_font(font_path);
    }
} // yny