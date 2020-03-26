#ifndef SAH_MODEL
#define SAH_MODEL

#include <vector>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/Vertex.hpp"
#include "assets/ShaderLoader.hpp"
#include "assets/TextureLoader.hpp"

struct Material {
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    unsigned int diffuseMap = TextureLoader::getTexture("white");

    float shininess = 32.0f;
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    unsigned int specularMap = TextureLoader::getTexture("white");

    float opacity = 1.0f;

    Material() {};
};

struct MeshGroup {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
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
    std::vector<Vertex> vertecies;

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

public:
    Model();
    Model(Model* _model);
    ~Model();

    void addVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv);
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
};

#endif
