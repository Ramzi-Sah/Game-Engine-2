#ifndef SAH_RAY
#define SAH_RAY

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 position;
    glm::vec3 direction;

    Ray(glm::vec3 _position, glm::vec3 _direction) {
        position = _position;
        direction = _direction;
    };
};

#endif
