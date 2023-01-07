//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_Component_H
#define YENISEY_Component_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace yny {
    class Object;

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texcoord;
    };

    enum ComponentType {
        Transform,
        Rigibody,
        Render,
        Collider,
        Mesh,
        Script,
    };

    class Component {
    public:
        Object* componentsObject = nullptr;
        ComponentType type;
    };

} // yny

#endif //YENISEY_Component_H
