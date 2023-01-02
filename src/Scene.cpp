//
// Created by vladimir on 28.12.22.
//

#include <iostream>
#include "Scene.h"

namespace yny {
    void Scene::update_vertices() {

        this->Object::update_vertices(player);

        this->apply_transform();
    }

    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        skybox.render(player);

        glClear(GL_DEPTH_BUFFER_BIT);

        this->Object::render(player);

    }

    void Scene::update_time() {
        this->Object::update_time(player);
    }

    Scene::Scene() {
        scene = this;
    }
} // yny