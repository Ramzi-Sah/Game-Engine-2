#ifndef SAH_TERRAINCHUNK
#define SAH_TERRAINCHUNK

#include "../../../engine/Physics.hpp"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "../../../config.hpp"
#include "../../../engine/Model.hpp"
#include "Bioms.hpp"

class TerrainChunk {
private:
    static glm::vec3 calculateVertexNormal(int x, int z);
    std::vector<float> heights;

public:
    TerrainChunk() {};
    ~TerrainChunk();

    TerrainChunk(int posGridX, int posGridZ, bool isFlat);
    TerrainChunk(TerrainChunk* _terrainChunk);

    void render();
    void renderShadow();

    Model* chunk;

};

#endif
