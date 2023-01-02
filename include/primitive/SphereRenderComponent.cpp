//
// Created by vladimir on 02.01.23.
//

#include "SphereRenderComponent.h"
#include "tools.h"


namespace yny {
    void SphereRenderComponent::generate_sphere(float radius, int quality) {

        vertices.clear();
        indices.clear();

        for (int latitude = -quality; latitude <= quality; ++latitude)
        {
            for (int longitude = 0; longitude <= 4 * quality; ++longitude)
            {
                float lat = (latitude * glm::pi<float>()) / (2.f * quality);
                float lon = (longitude * glm::pi<float>()) / (2.f * quality);

                auto & vertex = vertices.emplace_back();
                vertex.normal = {std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon)};
                vertex.position = vertex.normal * radius;
                vertex.tangent = {-std::cos(lat) * std::sin(lon), 0.f, std::cos(lat) * std::cos(lon)};
            }
        }

        for (int latitude = 0; latitude < 2 * quality; ++latitude)
        {
            for (int longitude = 0; longitude < 4 * quality; ++longitude)
            {
                std::uint32_t i0 = (latitude + 0) * (4 * quality + 1) + (longitude + 0);
                std::uint32_t i1 = (latitude + 1) * (4 * quality + 1) + (longitude + 0);
                std::uint32_t i2 = (latitude + 0) * (4 * quality + 1) + (longitude + 1);
                std::uint32_t i3 = (latitude + 1) * (4 * quality + 1) + (longitude + 1);

                indices.insert(indices.end(), {i0, i1, i2, i2, i1, i3});
            }
        }

    }

    void SphereRenderComponent::update_vertices(Player& scene_player) {
        generate_sphere(100, 5);
    }

    SphereRenderComponent::SphereRenderComponent() {

    }
} // yny