#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_uv;

// for fragment shader
out vec2 v_uv;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    // calculate pos
    gl_Position = u_projection * u_view * u_model * vec4(in_position.x, in_position.y, in_position.z, 1.0);

    v_uv = in_uv;
};
