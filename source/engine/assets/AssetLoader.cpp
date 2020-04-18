#include "AssetLoader.hpp"

void AssetLoader::init() {
    std::cout << "loading Assets..." << std::endl;
    float timeStart = glfwGetTime();

    createShaderPrograms();
    createTextures();
    createModels();

    std::cout << "loading Assets took: " << glfwGetTime() - timeStart << " seconds." << '\n';
};
void AssetLoader::createShaderPrograms() {
    // default shader
    ShaderLoader::createShaderProgram("Default", "data/shaders/default");
    ShaderLoader::createShaderProgram("DefaultPotato", "data/shaders/defaultPotato"); // calculates light in Vertex Shader
    ShaderLoader::createShaderProgram("ShadowDepth", "data/shaders/shadowDepth");
    ShaderLoader::createShaderProgram("Terrain", "data/shaders/terrain");
    ShaderLoader::createShaderProgram("Grass", "data/shaders/grass");
    ShaderLoader::createShaderProgram("Border", "data/shaders/border");

    // ui shaders
    ShaderLoader::createShaderProgram("Line", "data/shaders/line");
    ShaderLoader::createShaderProgram("ScreenQuad", "data/shaders/screenQuad");
    ShaderLoader::createShaderProgram("Hoover", "data/shaders/hoover");
    ShaderLoader::createShaderProgram("Select", "data/shaders/select");

    // debug shaders
    ShaderLoader::createShaderProgram("Depth", "data/shaders/depth");
    ShaderLoader::createShaderProgram("UV", "data/shaders/uv");
    ShaderLoader::createShaderProgram("Normal", "data/shaders/normal");
};

void AssetLoader::createModels() {
    // basic Shapes
    ModelLoader::createModel("cube", "BasicShapes/cube");
    ModelLoader::createModel("plane", "BasicShapes/plane");
    ModelLoader::createModel("cone", "BasicShapes/cone");
    ModelLoader::createModel("sphere", "BasicShapes/sphere");
    ModelLoader::createModel("icoSphere", "BasicShapes/icoSphere");
    ModelLoader::createModel("cylinder", "BasicShapes/cylinder");
    ModelLoader::createModel("suzanne", "BasicShapes/suzanne");

    // models
    ModelLoader::createModel("player", "player");
    ModelLoader::createModel("tree", "tree");
    ModelLoader::createModel("grass", "grass");

    // UI
    ModelLoader::createModel("axis", "UI/axis");
};

void AssetLoader::createTextures() {
    TextureLoader::createTexture("white", "data/textures/white.jpg", false, GL_CLAMP_TO_EDGE);

    // terrain
    TextureLoader::createTexture("grass", "data/textures/terrain/grass.jpg", false, GL_REPEAT);
    TextureLoader::createTexture("rock", "data/textures/terrain/rock.jpg", false, GL_REPEAT);

    // world border
    TextureLoader::createTexture("worldBorder", "data/textures/terrain/borders.png", true, GL_REPEAT);
    TextureLoader::createTexture("worldBorder1", "data/textures/terrain/borders1.png", true, GL_REPEAT);

    //grass 1
    TextureLoader::createTexture("grass1Defuse", "data/textures/grass/grass1/grass01.jpg", false, GL_REPEAT);
    TextureLoader::createTexture("grass1Specular", "data/textures/grass/grass1/grass01_s.jpg", false, GL_REPEAT);

    //---------------------------------------------------------------------------
    // container
    TextureLoader::createTexture("containerDefuse", "data/textures/container/diffuse.png", true, GL_CLAMP_TO_EDGE);
    TextureLoader::createTexture("containerSpecular", "data/textures/container/specular.png", true, GL_CLAMP_TO_EDGE);

    // models
    TextureLoader::createTexture("grassModelDefuse", "data/models/grass/defuse.png", true, GL_CLAMP_TO_EDGE);
};

void AssetLoader::dispose() {
    // TODO: dispose gl buffers textures models & shadow mlap texture
};
