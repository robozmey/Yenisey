//
// Created by vladimir on 02.01.23.
//

#include "Object.h"
#include "component/RigibodyComponent.h"
#include "component/TransformComponent.h"

namespace yny {
    void RigibodyComponent::move(float dt) {
        yny::TransformComponent* tc = reinterpret_cast<yny::TransformComponent *>(componentsObject->components[yny::Transform]);
        tc->move(speed * dt);

        if (gravity) {
            speed += glm::vec3(0, -9.8, 0) * dt;
//            tc->move(gravity_speed * dt);
        }
    }
} // yny