#include "Bioms.hpp"

PerlinNoise Bioms::hightNoise = PerlinNoise(Config_Terrain::SEED);

//------------------------------------------------------------------------------
float Bioms::getHight(float x, float z) {
    // return 0.0f;
// /*
    // generate height from noise
    // float height = hightNoise.noise(x * Config_Terrain::frequancy, z * Config_Terrain::frequancy) * Config_Terrain::amplitude;

    // /*
    // generate height from noise
    float height = hightNoise.noise(x * Config_Terrain::frequancy, z * Config_Terrain::frequancy) * Config_Terrain::amplitude +
    hightNoise.noise(x * Config_Terrain::frequancy * 2, z * Config_Terrain::frequancy * 2) * Config_Terrain::amplitude / 2 +
    hightNoise.noise(x * Config_Terrain::frequancy * 4, z * Config_Terrain::frequancy * 4) * Config_Terrain::amplitude / 4 +
    hightNoise.noise(x * Config_Terrain::frequancy * 8, z * Config_Terrain::frequancy * 8) * Config_Terrain::amplitude / 8;
    // */

    // if (height < 0) height = -height;
    height = pow(height, Config_Terrain::elevation);

// */
// /*
    // island
    float distance_x = abs(x - Config_Terrain::islandSize * 0.5f);
    float distance_z = abs(z - Config_Terrain::islandSize * 0.5f);
    float distance = sqrt(pow(distance_x, 2) + pow(distance_z, 2)); // circular mask

    float max_width = Config_Terrain::islandSize * 0.5f - 10.0f;
    float delta = distance / max_width;

    height *= fmax(0.0f, 1.0f - pow(delta, 2));
// */
    return height;
};
