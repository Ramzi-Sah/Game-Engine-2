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

// for fog
uniform float u_fogMaxDist;
uniform float u_fogMinDist;
uniform vec3 u_fogColor;
uniform vec3 u_viewPos;
float LinearizeDepth(vec3 fragmentPos) {
    // https://opengl-notes.readthedocs.io/en/latest/topics/texturing/aliasing.html#fog
    float fog_factor = (u_fogMaxDist - length(fragmentPos)) / (u_fogMaxDist - u_fogMinDist);
    return clamp(fog_factor, 0.0f, 1.0f);
};

uniform float u_uvOffset;
void main() {
    FragColor = texture(u_material.diffuseMap, v_uv + u_uvOffset);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    vec3 color = vec3(
        sin(u_uvOffset + v_uv.x + v_uv.y),
        sin(u_uvOffset + 3.141592 * 1/3 + v_uv.x + v_uv.y),
        sin(u_uvOffset + 3.141592 * 2/3 + v_uv.x + v_uv.y)
    );
    FragColor *= vec4(color, 1.0f);

    // mix with  fog
    FragColor = mix(vec4(u_fogColor, 1.0f), FragColor, LinearizeDepth(u_viewPos - v_pos));
};
