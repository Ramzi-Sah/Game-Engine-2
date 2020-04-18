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
uniform float u_near;
uniform float u_far;
uniform vec3 u_fogColor;
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near)) / u_far;
};

uniform float u_uvOffset;
uniform float u_colorOffset;
void main() {
    FragColor = texture(u_material.diffuseMap, v_uv + u_uvOffset);

    vec3 color = vec3(
        sin(u_colorOffset + v_uv.x + v_uv.y),
        sin(u_colorOffset + 3.141592 * 1/3 + v_uv.x + v_uv.y),
        sin(u_colorOffset + 3.141592 * 2/3 + v_uv.x + v_uv.y)
    );
    FragColor *= vec4(color, 1.0f);

    // mix with  fog
    FragColor = mix(FragColor, vec4(u_fogColor, 1.0f), LinearizeDepth(gl_FragCoord.z));
};
