//
// Created by vladimir on 02.01.23.
//

#include "component/MeshComponent.h"
#include "component/TransformComponent.h"
#include "base/Object.h"

namespace yny {
    void MeshComponent::apply_transform() {
        TransformComponent* transform_component = reinterpret_cast<TransformComponent *>(componentsObject->components[Transform]);

        for (vertex& v : vertices) {
            glm::mat4 transform = glm::mat4(1);
            transform = glm::rotate(transform, transform_component->rotation.x, {1, 0, 0});
            transform = glm::rotate(transform, transform_component->rotation.y, {0, 1, 0});
            transform = glm::rotate(transform, transform_component->rotation.z, {0, 0, 1});
            transform = glm::translate(transform, transform_component->position);

            v.position = glm::vec3(transform * glm::vec4(v.position, 1));
        }
    }

    void MeshComponent::update_vertices(Camera* scene_player) {}

    MeshComponent::MeshComponent() : Component() {
        type = Mesh;
    }

    std::vector<glm::vec2> MeshComponent::get_bounding_box() {
        std::vector<glm::vec2> bounding_box(3);
        for (int i = 0; i < 3; i++) {
            bounding_box[i][0] = 0;
            bounding_box[i][1] = 0;
        }
        return bounding_box;
    }
}