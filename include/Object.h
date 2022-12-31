//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_OBJECT_H
#define YENISEY_OBJECT_H

#include "Feature.h"
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
        std::vector<Feature> features;

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        virtual void render(Player& scene_player);
    };

} // yny

#endif //YENISEY_OBJECT_H
