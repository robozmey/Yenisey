#ifndef YENISEY_ELEVATION_H
#define YENISEY_ELEVATION_H

#include <fstream>
#include <vector>

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

namespace yny {

    const int ELEVATION_MAP_SIZE = 3601;

    #define elevation_map_t std::vector<std::vector<uint16_t>>

    void upload_elevation_map(int latitude, int longitude, elevation_map_t &elevation_map) ;

    GLuint load_elevation_texture(elevation_map_t &elevation_map);
}

#endif //YENISEY_ELEVATION_H