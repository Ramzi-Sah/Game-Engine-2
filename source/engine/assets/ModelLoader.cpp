#include "ModelLoader.hpp"

std::map<std::string, Model*> ModelLoader::m_models;
std::vector<VertexBoneData> ModelLoader::m_modelBones;

// /*
void ModelLoader::createModel(std::string modelName, std::string modelPath) {
    std::string objFile = "data/models/" + modelPath;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objFile.c_str(), aiProcess_Triangulate); // aiProcess_FlipUVs

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "[ERROR] can't import model, " << importer.GetErrorString() << std::endl;
        return;
    };

    // create model
    Model* model = new Model();

    // std::cout << "---------------------- " << modelName << " ----------------------" << '\n';

    // get model data
    processNode(scene->mRootNode, scene, model);

    // get animation data
    if (scene->mNumAnimations > 0)
        processAnimation(scene, model);

    // std::cout << "---------------------------------------------------" << '\n';
    // std::cout << "nbr bones: " << model->animationManager.bones.size() << '\n';
    // for (int i = 0; i < model->animationManager.bones.size(); i++){
    //     std::cout << model->animationManager.bones[i].id << " " << model->animationManager.bones[i].name << '\n';
    //
    //     for (int j = 0; j < model->animationManager.bones[i].childrenBonesIds.size(); j++) {
    //         std::cout << "   - " <<  model->animationManager.bones[model->animationManager.bones[i].childrenBonesIds[j]].name << '\n';
    //     }
    // };

    // if (model->animationManager.animations.size() > 0)
    //     std::cout << "nbr position keyFrames for bone " << model->animationManager.animations[0].keyFrames[0].boneId << ": " << model->animationManager.animations[0].keyFrames[0].positions.size() << '\n';

    // add it to models map
    m_models[modelName] = model;
};

void ModelLoader::processNode(aiNode *node, const aiScene *scene, Model* &model) {
    // process all the node's meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        processMesh(mesh, material, model);
    };

    // store hieharchy
    std::vector<std::string> hierarchy;
    hierarchy.push_back(node->mName.data);

    // do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, model);

        // add child to node hieharchy
        hierarchy.push_back(node->mChildren[i]->mName.data);
    };
    model->animationManager.hierarchy.push_back(hierarchy);

};
void ModelLoader::processMesh(aiMesh *mesh, aiMaterial* &material, Model* &model) {
    // std::cout << "------------------ mesh group ------------------" << '\n';
    m_modelBones.clear();
    m_modelBones.resize(mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumBones; i++) {
        // check if bone alredy added to animation model
        bool boneExists = false;
        unsigned int boneId = -1;
        for (int j = 0; j < model->animationManager.bones.size(); j++){
            if (model->animationManager.bones[j].name == mesh->mBones[i]->mName.data) {
                boneExists = true;
                boneId = model->animationManager.bones[j].id;
                break;
            };
        };

        if (!boneExists) {
            // create bone if does not exist
            boneId = model->animationManager.bones.size();
            model->animationManager.bones.push_back(
                Bone(mesh->mBones[i]->mName.data, boneId)
            );

            // set bone offset matrix
            aiMatrix4x4 aiOffsetMatrix = mesh->mBones[i]->mOffsetMatrix;
            glm::mat4 offsetMatrix = glm::mat4(1.0f);

            offsetMatrix[0][0] = (GLfloat)aiOffsetMatrix.a1; offsetMatrix[0][1] = (GLfloat)aiOffsetMatrix.b1;  offsetMatrix[0][2] = (GLfloat)aiOffsetMatrix.c1; offsetMatrix[0][3] = (GLfloat)aiOffsetMatrix.d1;
            offsetMatrix[1][0] = (GLfloat)aiOffsetMatrix.a2; offsetMatrix[1][1] = (GLfloat)aiOffsetMatrix.b2;  offsetMatrix[1][2] = (GLfloat)aiOffsetMatrix.c2; offsetMatrix[1][3] = (GLfloat)aiOffsetMatrix.d2;
            offsetMatrix[2][0] = (GLfloat)aiOffsetMatrix.a3; offsetMatrix[2][1] = (GLfloat)aiOffsetMatrix.b3;  offsetMatrix[2][2] = (GLfloat)aiOffsetMatrix.c3; offsetMatrix[2][3] = (GLfloat)aiOffsetMatrix.d3;
            offsetMatrix[3][0] = (GLfloat)aiOffsetMatrix.a4; offsetMatrix[3][1] = (GLfloat)aiOffsetMatrix.b4;  offsetMatrix[3][2] = (GLfloat)aiOffsetMatrix.c4; offsetMatrix[3][3] = (GLfloat)aiOffsetMatrix.d4;

            model->animationManager.bones[boneId].offsetMatrix = offsetMatrix;
        };

        for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
            int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            m_modelBones[vertexID].addBoneData(boneId, weight * 1000);
        };
    };

    // process vertex positions, normals and texture coordinates
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // check if the mesh contain uv coordinate
        glm::vec2 uv = glm::vec2(0.0f, 0.0f);
        if(mesh->mTextureCoords[0])
            uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        model->addVertex(
    		glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
    		glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
    		uv,
            glm::vec4(m_modelBones[i].IDs[0], m_modelBones[i].IDs[1], m_modelBones[i].IDs[2], m_modelBones[i].IDs[3]),
            glm::vec4(m_modelBones[i].Weights[0], m_modelBones[i].Weights[1], m_modelBones[i].Weights[2], m_modelBones[i].Weights[3])
        );
    };

    // process indices
    unsigned int nbrIndecies = mesh->mNumFaces * 3;
    unsigned int indecies[nbrIndecies];
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < 3; j++) {
            indecies[i * 3 + j] = face.mIndices[j];
        };
    };

    // process material
    Material mat = Material();
    if(mesh->mMaterialIndex >= 0) {
        // http://assimp.sourceforge.net/lib_html/materials.html
        std::string name;
        material->Get(AI_MATKEY_NAME, name);
        mat.name = "material"; // FIXME


        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        mat.ambient = glm::vec3(color.r, color.g, color.b);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        mat.diffuse = glm::vec3(color.r, color.g, color.b);

        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        mat.specular = glm::vec3(color.r, color.g, color.b);


        float shininess, opacity;
        material->Get(AI_MATKEY_SHININESS, shininess);
        // mat.shininess = shininess;

        material->Get(AI_MATKEY_OPACITY, opacity);
        // mat.opacity = opacity;
    };

    model->loadVertecies(indecies, nbrIndecies, mat);
};

//------------------------------------------------------------------------------
void ModelLoader::processBones(ModelAnimationManager &animationManager, aiNode *node) {

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        // get bone nodeTransform
        int boneIndex = animationManager.getBoneIndex(node->mChildren[i]->mName.data);
        if (boneIndex != -1) {
            aiMatrix4x4 aiNodeTransformation = node->mChildren[i]->mTransformation;
            glm::mat4 nodeTransform = glm::mat4(1.0f);

            nodeTransform[0][0] = (GLfloat)aiNodeTransformation.a1; nodeTransform[0][1] = (GLfloat)aiNodeTransformation.b1;  nodeTransform[0][2] = (GLfloat)aiNodeTransformation.c1; nodeTransform[0][3] = (GLfloat)aiNodeTransformation.d1;
            nodeTransform[1][0] = (GLfloat)aiNodeTransformation.a2; nodeTransform[1][1] = (GLfloat)aiNodeTransformation.b2;  nodeTransform[1][2] = (GLfloat)aiNodeTransformation.c2; nodeTransform[1][3] = (GLfloat)aiNodeTransformation.d2;
            nodeTransform[2][0] = (GLfloat)aiNodeTransformation.a3; nodeTransform[2][1] = (GLfloat)aiNodeTransformation.b3;  nodeTransform[2][2] = (GLfloat)aiNodeTransformation.c3; nodeTransform[2][3] = (GLfloat)aiNodeTransformation.d3;
            nodeTransform[3][0] = (GLfloat)aiNodeTransformation.a4; nodeTransform[3][1] = (GLfloat)aiNodeTransformation.b4;  nodeTransform[3][2] = (GLfloat)aiNodeTransformation.c4; nodeTransform[3][3] = (GLfloat)aiNodeTransformation.d4;

            animationManager.bones[boneIndex].nodeTransform = nodeTransform;
        };

        // recall function for all childrens
        processBones(animationManager, node->mChildren[i]);
    };
};

void ModelLoader::processAnimation(const aiScene *scene, Model* &model) {
    // correct animation
    aiMatrix4x4 aiGlobalInverseTransform = scene->mRootNode->mTransformation.Inverse();
    glm::mat4 globalInverseTransform = glm::mat4(1.0f);

    globalInverseTransform[0][0] = (GLfloat)aiGlobalInverseTransform.a1; globalInverseTransform[0][1] = (GLfloat)aiGlobalInverseTransform.b1;  globalInverseTransform[0][2] = (GLfloat)aiGlobalInverseTransform.c1; globalInverseTransform[0][3] = (GLfloat)aiGlobalInverseTransform.d1;
    globalInverseTransform[1][0] = (GLfloat)aiGlobalInverseTransform.a2; globalInverseTransform[1][1] = (GLfloat)aiGlobalInverseTransform.b2;  globalInverseTransform[1][2] = (GLfloat)aiGlobalInverseTransform.c2; globalInverseTransform[1][3] = (GLfloat)aiGlobalInverseTransform.d2;
    globalInverseTransform[2][0] = (GLfloat)aiGlobalInverseTransform.a3; globalInverseTransform[2][1] = (GLfloat)aiGlobalInverseTransform.b3;  globalInverseTransform[2][2] = (GLfloat)aiGlobalInverseTransform.c3; globalInverseTransform[2][3] = (GLfloat)aiGlobalInverseTransform.d3;
    globalInverseTransform[3][0] = (GLfloat)aiGlobalInverseTransform.a4; globalInverseTransform[3][1] = (GLfloat)aiGlobalInverseTransform.b4;  globalInverseTransform[3][2] = (GLfloat)aiGlobalInverseTransform.c4; globalInverseTransform[3][3] = (GLfloat)aiGlobalInverseTransform.d4;

    // apply correction
    globalInverseTransform = glm::rotate(globalInverseTransform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // set global correction matrix
    model->animationManager.globalInverseTransform = globalInverseTransform;


    // get bones heirarchy
    for (int i = 0; i < model->animationManager.hierarchy.size(); i++) {
        // return if not a bone
        int boneIndex = model->animationManager.getBoneIndex(model->animationManager.hierarchy[i][0]);
        if (boneIndex == -1)
            continue;

        for (int j = 1; j < model->animationManager.hierarchy[i].size(); j++){
            model->animationManager.bones[boneIndex].childrenBonesIds.push_back(model->animationManager.getBoneIndex(model->animationManager.hierarchy[i][j]));
        };
    };

    // handle nodeTransform of bones and there heiharchy
    processBones(model->animationManager, scene->mRootNode);

    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        // init animation
        Animation animation = Animation(scene->mAnimations[i]->mName.data);
        animation.duration = scene->mAnimations[i]->mDuration * scene->mAnimations[i]->mTicksPerSecond;

        for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
            // get bone id
            std::string boneName = scene->mAnimations[i]->mChannels[j]->mNodeName.data;
            int boneId = -1;
            for (int i = 0; i < model->animationManager.bones.size(); i++) {
                if (model->animationManager.bones[i].name == boneName) {
                    boneId = model->animationManager.bones[i].id;
                    break;
                };
            };
            if (boneId == -1) std::cout << "WARNING: no bone " << scene->mAnimations[i]->mChannels[j]->mNodeName.data << '\n';

            animation.keyFrames.push_back(KeyFrames(boneId));

            // get key frames data
            animation.keyFrames[animation.keyFrames.size() - 1].nbrPositions = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
            for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
                float time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;

                aiVector3D ai_position = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
                glm::vec3 position = glm::vec3(ai_position.x, ai_position.y, ai_position.z);

                animation.keyFrames[animation.keyFrames.size() - 1].positions.push_back(position);
                animation.keyFrames[animation.keyFrames.size() - 1].positions_time.push_back(time);
            };

            animation.keyFrames[animation.keyFrames.size() - 1].nbrRotations = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
            for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
                float time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;

                aiQuaternion ai_rotation= scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
                glm::quat rotation = glm::quat(ai_rotation.w, ai_rotation.x, ai_rotation.y, ai_rotation.z);

                animation.keyFrames[animation.keyFrames.size() - 1].rotations.push_back(rotation);
                animation.keyFrames[animation.keyFrames.size() - 1].rotations_time.push_back(time);
            };

            animation.keyFrames[animation.keyFrames.size() - 1].nbrScales = scene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
            for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
                float time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;

                aiVector3D ai_scale = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
                glm::vec3 scale = glm::vec3(ai_scale.x, ai_scale.y, ai_scale.z);

                animation.keyFrames[animation.keyFrames.size() - 1].scales.push_back(scale);
                animation.keyFrames[animation.keyFrames.size() - 1].scales_time.push_back(time);
            };

        };

        // push back to model animations
        model->animationManager.animations.push_back(animation);
    };
};

// -----------------------------------------------------------------------------
Model* ModelLoader::getModel(std::string modelName) {
    return m_models[modelName];
};

void ModelLoader::deleteModel() {
    // no need to delete any model for now
};
