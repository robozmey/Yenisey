#include <fstream>
#include <vector>
#include <iostream>

#include "elevation/elevation.h"

namespace yny {

    void upload_elevation_map(int latitude, int longitude, elevation_map_t &elevation_map) {
        std::cout << latitude << " " << longitude << std::endl;
        std::string latitude_prefix = std::string(latitude > 0 ? "N" : "S") + (latitude < 10 ? "0" : "");
        std::string longitude_prefix =
                std::string(longitude > 0 ? "E" : "W") + (longitude < 10 ? "00" : (longitude < 100 ? "0" : ""));
        std::string filename =
                std::string("elevation/") + latitude_prefix + std::to_string(latitude) + longitude_prefix +
                std::to_string(longitude) + ".hgt";
        std::ifstream elevation_stream;
        elevation_stream.open(filename, std::ios::binary | std::ios::in);
        elevation_map.assign(ELEVATION_MAP_SIZE, std::vector<uint16_t>(ELEVATION_MAP_SIZE, 0));
        for (int i = 0; i < ELEVATION_MAP_SIZE; i++) {
            for (int j = 0; j < ELEVATION_MAP_SIZE; j++) {
                u_char a, b;
                elevation_stream.read(reinterpret_cast<char *>(&a), 1);
                elevation_stream.read(reinterpret_cast<char *>(&b), 1);
                elevation_map[i][j] = uint16_t(a) * 256 + uint16_t(b);
//                std::cout << elevation_map[i][j] << " ";
            }
        }
        elevation_stream.close();
    }

//    GLuint load_elevation_texture(elevation_map_t &elevation_map) {
//        int width = elevation_map[0].size(), height = elevation_map.size();
//
//        std::vector<uint16_t> data(height * width);
//        for (int i = 0; i < height; i++) {
//            for (int j = 0; j < width; j++) {
//                data[i * width + j] = elevation_map[i][j] * 30;
//            }
//        }
//
//        GLuint result;
//        glGenTextures(1, &result);
//        glBindTexture(GL_TEXTURE_2D, result);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data.data());
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        return result;
//    }
}