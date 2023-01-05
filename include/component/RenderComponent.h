//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_RENDERCOMPONENT_H
#define YENISEY_RENDERCOMPONENT_H

#include <vector>
#include "Component.h"
#include "Player.h"
#include "Material.h"

namespace yny {

    class RenderComponent : public Component {
    public:
        ComponentType type = Render;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;
        GLuint transform_location;

        GLuint vao, vbo, ebo;
        GLuint program;

        Material* material;

        GLuint material_type_location;
        GLuint material_color_location;
        GLuint material_texture_location;

        virtual void render(Player& scene_player);

        RenderComponent();

        explicit RenderComponent(Material* material);

    private:
        void create_render_component();

    };

} // yny

#endif //YENISEY_RENDERCOMPONENT_H
