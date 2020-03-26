#ifndef SAH_VERTEX
#define SAH_VERTEX

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv) {
        position = _position;
        normal = _normal;
        uv = _uv;
    };
};

#endif
