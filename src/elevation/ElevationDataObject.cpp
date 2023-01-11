//
// Created by vladimir on 02.01.23.
//

#include "elevation/ElevationDataObject.h"

namespace yny {

    float wgs_distance(float lat1, float lon1, float lat2, float lon2) {
        double p = 0.017453292519943295;    // Math.PI / 180
        double a = 0.5 - cos((lat2 - lat1) * p)/2 +
                   cos(lat1 * p) * cos(lat2 * p) *
                   (1 - cos((lon2 - lon1) * p))/2;

        return 12742 * asin(sqrt(a)); // 2 * R; R = 6371 km
    }

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

    void
    ElevationDataObject::move(int add_latitude, int add_longitude, int add_latitude_minute, int add_longitude_minute) {
        latitude += add_latitude;
        longitude += add_longitude;
        latitude_minute += add_latitude_minute;
        longitude_minute += add_longitude_minute;

        while(latitude_minute >= 60) {
            latitude_minute -= 60;
            latitude++;
        }
        while(latitude_minute < 0) {
            latitude_minute += 60;
            latitude--;
        }

        while(longitude_minute >= 60) {
            longitude_minute -= 60;
            longitude++;
        }
        while(longitude_minute < 0) {
            longitude_minute += 60;
            longitude--;
        }
    }
} // yny