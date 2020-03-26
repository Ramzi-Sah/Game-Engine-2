#include "Terrain.hpp"

TerrainChunk* Terrain::terrainChunks[Config_Terrain::nbrChunks][Config_Terrain::nbrChunks];
int Terrain::offset = Config_Terrain::nbrChunks / 2;
int Terrain::terrainPosX, Terrain::terrainPosZ;

unsigned int Terrain::islandGridSize = static_cast<int>(Config_Terrain::islandSize / (Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize));
TerrainChunk* Terrain::islandTerrainChunks[300][300]; // FIXME: size is posGridZ

void Terrain::create(float posX, float posZ, GLFWwindow* window) {
    std::cout << "creating terrain..." << std::endl;
    float timeStart = glfwGetTime();

    // std::cout << "island Grid Size: " << islandGridSize << '\n';

    // generate island chunks
    float creationStatus = 0.0f;
    for (int i = 0; i < islandGridSize; i++) {
        for (int j = 0; j < islandGridSize; j++) {
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

    for (int i = 0; i < Config_Terrain::nbrChunks; i++) {
        for (int j = 0; j < Config_Terrain::nbrChunks; j++) {
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
/*
    // terrain collider
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0.0f, -10.0f, 0.0f));

    // create box shape
	btBoxShape* cube = new btBoxShape(btVector3(FLT_MAX, 10.0f, FLT_MAX));
	btMotionState* motion = new btDefaultMotionState(t);

    // create box body
	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(0.0f, motion, cube);
    info.m_friction = 1.0f;

    // create rigidBody
	btRigidBody* body = new btRigidBody(info);

	// add body to simulation
    Physics::addRigidBody(body);

    // set some attribs
    body->setActivationState(DISABLE_DEACTIVATION);
    body->setUserPointer(body);
*/

};
void Terrain::dispose() {
    // dispose terrain chunks
    for (int i = 0; i < Config_Terrain::nbrChunks; i++) {
        for (int j = 0; j < Config_Terrain::nbrChunks; j++) {
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
    for (int i = 0; i < Config_Terrain::nbrChunks; i++) {
        for (int j = 0; j < Config_Terrain::nbrChunks; j++) {
            terrainChunks[i][j]->render();
        };
    };
};
void Terrain::renderShadow() {
    for (int i = 0; i < Config_Terrain::nbrChunks; i++) {
        for (int j = 0; j < Config_Terrain::nbrChunks; j++) {
            terrainChunks[i][j]->renderShadow();
        };
    };
};

int nbr = 0;
void Terrain::update(float posX, float posZ) {
    // FIXME: Optimize conditions
    // std::cout << "posX : " << posX << "  " << posToGrid(posX) << " | posZ : " <<  posZ << "  " << posToGrid(posZ) << '\n';
    // std::cout << "posToGridX : " << posToGrid(posX) << " | posToGridZ : " << posToGrid(posZ) << '\n';

    // from pos to grid pos
    int gridPosX = posToGrid(posX);
    int gridPosZ = posToGrid(posZ);

    // reorder terrainChunks array and create new terrain chunks row
    if (terrainPosX != gridPosX) {

        if (terrainPosX < gridPosX) {
            for (int z = 0; z < Config_Terrain::nbrChunks; z++)
                    delete terrainChunks[0][z];

            for (int x = 0; x < Config_Terrain::nbrChunks; x++) {
                for (int z = 0; z < Config_Terrain::nbrChunks; z++) {
                    if (x+1 == Config_Terrain::nbrChunks) {
                        if (gridPosX + Config_Terrain::nbrChunks - 1 - offset >= 0 && gridPosX + Config_Terrain::nbrChunks - 1 - offset < islandGridSize && z + gridPosZ - offset >= 0 && z + gridPosZ - offset < islandGridSize)
                            terrainChunks[x][z] = new TerrainChunk(islandTerrainChunks[gridPosX + Config_Terrain::nbrChunks - 1 - offset][z + gridPosZ - offset]);
                        else
                            terrainChunks[x][z] = new TerrainChunk(gridPosX + Config_Terrain::nbrChunks - 1 - offset, z + gridPosZ - offset, true);
                    } else {
                        terrainChunks[x][z] = terrainChunks[x+1][z];
                    };
                };
            };
        } else {
            for (int z = 0; z < Config_Terrain::nbrChunks; z++)
                delete terrainChunks[Config_Terrain::nbrChunks-1][z];

            for (int x = Config_Terrain::nbrChunks-1; x >= 0; x--) {
                for (int z = Config_Terrain::nbrChunks-1; z >= 0; z--) {
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
            for (int x = 0; x < Config_Terrain::nbrChunks; x++)
                delete terrainChunks[x][0];

            for (int x = 0; x < Config_Terrain::nbrChunks; x++) {
                for (int z = 0; z < Config_Terrain::nbrChunks; z++) {
                    if (z+1 == Config_Terrain::nbrChunks) {
                        if (gridPosX + x - offset >= 0 && gridPosX + x - offset < islandGridSize && gridPosZ + Config_Terrain::nbrChunks - 1 - offset >= 0 && gridPosZ + Config_Terrain::nbrChunks - 1 - offset < islandGridSize)
                            terrainChunks[x][z] = new TerrainChunk(islandTerrainChunks[gridPosX + x - offset][gridPosZ + Config_Terrain::nbrChunks - 1 - offset]);
                        else
                            terrainChunks[x][z] = new TerrainChunk(gridPosX + x - offset, gridPosZ + Config_Terrain::nbrChunks - 1 - offset, true);
                    } else {
                        terrainChunks[x][z] = terrainChunks[x][z+1];
                    };
                };
            };
        } else {
            for (int x = 0; x < Config_Terrain::nbrChunks; x++)
                delete terrainChunks[x][Config_Terrain::nbrChunks-1];

            for (int x = Config_Terrain::nbrChunks-1; x >= 0; x--) {
                for (int z = Config_Terrain::nbrChunks-1; z >= 0; z--) {
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
        return pos / (Config_Terrain::chunksNbrTiles * Config_Terrain::tileSize) - 1;
    else
        return pos / Config_Terrain::chunksNbrTiles / Config_Terrain::tileSize;
};
