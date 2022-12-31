//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_ELEVATIONOBJECT_H
#define YENISEY_ELEVATIONOBJECT_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include "elevation.h"
#include "Object.h"
#include "Player.h"

namespace yny {

    class ElevationObject : public Object {
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

        ElevationObject();
    private:
        void recalc_terrain(Player& scene_player);
    };

} // yny

#endif //YENISEY_ELEVATIONOBJECT_H
