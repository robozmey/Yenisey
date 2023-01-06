//
// Created by vladimir on 28.12.22.
//

#include <iostream>
#include "Scene.h"

namespace yny {
    void Scene::update_vertices() {

        this->Object::update_vertices(sceneCamera);

//        this->apply_transform();
    }

    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        skybox.render(sceneCamera);

        glClear(GL_DEPTH_BUFFER_BIT);

        std::vector<LightSource*> lightSources;

        LightSource ambientLightSource = LightSource();
//        lightSources.push_back(&ambientLightSource);

        LightSource directionalLightSource = LightSource();
        directionalLightSource.lightSourceType = DirectionalLight;
        directionalLightSource.direction = {0, 1, 1};
        lightSources.push_back(&directionalLightSource);

        for (auto lightSource : lightSources) {
            this->Object::render(sceneCamera, lightSource);
        }

    }

    void Scene::update_time() {
        this->Object::update_time(sceneCamera);
    }

    Scene::Scene() : Object("Scene") {
        scene = this;
    }

    Scene::Scene(std::string name) : Object(name) {
        scene = this;
    }
} // yny