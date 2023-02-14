//
// Created by vladimir on 09.01.23.
//

#include "ParticlesRenderComponent.h"
#include "tools/tools.h"

const char particle_vertex_shader_source[] =
        R"(#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in float in_size;
layout (location = 2) in float in_rotation;

out float size;
out float rotation;

void main()
{
    gl_Position = vec4(in_position, 1.0);
    size = in_size;
    rotation = in_rotation;
}
)";

const char particle_geometry_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float size[];
in float rotation[];

out vec3 position;
out vec2 texcoord;

void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;

    vec3 Z = -normalize(camera_position - center);

    vec3 X = normalize(vec3(Z.y, -Z.x, 0));
    vec3 Y = normalize(cross(X, Z));

    vec3 X_r = X * cos(rotation[0]) - Y * sin(rotation[0]);
    vec3 Y_r = X * sin(rotation[0]) + Y * cos(rotation[0]);

    for (int i = 0; i < 4; i++) {
        vec3 pos = center + size[0] * X_r * (i / 2 == 0 ? -1 : 1) + size[0] * Y_r * (i % 2 == 0 ? -1 : 1);
        gl_Position = projection * view * model * vec4(pos, 1.0);
        position = (model * vec4(pos, 1.0)).xyz;
        texcoord = vec2((i / 2 == 0 ? 0 : 1), (i % 2 == 0 ? 0 : 1));
        EmitVertex();
    }
    EndPrimitive();

}

)";

namespace yny {



    ParticlesRenderComponent::ParticlesRenderComponent() {
        auto particle_vertex_shader = create_shader(GL_VERTEX_SHADER, particle_vertex_shader_source);
        auto particle_geometry_shader = create_shader(GL_GEOMETRY_SHADER, particle_geometry_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(particle_vertex_shader, particle_geometry_shader, fragment_shader);

        getUniformLocations(program);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)(sizeof(float) * 3));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)(sizeof(float) * (3 + 1 + 3)));
    }

    void ParticlesRenderComponent::render(Camera *camera, GLuint light_map) {
        if (particles.size() < PARTICLES_MAX_COUNT) {
            particle p(rng);
            particles.push_back(p);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDisable(GL_CULL_FACE);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(particle), particles.data(), GL_DYNAMIC_DRAW);

        glUseProgram(program);
        write_program_uniforms(camera, light_map);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, particles.size());
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }

    void ParticlesRenderComponent::light_render(Camera *camera) {}

    void ParticlesRenderComponent::light_render(Camera *camera, LightSource *lightSource) {}
} // yny