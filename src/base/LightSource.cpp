//
// Created by vladimir on 05.01.23.
//

#include "base/LightSource.h"

namespace yny {
    LightSource::LightSource() {
        lightSourceType = AmbientLight;
        light_intensity = 0.2;
    }
} // yny