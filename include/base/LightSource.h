//
// Created by vladimir on 05.01.23.
//

#ifndef YENISEY_LIGHTSOURCE_H
#define YENISEY_LIGHTSOURCE_H

#include "glm/vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

namespace yny {

    enum LightSourceType {
        AmbientLight,
        DirectionalLight,
        SpotLight,
    };

    class Scene;

    class LightSource {
    public:
        LightSourceType lightSourceType;

        glm::vec3 direction;
        glm::vec3 position;

        glm::vec3 color = {1, 1, 1};
        float light_intensity = 0.2;

        glm::vec3 attenuation = {0.01, 0.0, 0.000001};

        int shadow_map_resolution = 1024;
        GLuint shadow_map;
        GLuint shadow_fbo;
        GLuint shadow_rbo;

        glm::mat4 shadow_transform;

        void shadow_render(Scene* scene);

        LightSource();
        LightSource(LightSourceType lightSourceType);
    private:
        void create_light_source();
    };

} // yny

#endif //YENISEY_LIGHTSOURCE_H
