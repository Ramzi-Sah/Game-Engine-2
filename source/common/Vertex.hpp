#ifndef SAH_VERTEX
#define SAH_VERTEX

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::ivec4 BoneIDs;
    glm::vec4 Weights;

    Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv) {
        position = _position;
        normal = _normal;
        uv = _uv;
        BoneIDs = glm::ivec4(-1, -1, -1, -1);
        Weights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    };

    Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _uv, glm::ivec4 _BoneIDs, glm::vec4 _Weights) {
        position = _position;
        normal = _normal;
        uv = _uv;
        BoneIDs = _BoneIDs;
        Weights = _Weights;
    };
};

#endif
