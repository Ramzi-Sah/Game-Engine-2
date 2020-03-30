#ifndef SAH_CONFIG
#define SAH_CONFIG

#include <iostream>

namespace Config {
    namespace Window {
        static std::string title = "Engine";
        static unsigned int height = 720;
        static unsigned int width = height * 16 / 9;
        static float version = 0.05f;
    };

    namespace Game {
        static float viewDistance = 400.0f;
        // TODO: add shadow maps resolution
    };

    namespace Keymap {
        static unsigned int FORWARD = 87;
        static unsigned int BACKWARD = 83;
        static unsigned int LEFT = 65;
        static unsigned int RIGHT = 68;

        static unsigned int SPEED = 340;
    };

    namespace Camera {
        static float translationSpeed = 10.0f;
        static float translationSpeedRapidMult = 5.0f;
        static float rotationSensitivity = 0.05f;
    };

    namespace Terrain {
        // mesh
        static const int chunksNbrTiles = 60; // need to diviseble by islandSize FIXME: texture problem on non even number
        static const float tileSize = 1.0f;

        // island
        static const float islandSize = 400.0f;

        // Bioms
        static const unsigned int seed = 0;
        static const float frequancy = 0.005f;
        static const float amplitude = 8.0f;
        static const unsigned int elevation = 3;
    };
};

#endif
