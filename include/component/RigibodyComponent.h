//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_RIGIBODYCOMPONENT_H
#define YENISEY_RIGIBODYCOMPONENT_H

#include "Component.h"

namespace yny {

    class RigibodyComponent : public Component{
    public:
        ComponentType type = Render;

        glm::vec3 speed = {0, 0, 0};

        bool gravity = true;
        glm::vec3 gravity_speed = {0, -10, 0};

        void move(float dt);

    };

} // yny

#endif //YENISEY_RIGIBODYCOMPONENT_H
