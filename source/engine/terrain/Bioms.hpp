#ifndef SAH_BIOMS
#define BIOMS

#include "../../config.hpp"
#include "../../common/PerlinNoise.hpp"

class Bioms {
private:
    static PerlinNoise hightNoise;
    static unsigned int heightMapTexture;

public:
    static void GenerateIslandMap() {};

    static float getHight(float x, float z);

};

#endif
