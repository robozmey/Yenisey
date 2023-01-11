//
// Created by vladimir on 29.12.22.
//

#include <string>
#include "base/Skybox.h"
#include "tools/tools.h"

const char vertex_shader_source[] =
        R"(#version 330 core

const vec2 VERTICES[4] = vec2[4](
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, -1.0)
);

uniform mat4 view;
uniform mat4 projection;

out vec3 position;

void main()
{
    vec2 vertex = VERTICES[gl_VertexID];
    mat4 view_projection_inverse = inverse(projection * view);
    vec4 ndc = vec4(vertex, 0.0, 1.0);
    vec4 clip_space = view_projection_inverse * ndc;
    position = clip_space.xyz / clip_space.w;
    gl_Position = vec4(vertex, 0.0, 1.0);
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform vec3 camera_position;

uniform sampler2D environment_texture;

in vec3 position;

layout (location = 0) out vec4 out_color;

const float PI = 3.141592653589793;

uniform mat4 projection;

void main()
{
    vec3 camera_direction = -normalize(camera_position - position);

    vec3 dir = camera_direction;
    float x = atan(dir.z, dir.x) / PI * 0.5 + 0.5;
    float y = -atan(dir.y, length(dir.xz)) / PI + 0.5;

    vec3 environment_albedo = texture(environment_texture, vec2(x, y)).rgb;

    out_color = vec4(environment_albedo, 1);
}
)";

namespace yny {
    void Skybox::render(Camera* camera) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, environment_texture);

        glUseProgram(program);
        glm::mat4 view = camera->get_camera_view();
        glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&view));
        glm::mat4 projection = camera->get_camera_projection();
        glUniformMatrix4fv(projection_location, 1, GL_FALSE,
                           reinterpret_cast<float *>(&projection));
        glm::vec3 camera_position = camera->get_camera_position();
        glUniform3fv(camera_position_location, 1, reinterpret_cast<float *>(&camera_position));
        glUniform1i(environment_texture_location, 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glClear(GL_DEPTH_BUFFER_BIT);
    }

    Skybox::Skybox() {
        glGenVertexArrays(1, &vao);

        std::string project_root = PROJECT_ROOT;
        environment_texture = load_texture(project_root + "/texture/qwantani_puresky.jpg");

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");
        camera_position_location = glGetUniformLocation(program, "camera_position");
        environment_texture_location = glGetUniformLocation(program, "environment_texture");
    }
} // yny