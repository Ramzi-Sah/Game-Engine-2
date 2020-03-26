#version 330 core

out vec4 FragColor;

in vec2 v_uv;

void main() {
    FragColor = vec4(vec2(v_uv.x, v_uv.y), 0.0f, 1.0);
};
