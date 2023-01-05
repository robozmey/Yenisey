//
// Created by vladimir on 02.01.23.
//

#include "component/ColliderComponent.h"
#include "component/TransformComponent.h"
#include "Object.h"

namespace yny {

    bool ColliderComponent::is_collide(ColliderComponent*) {
        return false;
    }

    ColliderComponent::ColliderComponent() : Component() {
        type = Collider;
    }

    float distance(glm::vec3 a, glm::vec3 b) {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    }

    bool SphereColliderComponent::is_collide(ColliderComponent * other) {
        if (other->collider_type == EmptyCollider)
            return false;

        if (other->collider_type == SphereCollider) {
            SphereColliderComponent* scc = static_cast<SphereColliderComponent *>(other);

            TransformComponent* tc1 = static_cast<TransformComponent *>(componentsObject->components[Transform]);
            TransformComponent* tc2 = static_cast<TransformComponent *>(other->componentsObject->components[Transform]);

            return distance(tc1->position, tc2->position) <= radius + scc->radius;
        }
    }
} // yny