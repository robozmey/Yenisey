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

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    class ElevationObject : public Object {
    public:
        GLuint texture_location;
        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;

        GLuint light_direction_location;

        int lod_count = 5;
        std::vector<elevation_map_t> elevation_map_lod;
        GLuint elevation_texture;
        GLuint vao, vbo, ebo;
        GLuint program;
        Player& scene_player;

        int latitude_minute = 0, longitude_minute = 0;
        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        void render() override;

        explicit ElevationObject(Player& scene_player);
    private:
        void recalc_terrain();
    };

} // yny

#endif //YENISEY_ELEVATIONOBJECT_H
