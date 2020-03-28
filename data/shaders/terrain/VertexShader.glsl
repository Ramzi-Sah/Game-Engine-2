#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_lightSpaceMatrix;

// for fragment shader
out vec3 v_pos;
out vec3 v_normal;
out vec2 v_uv;
out vec4 v_posLightSpace;

void main() {
    // calculate pos
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);

    // for fragment shader
    v_pos = vec3(u_model * vec4(in_position, 1.0));
    v_normal = normalize(vec3(u_model * vec4(in_normal, 0.0)));
    v_uv = in_uv;
    v_posLightSpace = u_lightSpaceMatrix * vec4(v_pos, 1.0);
};
