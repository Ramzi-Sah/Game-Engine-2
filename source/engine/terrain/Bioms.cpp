#include "Bioms.hpp"

PerlinNoise Bioms::hightNoise = PerlinNoise(Config::Terrain::seed);

//------------------------------------------------------------------------------
float Bioms::getHight(float x, float z) {
    // return 0.0f;

    float height = hightNoise.octaveNoise(x * Config::Terrain::frequancy, z * Config::Terrain::frequancy, Config::Terrain::nbrOctaves) * Config::Terrain::amplitude;
    height = pow(height, Config::Terrain::elevation);

    // island
    float distance_x = abs(x - Config::Terrain::islandSize * 0.5f);
    float distance_z = abs(z - Config::Terrain::islandSize * 0.5f);
    float distance = sqrt(pow(distance_x, 2) + pow(distance_z, 2)); // circular mask

    float max_width = Config::Terrain::islandSize / 2 - 100.0f ;
    float delta = distance / max_width;

    height *= fmax(0.0f, 1.0f - pow(delta, 2));

    return height;
};
