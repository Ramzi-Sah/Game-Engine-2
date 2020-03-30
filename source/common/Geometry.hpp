#ifndef SAH_GEOMETRY
#define SAH_GEOMETRY

#include <glm/glm.hpp>

namespace Geometry{
    static glm::vec3 polarToCartezian(float radius, float yaw, float pitch){
        return glm::vec3(
            radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            radius * sin(glm::radians(pitch)),
            radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        );
    };

    static float distance2d(glm::vec3 vec1, glm::vec3 vec2){
        return sqrt(pow(vec1.x - vec2.x, 2) + pow(vec1.z - vec2.z, 2));
    };


};

#endif
