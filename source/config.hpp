#ifndef SAH_CONFIG
#define SAH_CONFIG

#include <iostream>
#include <glm/glm.hpp>

namespace Config {
    namespace Window {
        extern std::string title;
        extern unsigned int height;
        extern unsigned int width;
        extern float version;
    };

    namespace Game {
        extern float viewDistance;
        extern glm::vec3 fogColor;
        extern float fogThikness;
        // TODO: add shadow maps resolution
    };

    namespace Light {
        extern glm::vec3 ambientColor;

        extern glm::vec3 directionalDir;
        extern glm::vec3 directionalColor;
    };

    namespace Player {
        static float speed = 20.0f;
        static float speedRapidMult = 3.0f;
    };

    namespace Keymap {
        static unsigned int FORWARD = 87; // z key
        static unsigned int BACKWARD = 83; // s key
        static unsigned int LEFT = 65; // q key
        static unsigned int RIGHT = 68; // d key

        static unsigned int SPEED = 340; // left shift key

        static unsigned int BASCULATE_VIEW_MOD = 86; // v key
    };

    namespace Camera {
        static float translationSpeed = 30.0f;
        static float translationSpeedRapidMult = 3.0f;
        static float rotationSensitivity = 0.05f;

        static float near = 0.1f;
    };

    namespace Terrain {
        // mesh
        extern const int chunksNbrTiles;
        extern const float tileSize;

        // island
        extern const float islandSize;

        // Bioms
        extern const unsigned int seed;
        extern const float frequancy;
        extern const float amplitude;
        extern const unsigned int elevation;
        extern const unsigned int nbrOctaves;

        // borders
        extern const float borderUVSpeed;
        extern const float uvSizeMult;

        // textures
        extern float gradientFactor;
    };
};

#endif
