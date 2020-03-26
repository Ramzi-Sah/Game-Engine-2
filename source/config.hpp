#ifndef SAH_CONFIG
#define SAH_CONFIG

#include <iostream>

namespace Config_Window {
    static std::string title = "Engine";
    static unsigned int height = 720;
    static unsigned int width = height * 16 / 9;
    static float version = 0.04f;
};

namespace Config_Keymap {
    static unsigned int FORWARD = 87;
    static unsigned int BACKWARD = 83;
    static unsigned int LEFT = 65;
    static unsigned int RIGHT = 68;

    static unsigned int SPEED = 340;
};

namespace Config_Camera {
    static float translationSpeed = 10.0f;
    static float translationSpeedRapidMult = 5.0f;
    static float rotationSensitivity = 0.05f;
};

namespace Config_Terrain {
    // mesh
    static const int nbrChunks = 11;
    static const int chunksNbrTiles = 60; // need to diviseble by islandSize FIXME: texture problem on non even number
    static const float tileSize = 1.0f;

    // island
    static const float islandSize = 66.0f * 10; // nbrChunks * chunksNbrTiles

    // Bioms
    static const unsigned int SEED = 0;
    static const float frequancy = 0.004f;
    static const float amplitude = 20.0f;
    static const unsigned int elevation = 2;
};

#endif
