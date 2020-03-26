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

    // ui shaders
    ShaderLoader::createShaderProgram("Line", "data/shaders/line");
    ShaderLoader::createShaderProgram("ScreenQuad", "data/shaders/screenQuad");
    ShaderLoader::createShaderProgram("Hoover", "data/shaders/hoover");
    ShaderLoader::createShaderProgram("Select", "data/shaders/select");

    // debug shaders
    ShaderLoader::createShaderProgram("Depth", "data/shaders/depth");
    ShaderLoader::createShaderProgram("ShadowDepth", "data/shaders/shadowDepth");
    ShaderLoader::createShaderProgram("UV", "data/shaders/uv");
    ShaderLoader::createShaderProgram("Normal", "data/shaders/normal");
};

void AssetLoader::createModels() {
    // basic Shapes
    ModelLoader::createModel("cube", "BasicShapes/cube");
    ModelLoader::createModel("cone", "BasicShapes/cone");
    ModelLoader::createModel("sphere", "BasicShapes/sphere");
    ModelLoader::createModel("icoSphere", "BasicShapes/icoSphere");
    ModelLoader::createModel("cylinder", "BasicShapes/cylinder");
    ModelLoader::createModel("suzanne", "BasicShapes/suzanne");

    // models
    ModelLoader::createModel("char", "char");
    ModelLoader::createModel("t72", "t72");
    ModelLoader::createModel("tree", "tree");
    ModelLoader::createModel("tree1", "tree1");
    ModelLoader::createModel("gloc", "gloc");
    ModelLoader::createModel("ak47", "ak47");

    // UI
    ModelLoader::createModel("axis", "UI/axis");
};

void AssetLoader::createTextures() {
    TextureLoader::createTexture("white", "data/textures/white.jpg", false);

    // container
    TextureLoader::createTexture("containerDefuse", "data/textures/container/diffuse.png", true);
    TextureLoader::createTexture("containerSpecular", "data/textures/container/specular.png", true);

    //grass 1
    TextureLoader::createTexture("grass1Defuse", "data/textures/grass/grass1/grass01.jpg", false);
    TextureLoader::createTexture("grass1Specular", "data/textures/grass/grass1/grass01_s.jpg", false);
};

void AssetLoader::dispose() {
    // TODO: dispose gl buffers textures models & shadow mlap texture
};
