#ifndef SAH_MODEL
#define SAH_MODEL

#include <vector>
#include <sstream>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assets/ShaderLoader.hpp"
#include "assets/TextureLoader.hpp"
#include "../common/Vertex.hpp"
#include "../common/Material.hpp"
#include "AnimationManager.hpp"

struct MeshGroup {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int numberVertecies;
    unsigned int numberIndecies;

    Material material;

    bool copy = false;
    MeshGroup() {};
    ~MeshGroup() {
        // dont delete original model
        if (copy)
            return;

        // flush buffers
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    };
};

class Model {
private:
    // for render
    void setShaderProgram(unsigned int _shaderProgram);
    unsigned int shaderProgram = ShaderLoader::getShader("Default");
    unsigned int mainShaderProgram = ShaderLoader::getShader("Default");
    unsigned int polygoneMode = GL_FILL; // GL_POINT, GL_LINE, GL_FILL
    unsigned int modelSize = 0;
    void draw();

    // for transforms
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    void setModelMatrixUniform();

    glm::mat4 position = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);

    // material
    void setMaterialUniform(Material _material);

    // for animation
    void setAnimationBonesTransformUniform();
    void setAnimationBonesTransformShadowUniform();

public:
    Model();
    Model(Model* _model);
    ~Model();

    std::vector<Vertex> vertecies;
    void addVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv);
    void addVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv, glm::ivec4 _BoneIDs, glm::vec4 _Weights);
    void loadVertecies(unsigned int _indecies[], unsigned int _numberIndecies, Material _material);

    void render();
    void renderShadow();
    void renderFrame(unsigned int texture);
    bool castShadow = true;
    bool faceCulled = true;
    bool hoovered = false;
    bool selected = false;

    // for render
    void setMainShaderProgram(unsigned int _shaderProgram);
    unsigned int getMainShaderProgram();
    void setPolygoneMode(unsigned int _polygoneMode);
    unsigned int getPolygoneMode();
    unsigned int getModelSize();

    // for transfomation
    void setPos(glm::vec3 _position);
    void setRot(float _angle, glm::vec3 _axes);
    void setRot(glm::quat _rotation);
    void setScale(glm::vec3 _scale);
    void updateTransform();

    std::vector<MeshGroup*> meshGroups;

    // for animation
    ModelAnimationManager animationManager;
    void updateAnimation(float deltaTime);
    void drawSkeleton(int boneId, glm::vec3 parentPos);
    void skeletonRender(int boneId);
};

#endif
