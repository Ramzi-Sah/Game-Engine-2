#version 330 core

out vec4 FragColor;

// from vertex shader
in vec4 v_color;

void main() {
    FragColor = v_color;
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
};
