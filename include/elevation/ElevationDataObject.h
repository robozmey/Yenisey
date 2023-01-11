//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_ELEVATIONDATAOBJECT_H
#define YENISEY_ELEVATIONDATAOBJECT_H

#include "elevation/elevation.h"
#include <map>

namespace yny {

    float wgs_distance(float lat1, float lon1, float lat2, float lon2);

    class ElevationDataObject {
    public:
        int lod_count = 5;

        int latitude = 55, longitude = 92;
        int latitude_minute = 29, longitude_minute = 30;

        float get_data(int latitude, int longitude, int lod, int i, int j);

        void move(int add_latitude, int add_longitude, int add_latitude_minute, int add_longitude_minute);

    private:
        std::map<std::pair<int, int>, lod_elevation_map_t> lod_elevation_maps;

        void upload_lod_elevation_map(int latitude, int longitude);
    };

} // yny

#endif //YENISEY_ELEVATIONDATAOBJECT_H
