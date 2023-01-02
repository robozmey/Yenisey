//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_SPHERERENDERCOMPONENT_H
#define YENISEY_SPHERERENDERCOMPONENT_H

#include "component/RenderComponent.h"

namespace yny {

    class SphereRenderComponent : public RenderComponent {
    public:

        void generate_sphere(float radius, int quality);

        void update_vertices(Player& scene_player) override;

        SphereRenderComponent();
    };

} // yny

#endif //YENISEY_SPHERERENDERCOMPONENT_H
