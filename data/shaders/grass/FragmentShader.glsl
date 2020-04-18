#version 330 core

out vec4 FragColor;

// from Vertex shader
in vec3 v_pos;
in vec2 v_uv;

// Material
struct Material {
    vec3 ambient;

    vec3 diffuse;
    sampler2D diffuseMap;

    float shininess;
    vec3 specular;
    sampler2D specularMap;

    float opacity;
};

uniform Material u_material;

void main() {
    FragColor = texture(u_material.diffuseMap, v_uv);
};
