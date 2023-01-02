//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_Component_H
#define YENISEY_Component_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace yny {

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
    };

    class Component {
        ComponentType type;
    };

} // yny

#endif //YENISEY_Component_H
