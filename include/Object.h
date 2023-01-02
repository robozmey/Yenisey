//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_OBJECT_H
#define YENISEY_OBJECT_H

#include "Component.h"
#include "Player.h"

#include <vector>

namespace yny {

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texcoord;
    };

    class Object {
    public:
        std::map<ComponentType, Component*> components;

        std::vector<Object*> objects;

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        void apply_transform();

        virtual void update_vertices(Player& scene_player);
        virtual void render(Player& scene_player);
        virtual void update_time(Player& scene_player);

        Object();
    };

} // yny

#endif //YENISEY_OBJECT_H
