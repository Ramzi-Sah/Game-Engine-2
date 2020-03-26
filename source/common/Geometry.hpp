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
};

#endif
