//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_ELEVATIONMESHCOMPONENT_H
#define YENISEY_ELEVATIONMESHCOMPONENT_H

#include "component/MeshComponent.h"
#include "ElevationDataObject.h"

namespace yny {

    class ElevationMeshComponent : public MeshComponent {
    public:
        int lod_count = 3;

        ElevationDataObject* elevationDataObject;
        int latitude_minute = 0, longitude_minute = 0;

        void update_vertices(Camera* scene_player) override;

        ElevationMeshComponent();
        ElevationMeshComponent(ElevationDataObject* elevationDataObject);

    private:
        void recalc_terrain(Camera* scene_player);
    };

} // yny

#endif //YENISEY_ELEVATIONMESHCOMPONENT_H
