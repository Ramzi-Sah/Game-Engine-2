#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

// for fragment shader
out vec3 v_normal;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    // calculate pos
    gl_Position = u_projection * u_view * u_model * vec4(in_position.x, in_position.y, in_position.z, 1.0);

    v_normal = normalize(vec3(u_model * vec4(in_normal, 0.0)));
};
