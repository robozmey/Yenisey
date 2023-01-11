//
// Created by vladimir on 09.01.23.
//

#include "elevation/ElevationColliderComponent.h"
#include "component/TransformComponent.h"
#include "Object.h"

namespace yny {
    ElevationColliderComponent::ElevationColliderComponent() {
        collider_type = ElevationCollider;
        elevationDataObject = new ElevationDataObject();
    }

    ElevationColliderComponent::ElevationColliderComponent(ElevationDataObject *elevationDataObject) : elevationDataObject(elevationDataObject) {
        collider_type = ElevationCollider;
    }

    bool ElevationColliderComponent::is_collide(ColliderComponent *other) {
        TransformComponent* tc1 = static_cast<TransformComponent *>(componentsObject->components[Transform]);
        TransformComponent* tc2 = static_cast<TransformComponent *>(other->componentsObject->components[Transform]);

        switch (other->collider_type) {
            case(SphereCollider): {
                SphereColliderComponent* scc = static_cast<SphereColliderComponent *>(other);

                float other_height = tc2->get_position().y;

                float center_latitude_grad = elevationDataObject->latitude + elevationDataObject->latitude_minute / 60.f;
                float center_longitude_grad = elevationDataObject->longitude + elevationDataObject->longitude_minute / 60.f;

                float latitude_factor = wgs_distance(center_latitude_grad,
                                                     center_longitude_grad,
                                                     center_latitude_grad + 1 / 3600.f,
                                                     center_longitude_grad) * 1000;
                float longitude_factor = wgs_distance(center_latitude_grad,
                                                      center_longitude_grad,
                                                      center_latitude_grad,
                                                      center_longitude_grad + 1 / 3600.f) * 1000;

                glm::vec3 diff = tc2->get_position() - tc1->get_position();
                diff.x /= latitude_factor;
                diff.z /= longitude_factor;

                diff.x += ELEVATION_MAP_SIZE / 2 - elevationDataObject->latitude_minute * 60;
                diff.z += ELEVATION_MAP_SIZE / 2 - elevationDataObject->longitude_minute * 60;

                float height = elevationDataObject->get_data(elevationDataObject->latitude, elevationDataObject->longitude, 0, diff.x, diff.z);

//                printf("%f %f\n", diff.x, diff.z);

                return (other_height - height) <= scc->radius;
            }

        }

        return false;
    }
} // yny