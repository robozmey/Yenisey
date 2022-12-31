//
// Created by vladimir on 30.12.22.
//

#ifndef YENISEY_WATER_H
#define YENISEY_WATER_H


#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include "elevation.h"
#include "Object.h"
#include "Player.h"


namespace yny {

    struct wave_func_t {
        float amplitude;
        glm::vec2 direction;
        float wavelength;
        float phase_constant;
    };

    class Water : public Object {
    public:
        glm::vec3 p1, p2;

        float height;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;
        GLuint light_direction_location;

        GLuint offsets_vbo;
        GLuint vao, vbo, ebo;
        GLuint program;

        std::vector<wave_func_t> wave_funcs;
        float h(float x, float z, float t);
        float dhx(float x, float z, float t);
        float dhz(float x, float z, float t);

        void render(Player& scene_player) override;

        Water();

    };

} // yny

#endif //YENISEY_WATER_H
