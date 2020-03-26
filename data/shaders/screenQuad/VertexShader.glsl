#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_uv;

// for fragment shader
out vec2 v_uv;

void main() {
    // calculate pos
    gl_Position = vec4(in_position.x/2-0.75f, in_position.y/2-0.5f, 0.0f, 1.0);
    // gl_Position = vec4(in_position.x, in_position.y, 0.0f, 1.0);

    v_uv = in_uv;
};
