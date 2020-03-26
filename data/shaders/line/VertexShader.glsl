#version 330 core

layout(location = 0) in vec3 in_position;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
    // calculate pos
    gl_Position = u_projection * u_view * vec4(in_position, 1.0);
};
