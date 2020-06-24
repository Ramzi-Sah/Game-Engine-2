#include "AnimationManager.hpp"

int ModelAnimationManager::getBoneIndex(std::string boneName) {
    for (unsigned int i = 0; i < bones.size(); i++) {
        if (bones[i].name == boneName)
            return bones[i].id;
    };

    // no bone with that name
    return -1;
};

void ModelAnimationManager::setAnimation(std::string animationName) {
    for (unsigned int i = 0; i < animations.size(); i++) {
        if (animations[i].name == animationName) {
            setAnimation(i);
            return;
        };
    };

    std::cout << "animation \"" << animationName << "\" does not exist." << '\n';
    setAnimation(-1);
};

void ModelAnimationManager::setAnimation(int _actualAnimationIndex) {
    actualAnimationIndex = _actualAnimationIndex;

    // check if no animation set
    if (actualAnimationIndex != -1)
        return;

    // reset animation time
    animations[actualAnimationIndex].time = 0.0f;
};

void ModelAnimationManager::update(float deltaTime) {
    // return if model has no animation
    // if (animations.size() == 0)
        // return;

    // tick actual animation
    if (actualAnimationIndex != -1)
        animations[actualAnimationIndex].tick(deltaTime);

    // transform bones by there parents
    transformBonesByParents(0, glm::mat4(1.0f));
};

void ModelAnimationManager::transformBonesByParents(int boneId, glm::mat4 parentTransforms) {

    glm::mat4 nodeTransformation = glm::mat4(1.0f);

    // apply user transform
    nodeTransformation = bones[boneId].nodeTransform;

    // check if animation select{ed
    if (actualAnimationIndex != -1) {
        // get keyframes
        int keyframeIndex = animations[actualAnimationIndex].getBoneKeyFramesIndex(bones[boneId].id);

        // bind keyframe transform with its bone
        if (keyframeIndex != -1)
            nodeTransformation = animations[actualAnimationIndex].keyFrames[keyframeIndex].transform;
    };

    // apply parent transforms
    bones[boneId].bindTransform = parentTransforms * nodeTransformation;

    bones[boneId].transform = globalInverseTransform * bones[boneId].bindTransform * bones[boneId].offsetMatrix;

    // recall function for all childrens
    for (int j = 0; j < bones[boneId].childrenBonesIds.size(); j++) {
        transformBonesByParents(bones[boneId].childrenBonesIds[j], bones[boneId].bindTransform);
    };
};

// -----------------------------------------------------------------------------
int Animation::getBoneKeyFramesIndex(int boneId) {
    for (unsigned int i = 0; i < keyFrames.size(); i++) {
        if (keyFrames[i].boneId == boneId)
            return i;
    };

    // no keyframes for this bone
    return -1;
};

void Animation::tick(float deltaTime) {
    // handle animation time
    time += deltaTime;
    if (time > duration) {
        time = 0.0f;

        // reset keydrames indecies
        for (int i = 0; i < keyFrames.size(); i++) {
            keyFrames[i].positionsIndex = 0;
            keyFrames[i].rotationsIndex = 0;
            keyFrames[i].scalesIndex = 0;
        };
    };

    // set bone's transforms
    for (int i = 0; i < keyFrames.size(); i++) {
        keyFrames[i].setBoneTransforms(time);
    };
};

// -----------------------------------------------------------------------------
void KeyFrames::setBoneTransforms(float time) {
    // get position index
    while (time > positions_time[positionsIndex]) {
        positionsIndex++;
        if (positionsIndex >= nbrPositions)
            positionsIndex = 0;
    };

    // get rotation index
    while (time > rotations_time[rotationsIndex]) {
        rotationsIndex++;
        if (rotationsIndex >= nbrRotations)
            rotationsIndex = 0;
    };

    transform = glm::translate(glm::mat4(1.0f), positions[positionsIndex]) * glm::mat4(rotations[rotationsIndex]);
};
