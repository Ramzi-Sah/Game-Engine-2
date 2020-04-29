#ifndef SAH_GEOMETRY
#define SAH_GEOMETRY

#include <glm/glm.hpp>

namespace Geometry{
    static glm::vec3 polarToCartezian(float radius, float yaw, float pitch){
        // to radians
        yaw = glm::radians(yaw);
        pitch = glm::radians(pitch);

        return glm::vec3(
            radius * cos(yaw) * cos(pitch),
            radius * sin(pitch),
            radius * sin(yaw) * cos(pitch)
        );
    };
    // should use other methode
    static glm::vec3 cartezianToPolar(float x, float y, float z){
        return glm::vec3();
    };

    static float distance2d(glm::vec3 vec1, glm::vec3 vec2){
        return sqrt(pow(vec1.x - vec2.x, 2) + pow(vec1.z - vec2.z, 2));
    };


};

#endif
