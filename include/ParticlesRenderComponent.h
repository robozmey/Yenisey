//
// Created by vladimir on 09.01.23.
//

#ifndef YENISEY_PARTICLESRENDERCOMPONENT_H
#define YENISEY_PARTICLESRENDERCOMPONENT_H

#include <random>
#include "component/RenderComponent.h"

namespace yny {

    const int PARTICLES_MAX_COUNT = 256;

    struct particle
    {
        glm::vec3 position;
        float size;
        glm::vec3 velocity;
        float rotation;
        float angular_velocity;

        particle(std::default_random_engine& rng) {
            printf("dgsdsdg");

            position.x = std::uniform_real_distribution<float>{-1.f, 1.0f}(rng);
            position.y = std::uniform_real_distribution<float>{0.0f, 1.0f}(rng);
            position.z = std::uniform_real_distribution<float>{-1.0f, 1.0f}(rng);

            size = std::uniform_real_distribution<float>{1000.f, 1000.f}(rng);

            velocity.x = std::uniform_real_distribution<float>{-0.15f, 0.15f}(rng);
            velocity.y = std::uniform_real_distribution<float>{-0.35f, -0.2}(rng);
            velocity.z = std::uniform_real_distribution<float>{-0.15f, 0.15f}(rng);

            rotation = 0;
            angular_velocity = std::uniform_real_distribution<float>{0, 0.5f}(rng);
        }
    };

    class ParticlesRenderComponent : public RenderComponent {
    public:
        std::default_random_engine rng;
        std::vector<particle> particles;

        void render(Camera* camera, GLuint light_map) override;

        ParticlesRenderComponent();
    };

} // yny

#endif //YENISEY_PARTICLESRENDERCOMPONENT_H
