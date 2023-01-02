//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_COLLIDERCOMPONENT_H
#define YENISEY_COLLIDERCOMPONENT_H

#include "component/Component.h"

namespace yny {

    enum ColliderType {
        EmptyCollider,
        BoxCollider,
        SphereCollider,
        MeshCollider,
    };

    class ColliderComponent : public Component {
    public:
        ColliderType collider_type = EmptyCollider;

        virtual bool is_collide(ColliderComponent*);

        ColliderComponent();
    };

    class SphereColliderComponent : public ColliderComponent {
        float radius = 1;

        bool is_collide(ColliderComponent*) override;
    };

} // yny

#endif //YENISEY_COLLIDERCOMPONENT_H
