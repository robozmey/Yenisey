//
// Created by vladimir on 31.12.22.
//

#include "TransformComponent.h"

namespace yny {
    TransformComponent::TransformComponent() {}

    TransformComponent::TransformComponent(glm::vec3 position) : position(position) {}

    void TransformComponent::move(glm::vec3 movement) {
        position += movement;
    }

    void TransformComponent::rotate(glm::vec3 rotation1) {
        rotation += rotation1;
    }
} // yny