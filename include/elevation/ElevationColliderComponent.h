//
// Created by vladimir on 09.01.23.
//

#ifndef YENISEY_ELEVATIONCOLLIDERCOMPONENT_H
#define YENISEY_ELEVATIONCOLLIDERCOMPONENT_H

#include "component/ColliderComponent.h"
#include "ElevationDataObject.h"

namespace yny {

    class ElevationColliderComponent : public ColliderComponent {
    public:

        ElevationDataObject* elevationDataObject{};

        bool is_collide(ColliderComponent*) override;

        ElevationColliderComponent();
        explicit ElevationColliderComponent(ElevationDataObject* elevationDataObject);

    };

} // yny

#endif //YENISEY_ELEVATIONCOLLIDERCOMPONENT_H
