#ifndef SAH_LIGHT
#define SAH_LIGHT

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "AssetLoader.hpp"

class Light {
private:
    static glm::vec3 lightDir;
    static glm::mat4 lightView;

    // shadows TODO: despose  fbo & texture
    static unsigned int depthFrame;
    static unsigned int depthMap;
    static glm::vec4 frustumL[8];

public:
    static void setAmbientLight(glm::vec3 _ambientLight);
    static void createDirectionalLight(glm::vec3 lightDir, glm::vec3 lightColor);

    // for shadows
    static void initShadowMap();
    static void recalculateLightSpaceMat(glm::vec4* frustumW);
    static void setShadowMapUniform();
    static unsigned int getDepthFrame();
    static unsigned int getDepthMap();
    static unsigned int shadowMapResolution;

    static glm::vec4* getFrustumLight();
};

#endif
