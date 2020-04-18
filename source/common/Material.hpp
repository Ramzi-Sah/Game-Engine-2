#ifndef SAH_MATERIAL
#define SAH_MATERIAL

#include <glm/glm.hpp>
#include "../engine/assets/TextureLoader.hpp"

struct Material {
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    unsigned int diffuseMap = TextureLoader::getTexture("white");

    float shininess = 32.0f;
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    unsigned int specularMap = TextureLoader::getTexture("white");

    float opacity = 1.0f;

    Material() {};
};

#endif
