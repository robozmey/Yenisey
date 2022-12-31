//
// Created by vladimir on 28.12.22.
//

#include <iostream>
#include "Scene.h"

namespace yny {
    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        skybox.render(player);

        glClear(GL_DEPTH_BUFFER_BIT);

        for (Object* object : objects) {
            object->render(player);
        }

        glClear(GL_DEPTH_BUFFER_BIT);

        for (Object* ui_element : ui) {
            ui_element->render(player);
        }
    }
} // yny