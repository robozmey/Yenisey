//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_ELEVATIONRENDERCOMPONENT_H
#define YENISEY_ELEVATIONRENDERCOMPONENT_H

#include "component/RenderComponent.h"
#include "elevation.h"

namespace yny {

    class ElevationRenderComponent : public RenderComponent {
    public:
        GLuint grass_texture;
        GLuint grass_normal_texture;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;
        GLuint grass_texture_location;
        GLuint grass_normal_texture_location;

        GLuint light_direction_location;

        int lod_count = 4;
        std::vector<elevation_map_t> elevation_map_lod;
        GLuint vao, vbo, ebo;
        GLuint program;

        int latitude_minute = 0, longitude_minute = 0;

        void render(Player& scene_player) override;

        ElevationRenderComponent();

    private:
        void recalc_terrain(Player& scene_player);

    };

} // yny

#endif //YENISEY_ELEVATIONRENDERCOMPONENT_H
