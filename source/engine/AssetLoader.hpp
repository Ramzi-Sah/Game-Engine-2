#ifndef SAH_ASSETLOADER
#define SAH_ASSETLOADER

#include "assets/ShaderLoader.hpp"
#include "assets/TextureLoader.hpp"
#include "assets/ModelLoader.hpp"

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
