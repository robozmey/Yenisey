//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_SPHEREMESHCOMPONENT_H
#define YENISEY_SPHEREMESHCOMPONENT_H

#include "component/MeshComponent.h"

namespace yny {

    class SphereMeshComponent : public MeshComponent {

        void generate_sphere(float radius, int quality);

        void update_vertices(Player& scene_player) override;
    };

} // yny

#endif //YENISEY_SPHEREMESHCOMPONENT_H
