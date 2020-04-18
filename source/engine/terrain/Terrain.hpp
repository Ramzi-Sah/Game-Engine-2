#ifndef SAH_TERRAIN
#define SAH_TERRAIN

#include "TerrainChunk.hpp"
#include "../GUI.hpp"

class Terrain {
private:
    static unsigned int chunksViewDistance;
    static std::vector<std::vector<TerrainChunk*>> terrainChunks;

    static unsigned int islandGridSize;
    static std::vector<std::vector<TerrainChunk*>> islandTerrainChunks;
    static TerrainChunk* flatChunk;

    static Model* borderModelBase;  // TODO: despose
    static std::vector<Model*> terrainBorders; // TODO: despose

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
