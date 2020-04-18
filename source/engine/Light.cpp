#include "Light.hpp"

//----------------------------- Ambient Light ----------------------------------
void Light::setAmbientLight(glm::vec3 _ambientLight) {
    // set all shader program's ambientLight uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniform3f(
            glGetUniformLocation(it->second, "ambientLight"),
            _ambientLight.r,
            _ambientLight.g,
            _ambientLight.b
        );
    };
};

//------------------------------ Defuse Light ----------------------------------
glm::vec3 Light::lightDir;
glm::mat4 Light::lightView;
void Light::createDirectionalLight(glm::vec3 _lightDir, glm::vec3 _lightColor) {
    // set light dir
    lightDir = glm::normalize(_lightDir);

    // calculate light view
    lightView = glm::lookAt(
        // lightDir,
        glm::vec3(0.0f, 0.0f, 0.0f),
        lightDir,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // set all shader program's defuseLight uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);

        glUniform3f(
            glGetUniformLocation(it->second, "diffuseLightDir"),
            lightDir.x,
            lightDir.y,
            lightDir.z
        );

        glUniform3f(
            glGetUniformLocation(it->second, "diffuseLightColor"),
            _lightColor.r,
            _lightColor.g,
            _lightColor.b
        );
    };
};

//-------------------------------- Fog -----------------------------------------
void Light::initFog() {
    // set all shader program's uniforms
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);

        glUniform1f(
            glGetUniformLocation(it->second, "u_near"),
            0.01f
        );

        glUniform1f(
            glGetUniformLocation(it->second, "u_far"),
            Config::Game::viewDistance
        );

        glUniform3f(
            glGetUniformLocation(it->second, "u_fogColor"),
            Config::Game::fogColor.x,
            Config::Game::fogColor.y,
            Config::Game::fogColor.z
        );
    };
};

//------------------------------ Shadow map ------------------------------------
unsigned int Light::depthFrame;
unsigned int Light::depthMap;
unsigned int Light::shadowMapResolution = 2048;
void Light::initShadowMap() {
    // generate depth buffer
    glGenFramebuffers(1, &depthFrame);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrame);

    // create depth map
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // texture borders to white
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth map to buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};
glm::vec4 Light::frustumL[8];
void Light::recalculateLightSpaceMat(glm::vec4* frustumW) {
    // cascade shadows theory http://ogldev.atspace.co.uk/www/tutorial49/tutorial49.html
    float minX =  FLT_MAX;
    float maxX = -FLT_MAX;
    float minY =  FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ =  FLT_MAX;
    float maxZ = -FLT_MAX;

    // calculate light view frustum
    for (int i = 0; i < 8; i++) {
        frustumL[i] = lightView * frustumW[i];

        minX = minX > frustumL[i].x ? frustumL[i].x : minX;
        maxX = maxX < frustumL[i].x ? frustumL[i].x : maxX;

        minY = minY > frustumL[i].y ? frustumL[i].y : minY;
        maxY = maxY < frustumL[i].y ? frustumL[i].y : maxY;

        minZ = minZ > frustumL[i].z ? frustumL[i].z : minZ;
        maxZ = maxZ < frustumL[i].z ? frustumL[i].z : maxZ;
    };

    // calculate shadow box projection
    glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, -minZ, -maxZ);

    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    // set all shader program's u_lightSpaceMatrix uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniformMatrix4fv(
            glGetUniformLocation(it->second, "u_lightSpaceMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(lightSpaceMatrix)
        );
    };
};
glm::vec4* Light::getFrustumLight() {
    return frustumL;
};

void Light::setShadowMapUniform() {
    // set all shader program's u_lightSpaceMatrix uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniform1i(
            glGetUniformLocation(it->second, "u_shadowMap"),
            0
        );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
    };
};

unsigned int Light::getDepthFrame() {
    // return depth frame buffer object
    return depthFrame;
};
unsigned int Light::getDepthMap() {
    // return depth frame buffer object
    return depthMap;
};
