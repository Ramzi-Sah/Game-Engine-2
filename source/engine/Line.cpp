#include "Line.hpp"

Line::Line() {};
Line::~Line() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
};

//------------------------------------------------------------------------------
void Line::addLine(glm::vec3 from, glm::vec3 to) {
    vertecies.push_back(Vertex(from, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    vertecies.push_back(Vertex(to, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
};

//------------------------------------------------------------------------------
void Line::loadVertecies() {
    // generate VAO & VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind VAO & VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // load VBO with vertecies
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecies.size(), &vertecies[0], GL_STATIC_DRAW);

    // handle data Vertex attribs
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0); // set position vertex attrib
    glEnableVertexAttribArray(0);

    // create Eelement Buffer object
    glGenBuffers(1, &EBO);

    // load ebo with data
    unsigned int indecies[vertecies.size()];
    for (unsigned int i = 0; i < vertecies.size(); i++) {
        indecies[i] = i;
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertecies.size(), &indecies[0], GL_STATIC_DRAW);

    // unbind VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

void Line::render() {
    glUseProgram(ShaderLoader::getShader("Line"));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDisable(GL_CULL_FACE);

    // glUniform3f(
    //     glGetUniformLocation(ShaderLoader::getShader("Line"), "u_color"),
    //     color.r,
    //     color.g,
    //     color.b
    // );

    // bind VAO
    glBindVertexArray(VAO);

    // draw model
    glDrawElements(GL_LINES, vertecies.size(), GL_UNSIGNED_INT, nullptr);
};
