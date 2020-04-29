#ifndef SAH_ASSET_LOADER
#define SAH_ASSET_LOADER

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
