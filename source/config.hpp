#ifndef SAH_CONFIG
#define SAH_CONFIG

#include <iostream>
#include <glm/glm.hpp>

namespace Config {
    namespace Window {
        static std::string title = "Engine";
        static unsigned int height = 720;
        static unsigned int width = height * 16 / 9;
        static float version = 0.09f;
    };

    namespace Game {
        static float viewDistance = 400.0f;
        static glm::vec3 fogColor = glm::vec3(0.5f, 0.75f, 1.0f);
        static float fogThikness = 75.0f;
        // TODO: add shadow maps resolution
    };

    namespace Player {
        static float speed = 20.0f;
        static float speedRapidMult = 5.0f;
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

        static float near = 0.1f;
    };

    namespace Terrain {
        // mesh
        static const int chunksNbrTiles = 60; // need to diviseble by islandSize FIXME: texture problem on non even number
        static const float tileSize = 1.0f;

        // island
        static const float islandSize = 1000.0f;

        // Bioms
        static const unsigned int seed = 0;
        static const float frequancy = 0.003f;
        static const float amplitude = 15.0f;
        static const unsigned int elevation = 2;
        static const unsigned int nbrOctaves = 5;

        // borders
        static const float borderUVSpeed = 0.0005f;
        static const float uvSizeMult = 50.0f;
    };
};

#endif
