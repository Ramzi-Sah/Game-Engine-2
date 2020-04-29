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

// fog
uniform vec3 u_fogColor;
in float v_visibility;

void main() {
    FragColor = texture(u_material.diffuseMap, v_uv);

    // mix with  fog
    FragColor = mix(vec4(u_fogColor, 1.0f), FragColor, v_visibility);
};
