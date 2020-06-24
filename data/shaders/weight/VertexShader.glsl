#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

// for transformation
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_lightSpaceMatrix;

// for fragment shader
out vec3 v_pos;
out vec3 v_normal;
out vec2 v_uv;
// out ivec4 v_boneIDs;
out vec4 v_weights;
out float nbrOfActivatedBones;

// for animation
const int MAX_BONES = 100;
uniform mat4 u_BonesTransform[MAX_BONES];

void main() {
    // clculate animation transform matrix
    mat4 boneTransform = mat4(1.0f);
    nbrOfActivatedBones = 0.0f;
    for (int i = 0; i < 4; i++) {
        // if (BoneIDs[i] != -1) boneTransform += u_BonesTransform[BoneIDs[i]] * Weights[i];

        if (BoneIDs[i] != -1) nbrOfActivatedBones += 1.0f / 4;
    }

    mat4 modelTransform = u_model * boneTransform;

    // calculate pos
    gl_Position = u_projection * u_view * modelTransform * vec4(in_position, 1.0);

    // for fragment shader
    v_pos = vec3(u_model * vec4(in_position, 1.0));
    v_normal = normalize(vec3(modelTransform * vec4(in_normal, 0.0)));
    v_uv = in_uv;

    // v_boneIDs = BoneIDs;
    v_weights = Weights;
};
