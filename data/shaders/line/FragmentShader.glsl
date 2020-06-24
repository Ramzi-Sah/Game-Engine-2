#version 330 core

out vec4 FragColor;
uniform vec3 u_color;

void main() {
    FragColor = vec4(u_color, 1.0);
    // FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0);
};
