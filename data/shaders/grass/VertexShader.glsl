#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

// for fragment shader
out vec3 v_pos;
out vec2 v_uv;

// for fog
out float v_visibility;
uniform float u_fogMaxDist;
uniform float u_fogMinDist;
uniform vec3 u_viewPos;
float fog(vec3 fragmentPos) {
    // https://opengl-notes.readthedocs.io/en/latest/topics/texturing/aliasing.html#fog
    float fog_factor = (u_fogMaxDist - length(fragmentPos)) / (u_fogMaxDist - u_fogMinDist);
    return clamp(fog_factor, 0.0f, 1.0f);
};


void main() {
    // calculate pos
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);

    // for fragment shader
    v_pos = vec3(u_model * vec4(in_position, 1.0));
    v_uv = in_uv;
    v_visibility = fog(u_viewPos - v_pos);
};
