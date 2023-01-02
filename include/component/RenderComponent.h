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

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        virtual void update_vertices(Player& scene_player);
        virtual void render(Player& scene_player);
        virtual void update_time(Player& scene_player);

    };

} // yny

#endif //YENISEY_RENDERCOMPONENT_H
