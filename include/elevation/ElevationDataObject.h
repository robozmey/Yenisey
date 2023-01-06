//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_ELEVATIONDATAOBJECT_H
#define YENISEY_ELEVATIONDATAOBJECT_H

#include "elevation/elevation.h"
#include <map>

namespace yny {

    class ElevationDataObject {
    public:
        int lod_count = 3;

        int latitude = 55, longitude = 92;
        int latitude_minute = 29, longitude_minute = 30;

        float get_data(int latitude, int longitude, int lod, int i, int j);

    private:
        std::map<std::pair<int, int>, lod_elevation_map_t> lod_elevation_maps;

        void upload_lod_elevation_map(int latitude, int longitude);
    };

} // yny

#endif //YENISEY_ELEVATIONDATAOBJECT_H
