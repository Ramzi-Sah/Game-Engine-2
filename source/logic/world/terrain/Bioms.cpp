#include "Bioms.hpp"

PerlinNoise Bioms::hightNoise = PerlinNoise(Config::Terrain::seed);

//------------------------------------------------------------------------------
float Bioms::getHight(float x, float z) {
    // return 0.0f;

// /*
    // generate height from noise
    float height = hightNoise.noise(x * Config::Terrain::frequancy, z * Config::Terrain::frequancy) * Config::Terrain::amplitude +
    hightNoise.noise(x * Config::Terrain::frequancy * 2, z * Config::Terrain::frequancy * 2) * Config::Terrain::amplitude / 2 +
    hightNoise.noise(x * Config::Terrain::frequancy * 4, z * Config::Terrain::frequancy * 4) * Config::Terrain::amplitude / 4 +
    hightNoise.noise(x * Config::Terrain::frequancy * 8, z * Config::Terrain::frequancy * 8) * Config::Terrain::amplitude / 8 +
    hightNoise.noise(x * Config::Terrain::frequancy * 20, z * Config::Terrain::frequancy * 20) * Config::Terrain::amplitude / 10;
// */

    // if (height < 0) height = -height;
    height = pow(height, Config::Terrain::elevation);

// /*
    // island
    float distance_x = abs(x - Config::Terrain::islandSize * 0.5f);
    float distance_z = abs(z - Config::Terrain::islandSize * 0.5f);
    float distance = sqrt(pow(distance_x, 2) + pow(distance_z, 2)); // circular mask

    float max_width = Config::Terrain::islandSize * 0.5f - 100.0f;
    float delta = distance / max_width;

    height *= fmax(0.0f, 1.0f - pow(delta, 2));
// */

    // return height <= 0 ? 0.0f : height;
    return height;
};
