#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float opacity;
};

uniform Material u_material;

// light
uniform vec3 ambientLight;
uniform vec3 diffuseLightDir;
uniform vec3 diffuseLightColor;
uniform vec3 u_viewPos;

// color out
out vec4 v_color;

void main() {
    // calculate pos
    gl_Position = u_projection * u_view * u_model * vec4(in_position.x, in_position.y, in_position.z, 1.0);

    // for fragment shader
    vec3 pos = vec3(u_model * vec4(in_position, 1.0));
    vec3 normal = normalize(vec3(u_model * vec4(in_normal, 0.0)));

    // calculate ambient light
    vec3 ambient = ambientLight * u_material.ambient;

    // calculate defuse light
    float diffuse_diff = max(dot(normal, diffuseLightDir), 0.0f);
    vec3 diffuse = diffuse_diff * diffuseLightColor * u_material.diffuse;

    // calculate specular light
    vec3 viewDir = normalize(u_viewPos - pos);
    vec3 reflectDir = reflect(diffuseLightDir, normal);
    float spec = pow(max(dot(viewDir, -reflectDir), 0.0f), u_material.shininess);
    vec3 specular = spec * diffuseLightColor * u_material.specular;

    v_color = vec4(ambient + diffuse + specular, u_material.opacity);
};
