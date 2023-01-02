//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_WATERRENDERCOMPONENT_H
#define YENISEY_WATERRENDERCOMPONENT_H

#include "elevation/elevation.h"
#include "Object.h"
#include "Player.h"
#include "component/RenderComponent.h"

namespace yny {

    struct wave_func_t {
        float amplitude;
        glm::vec2 direction;
        float wavelength;
        float phase_constant;
    };

    class WaterRenderComponent : public RenderComponent {
    public:
        glm::vec3 p1, p2;

        float height;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;
        GLuint light_direction_location;

        std::vector<glm::vec3> offsets;
        GLuint offsets_vbo;

        GLuint vao, vbo, ebo;
        GLuint program;

        std::vector<wave_func_t> wave_funcs;
        float h(float x, float z, float t);
        float dhx(float x, float z, float t);
        float dhz(float x, float z, float t);

        int lod_count = 3;

        void update_vertices(Player& scene_player) override;

        void render(Player& scene_player) override;

        WaterRenderComponent();

    };

} // yny

#endif //YENISEY_WATERRENDERCOMPONENT_H
