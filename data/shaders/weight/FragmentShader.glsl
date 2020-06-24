#version 330 core

out vec4 FragColor;

// from Vertex shader
in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;

// in ivec4 v_boneIDs;
in vec4 v_weights;
in float nbrOfActivatedBones;

void main() {
    FragColor = vec4(vec3(nbrOfActivatedBones), 1.0f);
};
