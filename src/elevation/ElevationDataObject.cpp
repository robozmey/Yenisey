//
// Created by vladimir on 02.01.23.
//

#include "elevation/ElevationDataObject.h"

namespace yny {

    void ElevationDataObject::upload_lod_elevation_map(int latitude, int longitude) {
        lod_elevation_map_t& lod_elevation_map = lod_elevation_maps[{latitude, longitude}];
        lod_elevation_map.resize(lod_count);
        upload_elevation_map(latitude, longitude, lod_elevation_map[0]);

        for (int lod = 1; lod < lod_count; lod++) {
            int pr_height = lod_elevation_map[lod-1].size();
            int pr_width = lod_elevation_map[lod-1][0].size();
            int height = (pr_height + 1) / 2;
            int width = (pr_width + 1) / 2;

            lod_elevation_map[lod].assign(height, std::vector<uint16_t>(width));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int val = 0;
                    int count = 0;
                    for (int i1 = 0; i1 < 2; i1++) {
                        for (int j1 = 0; j1 < 2; j1++) {
                            if (i*2 + i1 < pr_height && j*2 + j1 < pr_width) {
                                val += lod_elevation_map[lod-1][i*2 + i1][j*2 + j1];
                                count++;
                            }
                        }
                    }
                    lod_elevation_map[lod][i][j] = val / count;
                }
            }
        }
    }

    float ElevationDataObject::get_data(int latitude, int longitude, int lod, int i, int j) {
        int lat_size = ELEVATION_MAP_SIZE / (1 << lod);

        while (lat_size <= i) {
            i -= lat_size;
            latitude--;
        }
        while (i < 0) {
            i += lat_size;
            latitude++;
        }

        while (lat_size <= j) {
            j -= lat_size;
            longitude++;
        }
        while (j < 0) {
            j += lat_size;
            longitude--;
        }

        if (!lod_elevation_maps.contains({latitude, longitude})) {
            upload_lod_elevation_map(latitude, longitude);
        }

        elevation_map_t& em = lod_elevation_maps[{latitude, longitude}][lod];

        if (lod_elevation_maps[{latitude, longitude}].empty())
            return 0;

        return em[i][j];
    }
} // yny