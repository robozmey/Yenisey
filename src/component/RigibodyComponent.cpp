//
// Created by vladimir on 02.01.23.
//

#include "Object.h"
#include "Scene.h"
#include "component/RigibodyComponent.h"
#include "component/TransformComponent.h"

namespace yny {
    void RigibodyComponent::move(float dt) {
        yny::TransformComponent* tc = reinterpret_cast<yny::TransformComponent *>(componentsObject->components[yny::Transform]);
        tc->move(glm::inverse(rotation_matrix(tc->get_rotation())) * glm::vec4(speed, 1.f) * dt);

        if (componentsObject->is_collide(componentsObject->scene)) {
            speed = {0, 0, 0};
        } else {

            if (gravity) {
                speed += glm::vec3(0, -9.8, 0) * dt;
//                printf("%f %f %f\n", tc->get_position().x, tc->get_position().y, tc->get_position().z);
//                tc->move(gravity_speed * dt);
            }
        }
    }
} // yny