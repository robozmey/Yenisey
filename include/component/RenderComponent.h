//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_RENDERCOMPONENT_H
#define YENISEY_RENDERCOMPONENT_H

#include <vector>
#include "Component.h"
#include "Player.h"

namespace yny {

    class RenderComponent : public Component {
    public:
        ComponentType type = Render;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;

        GLuint vao, vbo, ebo;
        GLuint program;

        virtual void render(Player& scene_player);

        RenderComponent();

    };

} // yny

#endif //YENISEY_RENDERCOMPONENT_H
