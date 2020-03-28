#include "Terrain.hpp"

unsigned int Terrain::chunksViewDistance = 2 * (Config::Game::viewDistance / Config::Terrain::chunksNbrTiles);
std::vector<std::vector<TerrainChunk*>> Terrain::terrainChunks;

unsigned int Terrain::islandGridSize = static_cast<int>(Config::Terrain::islandSize / (Config::Terrain::chunksNbrTiles * Config::Terrain::tileSize));
std::vector<std::vector<TerrainChunk*>> Terrain::islandTerrainChunks;

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

            islandTerrainChunks[i][j] = new TerrainChunk(
                i,
                j,
                false
            );

            // loading menu
            creationStatus++;
            GUI::LoadingMenu("Generating island Chunks: ", creationStatus, pow(islandGridSize, 2));
        };
    };

    // generate chunks
    terrainPosX = posToGrid(posX);
    terrainPosZ = posToGrid(posZ);

    for (int i = 0; i < chunksViewDistance; i++) {
        terrainChunks.push_back(std::vector<TerrainChunk*>());
        for (int j = 0; j < chunksViewDistance; j++) {
            terrainChunks[i].push_back(nullptr);
            if (i + terrainPosX - offset >= 0 && i + terrainPosX - offset < islandGridSize && j + terrainPosZ - offset >= 0 && j + terrainPosZ - offset < islandGridSize) {
                terrainChunks[i][j] = new TerrainChunk(islandTerrainChunks[i + terrainPosX - offset][j - 1 + terrainPosX - offset]);
            } else {
                terrainChunks[i][j] = new TerrainChunk(
                    i + terrainPosX - offset,
                    j + terrainPosZ - offset,
                    true
                );
            };
        };
    };
    std::cout << "creating terrain took: " << glfwGetTime() - timeStart << " seconds." << '\n';
// /*
    //--------------------------------------------------------------------------
    // terrain colliders
	btTransform t;
	t.setIdentity();

    std::vector<btRigidBody::btRigidBodyConstructionInfo> colliders;

    // create box shapes
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(-Config::Terrain::islandSize/2, -10.0f, -Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(Config::Terrain::islandSize + Config::Terrain::islandSize/2, -10.0f, -Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(-Config::Terrain::islandSize/2, -10.0f, Config::Terrain::islandSize + Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(Config::Terrain::islandSize + Config::Terrain::islandSize/2, -10.0f, Config::Terrain::islandSize + Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };



    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(Config::Terrain::islandSize/2, -10.0f, -Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(-Config::Terrain::islandSize/2, -10.0f, Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(Config::Terrain::islandSize/2, -10.0f, Config::Terrain::islandSize + Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };
    {
    	btBoxShape* flatShape = new btBoxShape(btVector3(Config::Terrain::islandSize/2, 10.0f, Config::Terrain::islandSize/2));
        t.setOrigin(btVector3(Config::Terrain::islandSize + Config::Terrain::islandSize/2, -10.0f, Config::Terrain::islandSize/2));
    	btMotionState* motion = new btDefaultMotionState(t);
    	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, flatShape);
        colliders.push_back(info);
    };

    for (int i = 0; i < colliders.size(); i++) {
        colliders[i].m_friction = 0.5f; // terrain friction
    	btRigidBody* body = new btRigidBody(colliders[i]);
        Physics::addRigidBody(body);
        body->setActivationState(DISABLE_DEACTIVATION);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
        // body->setUserPointer(body);
    };

// */
    //--------------------------------------------------------------------------
    // https://www.texturemax.com/category/textures/terrain
    // set textures uniform
    glUseProgram(ShaderLoader::getShader("Terrain"));
    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.grass"),
        4
    );
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("grass1Defuse"));

    glUniform1i(
        glGetUniformLocation(ShaderLoader::getShader("Terrain"), "u_textures.rock"),
        5
    );
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::getTexture("rock"));

};
void Terrain::dispose() {
    // dispose terrain chunks
    for (int i = 0; i < chunksViewDistance; i++) {
        for (int j = 0; j < chunksViewDistance; j++) {
            delete terrainChunks[i][j];
        };
    };

    // dispose island chunks
    for (int i = 0; i < islandGridSize; i++) {
        for (int j = 0; j < islandGridSize; j++) {
            delete islandTerrainChunks[i][j];
        };
    };
};

void Terrain::render() {
    // for (int i = 0; i < islandGridSize; i++) {
    //     for (int j = 0; j < islandGridSize; j++) {
    //         islandTerrainChunks[i][j]->render();
    //     };
    // };
    for (int i = 0; i < chunksViewDistance; i++) {
        for (int j = 0; j < chunksViewDistance; j++) {
            terrainChunks[i][j]->render();
        };
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
                        else
                            terrainChunks[x][z] = new TerrainChunk(gridPosX + chunksViewDistance - 1 - offset, z + gridPosZ - offset, true);
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
                        else
                            terrainChunks[0][z] = new TerrainChunk(gridPosX - offset, gridPosZ + z - offset, true);
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
                        else
                            terrainChunks[x][z] = new TerrainChunk(gridPosX + x - offset, gridPosZ + chunksViewDistance - 1 - offset, true);
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
                        else
                            terrainChunks[x][0] = new TerrainChunk(gridPosX + x - offset, gridPosZ - offset, true);
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
