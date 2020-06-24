#ifndef SAH_LINE
#define SAH_LINE

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

class Line {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<Vertex> vertecies;

    void setMaterialUniform();

public:
    Line();
    ~Line();

    void addLine(glm::vec3 from, glm::vec3 to);
    void loadVertecies();
    void render();

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif
