#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <map>
#include <chrono>

#include "tools.h"

#include "Scene.h"
#include "Skybox.h"
#include "component/TransformComponent.h"
#include "component/RigibodyComponent.h"
#include "WaterRenderComponent.h"
#include "elevation/ElevationMeshComponent.h"
#include "WaterMeshComponent.h"
#include "primitive/SphereMeshComponent.h"
#include "PlayerScriptComponent.h"
#include "GLTF/GLTFMeshComponent.h"
#include "GLTF/GLTFRenderComponent.h"
#include "component/ColliderComponent.h"
#include "elevation/ElevationColliderComponent.h"
#include "ParticlesRenderComponent.h"
#include "interface/InterfaceRenderComponent.h"


int main() {
    std::string project_root = PROJECT_ROOT;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        sdl2_fail("SDL_Init: ");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window * window = SDL_CreateWindow("Yenisey",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           800, 600,
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    if (!window)
        sdl2_fail("SDL_CreateWindow: ");

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context)
        sdl2_fail("SDL_GL_CreateContext: ");

    if (auto result = glewInit(); result != GLEW_NO_ERROR)
        glew_fail("glewInit: ", result);

    if (!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 is not supported");

    glClearColor(0.8f, 0.8f, 1.f, 0.f);

    auto last_frame_start = std::chrono::high_resolution_clock::now();

    float time = 0.f;

    std::map<SDL_Keycode, bool> button_down;

    yny::ElevationDataObject elevationDataObject;

    yny::Material snowMaterial(project_root + "/texture/Stylized_Stone_Floor_005_basecolor.jpg");
    snowMaterial.add_normal_map(project_root + "/texture/Stylized_Stone_Floor_005_normal.jpg");

    yny::Material waterMaterial(glm::vec3({0, 0, 0.6}));

    yny::Scene scene;
    scene.skybox = yny::Skybox();

    yny::InterfaceData interfaceData;

    yny::Object interfaceObject("Interface");
    interfaceObject.add_component(yny::Render, new yny::InterfaceRenderComponent(&interfaceData));
    scene.add_object(&interfaceObject);

    yny::Object particlesObject("Particles");
    particlesObject.add_component(yny::Render, new yny::ParticlesRenderComponent);

    yny::Object playerObject("Player");
//    playerObject.add_component(yny::Rigibody);
    playerObject.add_component(yny::Script, new yny::PlayerScriptComponent(&interfaceData, &elevationDataObject));
//    playerObject.add_component(yny::Collider, new yny::SphereColliderComponent);
    yny::Camera playerCamera("Camera");
    playerObject.add_object(reinterpret_cast<yny::Object *>(&playerCamera));
    scene.add_object(&playerObject);
    reinterpret_cast<yny::TransformComponent *>(playerObject.components[yny::Transform])->move({0, 600, 0});
    scene.sceneCamera = &playerCamera;

    yny::Object snowmobile("Snowmobile");
    add_gltf_model(snowmobile, project_root + "/model/Macarena/Macarena.gltf");
    scene.add_object(&snowmobile);

    yny::Object sphere("Sphere");
//    sphere.add_component(yny::Rigibody);
    sphere.add_component(yny::Mesh, new yny::SphereMeshComponent());
    sphere.add_component(yny::Render);
    reinterpret_cast<yny::TransformComponent *>(sphere.components[yny::Transform])->move({0, 0, -1000});
    playerObject.add_object(&sphere);


    yny::Object bigSphere("Big Sphere");
//    bigSphere.add_component(yny::Rigibody);
    bigSphere.add_component(yny::Mesh, new yny::SphereMeshComponent(1000));
    bigSphere.add_component(yny::Render);
    reinterpret_cast<yny::TransformComponent *>(bigSphere.components[yny::Transform])->move({-2000, 0, -10000});
    scene.add_object(&bigSphere);


    yny::Object elevation_object("Terrain");
    elevation_object.add_component(yny::Mesh, new yny::ElevationMeshComponent(&elevationDataObject));
    elevation_object.add_component(yny::Render, new yny::RenderComponent(&snowMaterial));
//    elevation_object.add_component(yny::Collider, new yny::ElevationColliderComponent(&elevationDataObject));
    scene.add_object(&elevation_object);

    yny::Object waterObject("Water");
//    waterObject.add_component(yny::Rigibody);
    waterObject.add_component(yny::Mesh, new yny::WaterMeshComponent());
//    waterObject.add_component(yny::Render, new yny::RenderComponent(&waterMaterial));
//    scene.add_object(&waterObject);

    bool running = true;
    while (running)
    {
        for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT: switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            width = event.window.data1;
                            height = event.window.data2;
                            glViewport(0, 0, width, height);
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    button_down[event.key.keysym.sym] = true;
                    break;
                case SDL_KEYUP:
                    button_down[event.key.keysym.sym] = false;
                    break;
            }

        if (!running)
            break;

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - last_frame_start).count();
        last_frame_start = now;
        time += dt;

        scene.time = time;
        scene.dt = dt;

        scene.input.button_down = button_down;

        scene.sceneCamera->update_screen(width, height);

        scene.update();
        scene.update_vertices();

        scene.render();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);

    return 0;
}
