#version 330 core

layout(location = 0) in vec3 in_position;

// for transformation
uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    // calculate pos
    gl_Position = u_lightSpaceMatrix * u_model * vec4(in_position, 1.0);
};
