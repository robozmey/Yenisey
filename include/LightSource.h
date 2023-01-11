//
// Created by vladimir on 05.01.23.
//

#ifndef YENISEY_LIGHTSOURCE_H
#define YENISEY_LIGHTSOURCE_H

#include "glm/vec3.hpp"

namespace yny {

    enum LightSourceType {
        AmbientLight,
        DirectionalLight,
        SpotLight,
    };

    class LightSource {
    public:
        LightSourceType lightSourceType;

        glm::vec3 direction;
        glm::vec3 position;

        glm::vec3 color = {1, 1, 1};
        float light_intensity = 0.2;

        glm::vec3 attenuation = {1.0, 0.0, 0.001};

        LightSource();
    };

} // yny

#endif //YENISEY_LIGHTSOURCE_H
