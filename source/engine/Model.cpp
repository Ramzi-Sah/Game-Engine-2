#include "Model.hpp"

Model::Model() {};
Model::Model(Model* _model) {
    // copy model config
    mainShaderProgram = _model->getMainShaderProgram();
    polygoneMode = _model->getPolygoneMode();
    modelSize = _model->getModelSize();

    // copy model meshGroups
    for (int i = 0; i < _model->meshGroups.size(); i++) {
        // new meshGroup
        MeshGroup* meshGroup = new MeshGroup();

        // set as copy
        meshGroup->copy = true;

        // copy meshGroup proprities
        meshGroup->VAO = _model->meshGroups[i]->VAO;
        meshGroup->VBO = _model->meshGroups[i]->VAO;
        meshGroup->EBO = _model->meshGroups[i]->EBO;
        meshGroup->numberIndecies = _model->meshGroups[i]->numberIndecies;

        // copy material
        meshGroup->material = _model->meshGroups[i]->material;

        // push to meshGroups vector
        meshGroups.push_back(meshGroup);
    };
};

Model::~Model() {
    // draw meshGroups
    for (int i = 0; i < meshGroups.size(); i++) {
        delete meshGroups[i];
    };
};

void Model::addVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv) {
    // add vertex to model vertices
    vertecies.push_back(Vertex(_position, _normal, _uv));
};

void Model::loadVertecies(unsigned int _indecies[], unsigned int _numberIndecies, Material _material) {
    // new meshGroup
    MeshGroup* meshGroup = new MeshGroup();

    // set material
    meshGroup->material = _material;

    // get model size Bytes for monitoring
    modelSize += sizeof(Vertex) * vertecies.size();

    // generate VAO & VBO
    glGenVertexArrays(1, &(meshGroup->VAO));
    glGenBuffers(1, &(meshGroup->VBO));

    // bind VAO & VBO
    glBindVertexArray(meshGroup->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshGroup->VBO);

    // load VBO with vertecies
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecies.size(), &vertecies[0], GL_STATIC_DRAW);

    // handle data Vertex attribs
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0); // set position vertex attrib
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*3)); // set normal vertex attrib
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float)*6)); // set uv vertex attrib
    glEnableVertexAttribArray(2);

    // create Eelement Buffer object
    glGenBuffers(1, &(meshGroup->EBO));

    // load ebo with data
    meshGroup->numberIndecies = _numberIndecies;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGroup->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshGroup->numberIndecies, &_indecies[0], GL_STATIC_DRAW);

    // unbind VBO & VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // add to meshGroups vector
    meshGroups.push_back(meshGroup);

    // pop all vertecies
    vertecies.clear();
};

void Model::draw() {
    // draw meshGroups
    for (int i = 0; i < meshGroups.size(); i++) {
        // set material uniform
        setMaterialUniform(meshGroups[i]->material);

        // bind VAO
        glBindVertexArray(meshGroups[i]->VAO);

        // draw model
        glDrawElements(GL_TRIANGLES, meshGroups[i]->numberIndecies, GL_UNSIGNED_INT, nullptr);
    };
};
void Model::render() {
    if (faceCulled)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    // check if selected
    if (!selected) {
        if (hoovered) {
            // use hoover shader
            setShaderProgram(ShaderLoader::getShader("Hoover"));
            glUseProgram(shaderProgram);
        } else {
            // use main shader program
            setShaderProgram(mainShaderProgram);
            glUseProgram(shaderProgram);
        };

        // set polygone mode
        glPolygonMode(GL_FRONT_AND_BACK, polygoneMode);

        // set uniforms
        setModelMatrixUniform();

        // draw vertecies
        draw();
    } else {
        // enable sentencil
        glEnable(GL_STENCIL_TEST);

        // allways  draw
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        // write 0xff on sentecil buffer
        glStencilMask(0xFF);

        if (hoovered) {
            // use hoover shader
            setShaderProgram(ShaderLoader::getShader("Hoover"));
            glUseProgram(shaderProgram);
        } else {
            // use main shader program
            setShaderProgram(mainShaderProgram);
            glUseProgram(shaderProgram);
        };

        // set polygone mode
        glPolygonMode(GL_FRONT_AND_BACK, polygoneMode);

        // set uniforms
        modelMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(0.95f, 0.95f, 0.95f));
        setModelMatrixUniform();

        // draw vertecies
        draw();

        // draw only when sentecil fragment != 0xff
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        // write 0x00 on sentecil not equal to 0xff
        glStencilMask(0x00);

        // use select shader program
        setShaderProgram(ShaderLoader::getShader("Select"));
        glUseProgram(shaderProgram);

        // apply scale
        modelMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(1.05f, 1.05f, 1.05f));
        setModelMatrixUniform();

        // draw vertecies
        draw();

        // allwaysdraw
        glStencilMask(0xFF);

        glClear(GL_STENCIL_BUFFER_BIT);

        // enable sentencil
        glDisable(GL_STENCIL_TEST);
    };
};
void Model::renderShadow() {
    // check if model cast shadows
    if (!castShadow)
        return;

    // use main shader program
    setShaderProgram(ShaderLoader::getShader("ShadowDepth"));
    glUseProgram(shaderProgram);

    // set polygone mode
    glPolygonMode(GL_FRONT_AND_BACK, polygoneMode);

    // set uniforms
    setModelMatrixUniform();

    // draw vertecies
    draw();

    return;
};
void Model::renderFrame(unsigned int texture) {
    // use main shader program
    setShaderProgram(mainShaderProgram);
    glUseProgram(shaderProgram);

    // set polygone mode
    glPolygonMode(GL_FRONT_AND_BACK, polygoneMode);

    // set uniforms
    setModelMatrixUniform();

    // set texture
    glUniform1i(
        glGetUniformLocation(shaderProgram, "screenTexture"),
        0
    );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw vertecies
    draw();
};

/*-----------------------------------------------------------------------------*/
void Model::setModelMatrixUniform() {
    // set model matrix transform uniform
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "u_model"),
        1,
        GL_FALSE,
        glm::value_ptr(modelMatrix)
    );
};

void Model::setPos(glm::vec3 _position) {
    // set position
    position = glm::translate(glm::mat4(1.0f), _position);
};
void Model::setRot(float _angle, glm::vec3 _axes) {
    // set rotation
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(_angle), _axes);
};
void Model::setRot(glm::quat _rotation) {
    // set rotation from quaternion
    rotation = glm::toMat4(_rotation);
};
void Model::setScale(glm::vec3 _scale) {
    // set rotation
    scale = glm::scale(glm::mat4(1.0f), _scale);
};

void Model::updateTransform() {
    // recalculate model matrix
    modelMatrix = position * rotation * scale;
};

//------------------------------------------------------------------------------
void Model::setMaterialUniform(Material _material) {
    // set ambiant
    glUniform3f(
        glGetUniformLocation(shaderProgram, "u_material.ambient"),
        _material.ambient.r,
        _material.ambient.g,
        _material.ambient.b
    );

    // set diffuse
    glUniform3f(
        glGetUniformLocation(shaderProgram, "u_material.diffuse"),
        _material.diffuse.r,
        _material.diffuse.g,
        _material.diffuse.b
    );

    // set defuse map
    glUniform1i(
        glGetUniformLocation(shaderProgram, "u_material.diffuseMap"),
        1
    );
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _material.diffuseMap);

    // set shininess
    glUniform1f(
        glGetUniformLocation(shaderProgram, "u_material.shininess"),
        _material.shininess
    );

    // set specular
    glUniform3f(
        glGetUniformLocation(shaderProgram, "u_material.specular"),
        _material.specular.r,
        _material.specular.g,
        _material.specular.b
    );

    // set specular map
    glUniform1i(
        glGetUniformLocation(shaderProgram, "u_material.specularMap"),
        2
    );
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _material.specularMap);

    // set opacity
    glUniform1f(
        glGetUniformLocation(shaderProgram, "u_material.opacity"),
        _material.opacity
    );
};

//------------------------------------------------------------------------------
void Model::setShaderProgram(unsigned int _shaderProgram) {
    shaderProgram = _shaderProgram;
};
void Model::setMainShaderProgram(unsigned int _shaderProgram) {
    mainShaderProgram = _shaderProgram;
};
unsigned int  Model::getMainShaderProgram() {
    return mainShaderProgram;
};
void Model::setPolygoneMode(unsigned int _polygoneMode) {
    polygoneMode = _polygoneMode;
};
unsigned int Model::getPolygoneMode() {
    return polygoneMode;
};
unsigned int Model::getModelSize() {
    return modelSize;
};
