//
// Created by vladimir on 31.12.22.
//

#include "component/TransformComponent.h"
#include "Object.h"

glm::mat4 rotation_matrix(glm::vec3 rotation) {
    glm::mat4 view(1.f);
    view = glm::rotate(view, rotation.x, {1, 0, 0});
    view = glm::rotate(view, rotation.y, {0, 1, 0});
    view = glm::rotate(view, rotation.z, {0, 0, 1});
    return view;
}

namespace yny {
    TransformComponent::TransformComponent() {}

    TransformComponent::TransformComponent(glm::vec3 position) : position(position) {}

    void TransformComponent::move(glm::vec3 movement) {
        position += movement;
    }

    void TransformComponent::rotate(glm::vec3 additional_rotation) {
        rotation += additional_rotation;
    }

    glm::mat4 TransformComponent::get_transform() {
        glm::mat4 res(1);

        if (componentsObject != nullptr && componentsObject->parentObject != nullptr) {
            res = (reinterpret_cast<TransformComponent*>(componentsObject->parentObject->components[Transform]))->get_transform();
        }

        res = rotation_matrix(rotation);
        res = glm::translate(res, position);

        return res;
    }
} // yny