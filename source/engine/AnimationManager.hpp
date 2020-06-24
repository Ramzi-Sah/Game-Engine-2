#ifndef SAH_ANIMATION_MANAGER
#define SAH_ANIMATION_MANAGER

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Line.hpp"

struct Bone {
    std::string name = "SAH_BONE_NO_NAME";
    int id = -1;
    glm::mat4 offsetMatrix = glm::mat4(1.0f);
    glm::mat4 nodeTransform = glm::mat4(1.0f);
    glm::mat4 bindTransform = glm::mat4(1.0f);
    glm::mat4 transform = glm::mat4(1.0f);

    std::vector<int> childrenBonesIds;

    Line* line = nullptr;

    Bone(std::string _name, unsigned int _id) {
        name = _name;
        id = _id;
    };
};

struct KeyFrames {
    int boneId = -1;

    std::vector<glm::vec3> positions;
    std::vector<float> positions_time;
    unsigned int nbrPositions;
    unsigned int positionsIndex = 0;

    std::vector<glm::quat> rotations;
    std::vector<float> rotations_time;
    unsigned int nbrRotations;
    unsigned int rotationsIndex = 0;

    std::vector<glm::vec3> scales;
    std::vector<float> scales_time;
    unsigned int nbrScales;
    unsigned int scalesIndex = 0;

    glm::mat4 transform = glm::mat4(1.0f);

    KeyFrames(int _boneId) {
        boneId = _boneId;
    };

    void setBoneTransforms(float time);
};

struct Animation {
    std::string name = "SAH_ANIMATION_NO_NAME";
    float duration = -1.0f;
    float time = 0.0f;

    std::vector<KeyFrames> keyFrames; // keyframe object for every bone

    Animation(std::string _name) {
        name = _name;
    };

    int getBoneKeyFramesIndex(int boneId);
    void tick(float deltaTime);
};

class ModelAnimationManager {
public:
    std::vector<Bone> bones;
    std::vector<std::vector<std::string>> hierarchy;
    int getBoneIndex(std::string boneName);

    std::vector<Animation> animations;
    int actualAnimationIndex = -1;
    glm::mat4 globalInverseTransform = glm::mat4(1.0f);

    void setAnimation(std::string animationName);
    void setAnimation(int _actualAnimationIndex);

    void update(float deltaTime);
    void transformBonesByParents(int boneId, glm::mat4 parentTransforms);

    bool renderSkeleton = false;
};

#endif
