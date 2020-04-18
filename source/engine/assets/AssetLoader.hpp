#ifndef SAH_ASSETLOADER
#define SAH_ASSETLOADER

#include "ShaderLoader.hpp"
#include "TextureLoader.hpp"
#include "ModelLoader.hpp"

class AssetLoader {
private:
    static void createShaderPrograms();
    static void createTextures();
    static void createModels();

    static void dispose();
public:
    static void init();
};

#endif
