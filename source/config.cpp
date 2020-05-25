#include "config.hpp"

//-------------------------------- Window ---------------------------------------
std::string Config::Window::title = "Engine";
unsigned int Config::Window::height = 720;
unsigned int Config::Window::width = height * 16 / 9;
float Config::Window::version = 0.1f;

//-------------------------------- Game ---------------------------------------
float Config::Game::viewDistance = 500.0f;
glm::vec3 Config::Game::fogColor = glm::vec3(0.5f, 0.75f, 1.0f);
float Config::Game::fogThikness = 50.0f;

//------------------------------- Light ----------------------------------------
glm::vec3 Config::Light::ambientColor = glm::vec3(0.544f, 0.653f, 0.815f);

glm::vec3 Config::Light::directionalDir = glm::vec3(-0.4f, 1.0f, -0.5f);
glm::vec3 Config::Light::directionalColor = glm::vec3(0.879f, 0.785f, 0.757f);

//------------------------------ Terrain ---------------------------------------
// mesh
const int Config::Terrain::chunksNbrTiles = 120; // yes signed int !
const float Config::Terrain::tileSize = 1.0f;

// island
const float Config::Terrain::islandSize = 2000.0f;

// Bioms
const unsigned int Config::Terrain::seed = 0;
const float Config::Terrain::frequancy = 0.003f;
const float Config::Terrain::amplitude = 12.0f;
const unsigned int Config::Terrain::elevation = 2;
const unsigned int Config::Terrain::nbrOctaves = 4;

// borders
const float Config::Terrain::borderUVSpeed = 0.0005f;
const float Config::Terrain::uvSizeMult = 50.0f;

// textures
float Config::Terrain::gradientFactor = 0.1f;  // -1.0f to 1.0f
