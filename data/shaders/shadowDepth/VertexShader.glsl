#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

// for transformation
uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_view;
uniform mat4 u_model;

// for animation
const int MAX_BONES = 100;
uniform mat4 u_BonesTransform[MAX_BONES];

void main() {
    // clculate animation transform matrix
    mat4 boneTransform = mat4(1.0f);
    for (int i = 0; i < 4; i++) {
        // if (BoneIDs[i] != -1) boneTransform += u_BonesTransform[BoneIDs[i]] * Weights[i];
    }

    // calculate pos
    gl_Position = u_lightSpaceMatrix * u_model * boneTransform * vec4(in_position, 1.0);
};
