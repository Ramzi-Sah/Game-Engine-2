#ifndef SAH_MODEL_LOADER
#define SAH_MODEL_LOADER

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define SAH_NULL_STR "SAH_NULL_STR"
#define LOG_ERRORS false

#include "../Model.hpp"

struct VertexBoneData {
    int IDs[4] = {-1, -1, -1, -1};
    float Weights[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    void addBoneData(int BoneID, float Weight) {
        for (int i = 0; i < 4; i++) {
            if (IDs[i] == -1) {
                IDs[i] = BoneID;
                Weights[i] = Weight;
                return;
            };
        };

        // std::cout << "[ERROR] vertex affected by more than 4 bones." << '\n';
        return;
    };
};

class ModelLoader {
private:
    static std::map<std::string, Model*> m_models;

    static std::vector<VertexBoneData> m_modelBones;
    static void processNode(aiNode *node, const aiScene *scene, Model* &model);
    static void processMesh(aiMesh *mesh, aiMaterial* &material, Model* &model);
    static void processAnimation(const aiScene *scene, Model* &model);
    static void processBones(ModelAnimationManager &animationManager, aiNode *node);

public:
    ModelLoader() {};
    ~ModelLoader() {};

    static void createModel(std::string modelName, std::string modelPath);
    static Model* getModel(std::string modelName);
    static void deleteModel();
};

#endif
