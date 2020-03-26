#ifndef SAH_TERRAIN
#define SAH_TERRAIN

#include "TerrainChunk.hpp"
#include "../../../engine/GUI.hpp"

class Terrain {
private:
    // island
    static unsigned int islandGridSize;
    static TerrainChunk* islandTerrainChunks[300][300];

    // for update & render
    static TerrainChunk* terrainChunks[Config_Terrain::nbrChunks][Config_Terrain::nbrChunks];
    static int offset;
    static int terrainPosX, terrainPosZ;
public:
    // terrain
    static void create(float posX, float posZ, GLFWwindow* window);
    static void dispose();
    static void render();
    static void renderShadow();
    static void update(float posX, float posZ);

    // common
    static int posToGrid(float posX);

};

#endif
