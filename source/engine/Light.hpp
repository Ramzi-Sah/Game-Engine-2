#ifndef SAH_LIGHT
#define SAH_LIGHT

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../config.hpp"
#include "assets/AssetLoader.hpp"

class Light {
private:
    static glm::vec3 lightDir;
    static glm::mat4 lightView;

    // shadows TODO: despose  fbo & texture
    static unsigned int depthFrame;
    static unsigned int depthMap;
    static glm::vec4 frustumL[8];

    static void setAmbientLight(glm::vec3 _ambientLight);
    static void setDirectionalLight(glm::vec3 lightDir, glm::vec3 lightColor);

public:
    // update ambient and directional light from config
    static void updateLightingConfig();

    // for fog
    static void setFog();

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
