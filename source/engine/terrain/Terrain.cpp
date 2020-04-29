#include "Terrain.hpp"

unsigned int Terrain::chunksViewDistance = 2 * (Config::Game::viewDistance / Config::Terrain::chunksNbrTiles);
std::vector<std::vector<TerrainChunk*>> Terrain::terrainChunks;

unsigned int Terrain::islandGridSize = static_cast<int>(Config::Terrain::islandSize / (Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize)) + 1;
std::vector<std::vector<TerrainChunk*>> Terrain::islandTerrainChunks;
TerrainChunk* Terrain::flatChunk;

Model* Terrain::borderModelBase;
std::vector<Model*> Terrain::terrainBorders;

int Terrain::offset = chunksViewDistance / 2;
int Terrain::terrainPosX, Terrain::terrainPosZ;

void Terrain::create(float posX, float posZ, GLFWwindow* window) {
    std::cout << "creating terrain..." << std::endl;
    float timeStart = glfwGetTime();

    // generate island chunks
    float creationStatus = 0.0f;
    for (int i = 0; i < islandGridSize; i++) {
        islandTerrainChunks.push_back(std::vector<TerrainChunk*>());
        for (int j = 0; j < islandGridSize; j++) {
            islandTerrainChunks[i].push_back(nullptr);
            islandTerrainChunks[i][j] = new TerrainChunk(i, j, false);

            // loading menu
            creationStatus++;
            GUI::LoadingMenu("Generating island Chunks: ", creationStatus, pow(islandGridSize, 2));
        };
    };
    // std::cout << "island generated." << std::endl;

    // generate flat terrain
    flatChunk = new TerrainChunk(0, 0, true);

    // generate chunks
    terrainPosX = posToGrid(posX);
    terrainPosZ = posToGrid(posZ);

    // std::cout << "terrainPosX: " << terrainPosX << '\n';
    // std::cout << "terrainPosZ: " << terrainPosZ << '\n';

    for (int i = 0; i < chunksViewDistance; i++) {
        terrainChunks.push_back(std::vector<TerrainChunk*>());
        for (int j = 0; j < chunksViewDistance; j++) {
            terrainChunks[i].push_back(nullptr);
            if (i + terrainPosX - offset >= 0 && i + terrainPosX - offset < islandGridSize && j + terrainPosZ - offset >= 0 && j + terrainPosZ - offset < islandGridSize) {
                terrainChunks[i][j] = new TerrainChunk(islandTerrainChunks[i + terrainPosX - offset][j + terrainPosX - offset]);
            } else {
                terrainChunks[i][j] = new TerrainChunk(flatChunk);
                terrainChunks[i][j]->setPosGrid(i + terrainPosX - offset, j + terrainPosZ - offset);
            };
        };
    };
    std::cout << "creating terrain took: " << glfwGetTime() - timeStart << " seconds." << '\n';

    //--------------------------------------------------------------------------
    // terrain borders
	btTransform t;
	t.setIdentity();

    std::vector<btRigidBody::btRigidBodyConstructionInfo> colliders;
    static const float thikness = 1.0f;
    static const float maxHeight = 999.0f; // FLT_MAX dosen't do the trick :(

    // create border model
    borderModelBase = new Model();
    borderModelBase->addVertex(glm::vec3(0.0f, -maxHeight, -Config::Terrain::islandSize/2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    borderModelBase->addVertex(glm::vec3(0.0f, -maxHeight,  Config::Terrain::islandSize/2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, Config::Terrain::islandSize / Config::Terrain::uvSizeMult));
    borderModelBase->addVertex(glm::vec3(0.0f,  maxHeight,  Config::Terrain::islandSize/2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(maxHeight*2 / Config::Terrain::uvSizeMult, Config::Terrain::islandSize / Config::Terrain::uvSizeMult));
    borderModelBase->addVertex(glm::vec3(0.0f,  maxHeight, -Config::Terrain::islandSize/2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(maxHeight*2 / Config::Terrain::uvSizeMult, 0.0f));
    unsigned int borderModelBaseIndecies[] = {
        2, 1, 0,
        3, 2, 0
    };

    // material
    Material borderModelBaseMaterial = Material();
	borderModelBaseMaterial.diffuseMap = TextureLoader::getTexture("worldBorder");
	borderModelBaseMaterial.ambient = glm::vec3(0.0f, 0.0f, 0.0f);

	borderModelBase->castShadow = false;
	borderModelBase->faceCulled = false; // FIXME: not working

    borderModelBase->setMainShaderProgram(ShaderLoader::getShader("Border"));

    borderModelBase->loadVertecies(borderModelBaseIndecies, 6, borderModelBaseMaterial);

    {
        Model* borderModel = new Model(borderModelBase);
        borderModel->setPos(glm::vec3(Config::Terrain::islandSize/2, 0.0f, 0.0f));
        borderModel->setRot(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        borderModel->updateTransform();
        terrainBorders.push_back(borderModel);
    };

    {
        Model* borderModel = new Model(borderModelBase);
        borderModel->setPos(glm::vec3(0.0f, 0.0f, Config::Terrain::islandSize/2));
        borderModel->updateTransform();
        terrainBorders.push_back(borderModel);
    };

    {
        Model* borderModel = new Model(borderModelBase);
        borderModel->setPos(glm::vec3(Config::Terrain::islandSize/2, 0.0f, Config::Terrain::islandSize));
        borderModel->setRot(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        borderModel->updateTransform();
        terrainBorders.push_back(borderModel);
    };

    {
        Model* borderModel = new Model(borderModelBase);
        borderModel->setPos(glm::vec3(Config::Terrain::islandSize, 0.0f, Config::Terrain::islandSize/2));
        borderModel->setRot(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        borderModel->updateTransform();
        terrainBorders.push_back(borderModel);
    };

    Physics::addBox(btVector3(Config::Terrain::islandSize/2, maxHeight, thikness), 0.0f, btVector3(Config::Terrain::islandSize/2, 0.0f, -thikness/2));
    Physics::addBox(btVector3(thikness, maxHeight, Config::Terrain::islandSize/2), 0.0f, btVector3(-thikness/2, 0.0f, Config::Terrain::islandSize/2));
    Physics::addBox(btVector3(Config::Terrain::islandSize/2, maxHeight, thikness), 0.0f, btVector3(Config::Terrain::islandSize/2, 0.0f, Config::Terrain::islandSize+thikness/2));
    Physics::addBox(btVector3(thikness, maxHeight, Config::Terrain::islandSize/2), 0.0f, btVector3(Config::Terrain::islandSize+thikness/2, 0.0f, Config::Terrain::islandSize/2));
    // body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

// */
    //--------------------------------------------------------------------------
    // https://www.texturemax.com/category/textures/terrain
    // set textures uniform
    glUseProgram(ShaderLoader::getShader("Terrain"));

    // grass 1 texture
    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.grass"),
        4
    );
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("grass1Defuse"));

    // grass 1 specular texture
    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.grass_specular"),
        5
    );
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("grass1Specular"));

    // rock 1 texture
    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.rock"),
        6
    );
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("rock"));

    // rock 1 specular texture
    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.rock_specular"),
        7
    );
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("white"));

    //--------------------------------------------------------------------------
    // generate some entities
	// trees
	for (int i = 0; i < static_cast<int>(Config::Terrain::islandSize) * 0.25f; i++) {
		Model* tree = new Model(ModelLoader::getModel("tree"));
		tree->setScale(glm::vec3(1.5f, 1.5f, 1.5f));
		tree->updateTransform();
		float posX = rand() % int(Config::Terrain::islandSize);
		float posZ = rand() % int(Config::Terrain::islandSize);
		PhysicsEntity* treeBody = new PhysicsEntity_Box(btVector3(1.0f, 5.0f, 1.0f), 0.0f, btVector3(posX, Bioms::getHight(posX, posZ), posZ));
        Entity* treeEnt = new Entity(tree, treeBody);
		treeEnt->setRot(glm::quat(glm::radians(float(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeEnt->setStaticFlag(true);
        treeEnt->cameraOffsetPos = glm::vec3(0.0f, 7.5f, 0.0f);
		Entities::addEntity(treeEnt);
	};

	// grass
	for (int i = 0; i < static_cast<int>(Config::Terrain::islandSize) * 1.0f; i++) {
		Model* grass = new Model(ModelLoader::getModel("grass"));
		grass->setMainShaderProgram(ShaderLoader::getShader("Grass"));
		grass->meshGroups[0]->material.diffuseMap = TextureLoader::getTexture("grassModelDefuse");
		grass->faceCulled = false;
		grass->castShadow = false;
		float posX = rand() % int(Config::Terrain::islandSize);
		float posZ = rand() % int(Config::Terrain::islandSize);
		PhysicsEntity* grassBody = new PhysicsEntity_Box(btVector3(0.0f, 0.0f, 0.0f), 0.0f, btVector3(posX, Bioms::getHight(posX, posZ), posZ));
		Entity* grassEnt = new Entity(grass, grassBody);
		grassEnt->setRot(glm::quat(glm::radians(float(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f)));
        grassEnt->setStaticFlag(true);
		Entities::addEntity(grassEnt);
	};

};
void Terrain::dispose() {
    // dispose terrain chunks
    // dispose island chunks
    for (int i = 0; i < islandGridSize; i++) {
        for (int j = 0; j < islandGridSize; j++) {
            delete terrainChunks[i][j];
            delete islandTerrainChunks[i][j];
        };
    };

    // delete flat terrain
    delete flatChunk;
};

void Terrain::render() {
    for (int i = 0; i < chunksViewDistance; i++) {
        for (int j = 0; j < chunksViewDistance; j++) {
            // islandTerrainChunks[i][j]->render();
            terrainChunks[i][j]->render();
        };
    };

    // render borders
    // set border uv offset uniform
    static float uvOffset = 0.0f;
    uvOffset += Config::Terrain::borderUVSpeed;
    glUseProgram(ShaderLoader::getShader("Border"));
    glUniform1f(
        glGetUniformLocation(ShaderLoader::getShader("Border"), "u_uvOffset"),
        uvOffset
    );

    for (int i = 0; i < terrainBorders.size(); i++) {
        // terrainBorders[i]->meshGroups[0]->material.ambient = borderColor;
        terrainBorders[i]->render();
    };
};
void Terrain::renderShadow() {
    for (int i = 0; i < chunksViewDistance; i++) {
        for (int j = 0; j < chunksViewDistance; j++) {
            terrainChunks[i][j]->renderShadow();
        };
    };
};

int nbr = 0;
void Terrain::update(float posX, float posZ) {
    // FIXME: Optimize conditions (this function is called evry frame)
    // std::cout << "posX : " << posX << "  " << posToGrid(posX) << " | posZ : " <<  posZ << "  " << posToGrid(posZ) << '\n';
    // std::cout << "posToGridX : " << posToGrid(posX) << " | posToGridZ : " << posToGrid(posZ) << '\n';

    // from pos to grid pos
    int gridPosX = posToGrid(posX);
    int gridPosZ = posToGrid(posZ);

    // reorder terrainChunks array and create new terrain chunks row
    if (terrainPosX != gridPosX) {

        if (terrainPosX < gridPosX) {
            for (int z = 0; z < chunksViewDistance; z++)
                    delete terrainChunks[0][z];

            for (int x = 0; x < chunksViewDistance; x++) {
                for (int z = 0; z < chunksViewDistance; z++) {
                    if (x+1 == chunksViewDistance) {
                        if (gridPosX + chunksViewDistance - 1 - offset >= 0 && gridPosX + chunksViewDistance - 1 - offset < islandGridSize && z + gridPosZ - offset >= 0 && z + gridPosZ - offset < islandGridSize)
                            terrainChunks[x][z] = new TerrainChunk(islandTerrainChunks[gridPosX + chunksViewDistance - 1 - offset][z + gridPosZ - offset]);
                        else {
                            terrainChunks[x][z] = new TerrainChunk(flatChunk);
                            terrainChunks[x][z]->setPosGrid(gridPosX + chunksViewDistance - 1 - offset, z + gridPosZ - offset);
                        };
                    } else {
                        terrainChunks[x][z] = terrainChunks[x+1][z];
                    };
                };
            };
        } else {
            for (int z = 0; z < chunksViewDistance; z++)
                delete terrainChunks[chunksViewDistance-1][z];

            for (int x = chunksViewDistance-1; x >= 0; x--) {
                for (int z = chunksViewDistance-1; z >= 0; z--) {
                    if (x == 0) {
                        if (gridPosX - offset >= 0 && gridPosX - offset < islandGridSize && gridPosZ + z - offset >= 0 && z + gridPosZ - offset < islandGridSize)
                            terrainChunks[x][z] = new TerrainChunk(islandTerrainChunks[gridPosX - offset][gridPosZ + z - offset]);
                        else {
                            terrainChunks[0][z] = new TerrainChunk(flatChunk);
                            terrainChunks[0][z]->setPosGrid(gridPosX - offset, gridPosZ + z - offset);
                        };
                    } else {
                        terrainChunks[x][z] = terrainChunks[x-1][z];
                    };
                };
            };
        };

        // set terrain posX
        terrainPosX = gridPosX;
    };

    if (terrainPosZ != gridPosZ) {

        if (terrainPosZ < gridPosZ) {
            for (int x = 0; x < chunksViewDistance; x++)
                delete terrainChunks[x][0];

            for (int x = 0; x < chunksViewDistance; x++) {
                for (int z = 0; z < chunksViewDistance; z++) {
                    if (z+1 == chunksViewDistance) {
                        if (gridPosX + x - offset >= 0 && gridPosX + x - offset < islandGridSize && gridPosZ + chunksViewDistance - 1 - offset >= 0 && gridPosZ + chunksViewDistance - 1 - offset < islandGridSize)
                            terrainChunks[x][z] = new TerrainChunk(islandTerrainChunks[gridPosX + x - offset][gridPosZ + chunksViewDistance - 1 - offset]);
                        else {
                            terrainChunks[x][z] = new TerrainChunk(flatChunk);
                            terrainChunks[x][z]->setPosGrid(gridPosX + x - offset, gridPosZ + chunksViewDistance - 1 - offset);
                        };
                    } else {
                        terrainChunks[x][z] = terrainChunks[x][z+1];
                    };
                };
            };
        } else {
            for (int x = 0; x < chunksViewDistance; x++)
                delete terrainChunks[x][chunksViewDistance-1];

            for (int x = chunksViewDistance-1; x >= 0; x--) {
                for (int z = chunksViewDistance-1; z >= 0; z--) {
                    if (z == 0) {
                        if (gridPosX + x - offset >= 0 && gridPosX + x - offset < islandGridSize && gridPosZ - offset >= 0 && gridPosZ - offset < islandGridSize)
                            terrainChunks[x][z] =  new TerrainChunk(islandTerrainChunks[gridPosX + x - offset][gridPosZ - offset]);
                        else {
                            terrainChunks[x][0] = new TerrainChunk(flatChunk);
                            terrainChunks[x][0]->setPosGrid(gridPosX + x - offset, gridPosZ - offset);
                        };
                    } else {
                        terrainChunks[x][z] = terrainChunks[x][z-1];
                    };
                };
            };
        };

        // set terrain posZ
        terrainPosZ = gridPosZ;
    };

};

//-------------------------------------------------------------------------------
int Terrain::posToGrid(float pos) {
    if (pos < 0)
        return pos / (Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize) - 1;
    else
        return pos / Config::Terrain::chunksNbrTiles / Config::Terrain::tileSize;
};
