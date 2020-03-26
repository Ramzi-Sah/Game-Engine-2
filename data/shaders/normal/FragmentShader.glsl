#version 330 core

out vec4 FragColor;

in vec3 v_normal;

void main() {
    FragColor = vec4(v_normal * 0.5 + 0.5, 1.0f);
};
