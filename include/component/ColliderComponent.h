//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_COLLIDERCOMPONENT_H
#define YENISEY_COLLIDERCOMPONENT_H

#include <utility>
#include "component/Component.h"

namespace yny {

    enum ColliderType {
        EmptyCollider,
        BoxCollider,
        SphereCollider,
        MeshCollider,
        ElevationCollider,
    };

    class ColliderComponent : public Component {
    public:
        ColliderType collider_type = EmptyCollider;

        virtual bool is_collide(ColliderComponent*);

        virtual std::pair<glm::vec3, glm::vec3> bbox();

        ColliderComponent();
    };

    class SphereColliderComponent : public ColliderComponent {
    public:
        float radius = 1;

        bool is_collide(ColliderComponent*) override;

        SphereColliderComponent();
        explicit SphereColliderComponent(float radius);
    };

} // yny

#endif //YENISEY_COLLIDERCOMPONENT_H
