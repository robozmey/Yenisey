//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_WATERRENDERCOMPONENT_H
#define YENISEY_WATERRENDERCOMPONENT_H

#include "elevation/elevation.h"
#include "base/Object.h"
#include "base/Camera.h"
#include "component/RenderComponent.h"

namespace yny {

    class WaterRenderComponent : public RenderComponent {
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

        void render(Camera* scene_player, GLuint light_map) override;

        WaterRenderComponent();

    };

} // yny

#endif //YENISEY_WATERRENDERCOMPONENT_H
