cmake_minimum_required(VERSION 3.24)
project(Yenisey)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

if(APPLE)
    # brew version of glew doesn't provide GLEW_* variables
    get_target_property(GLEW_INCLUDE_DIRS GLEW::GLEW INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(GLEW_LIBRARIES GLEW::GLEW INTERFACE_LINK_LIBRARIES)
    get_target_property(GLEW_LIBRARY GLEW::GLEW LOCATION)
    list(APPEND GLEW_LIBRARIES "${GLEW_LIBRARY}")
endif()

set(TARGET_NAME "${PROJECT_NAME}")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(Yenisey
        src/main.cpp
        src/base/Scene.cpp
        src/base/Object.cpp
        src/base/Camera.cpp
        src/tools/tools.cpp
        src/base/Skybox.cpp
        src/tools/stb_image.c
        src/component/Component.cpp
        src/component/TransformComponent.cpp
        src/component/RenderComponent.cpp
        src/elevation/elevation.cpp
        src/elevation/ElevationDataObject.cpp include/component/RigibodyComponent.h src/component/RigibodyComponent.cpp src/water/WaterRenderComponent.cpp src/component/ColliderComponent.cpp src/component/MeshComponent.cpp src/elevation/ElevationMeshComponent.cpp src/primitive/SphereMeshComponent.cpp src/water/WaterMeshComponent.cpp src/base/Material.cpp src/base/LightSource.cpp src/base/Input.cpp src/component/ScriptComponent.cpp src/scripts/FreeCameraScriptComponent.cpp src/GLTF/gltf_loader.cpp src/GLTF/GLTFMeshComponent.cpp src/GLTF/GLTFRenderComponent.cpp src/elevation/ElevationColliderComponent.cpp src/scripts/PlayerScriptComponent.cpp src/ParticlesRenderComponent.cpp src/interface/msdf_loader.cpp src/interface/InterfaceData.cpp src/interface/InterfaceRenderComponent.cpp src/obj/ObjMeshComponent.cpp src/obj/obj_parser.cpp)

target_include_directories(${TARGET_NAME} PUBLIC
        include
        "${CMAKE_CURRENT_LIST_DIR}/rapidjson/include"
        "${SDL2_INCLUDE_DIRS}"
        "${GLEW_INCLUDE_DIRS}"
        "${OPENGL_INCLUDE_DIRS}"
        )
target_link_libraries(${TARGET_NAME} PUBLIC
        "${GLEW_LIBRARIES}"
        "${SDL2_LIBRARIES}"
        "${OPENGL_LIBRARIES}"
        )
target_compile_definitions(${TARGET_NAME} PUBLIC -DPROJECT_ROOT="${PROJECT_ROOT}")
