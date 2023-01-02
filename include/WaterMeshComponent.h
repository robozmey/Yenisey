//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_WATERMESHCOMPONENT_H
#define YENISEY_WATERMESHCOMPONENT_H

#include "component/MeshComponent.h"

namespace yny {

    struct wave_func_t {
        float amplitude;
        glm::vec2 direction;
        float wavelength;
        float phase_constant;
    };

    class WaterMeshComponent : public MeshComponent {
    public:

        std::vector<glm::vec3> offsets;
        std::vector<wave_func_t> wave_funcs;
        float h(float x, float z, float t);
        float dhx(float x, float z, float t);
        float dhz(float x, float z, float t);

        int lod_count = 3;

        void update_vertices(Player& scene_player) override;

        WaterMeshComponent();

    };

} // yny

#endif //YENISEY_WATERMESHCOMPONENT_H
