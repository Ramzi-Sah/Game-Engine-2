#ifndef SAH_CAMERA
#define SAH_CAMERA

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../config.hpp"
#include "assets/AssetLoader.hpp"
#include "../common/Geometry.hpp"

struct Ray {
    glm::vec3 position;
    glm::vec3 direction;

    Ray(glm::vec3 _position, glm::vec3 _direction) {
        position = _position;
        direction = _direction;
    };

    void setSize(float size) {
        direction = glm::vec3(direction.x * size, direction.y * size, direction.z * size);
    };
};

class Camera {
private:
    // for projection
    static glm::mat4 projection;
    static float nearPlane;
    static float farPlane;
    static float fov;

    static float cascade1;
    static float cascade2;
    static float cascade3;

    // frustum
    static void reClculateFrustum();
    static glm::vec4 frustum[8];
    static void clculateFrustumWorld();
    static glm::vec4 frustumW[8];

    // set uniforms
    static void setProjectionMatrixUniform();
    static void setViewPosUniform();

    // for view
    static void setViewMatrixUniform();
    static Camera* usedCam;

    glm::mat4 view = glm::mat4(1.0f);

    /// for transfomation
    static float translationSpeed;

    glm::vec3 position = glm::vec3(0.0f, 2.0f, -5.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f,  0.0f);

    float yaw = 90.0f;
    float pitch = 0.0f;

    static unsigned int windowWidth;
    static unsigned int windowHeight;

    static float lastX;
    static float lastY;

    static bool disabledMouse;

    // frame render
    unsigned int fbo;
    unsigned int texColorBuffer;
    unsigned int rbo;

public:
    // for projection
    static void setProjectionMatrix(unsigned int _windowWidth, unsigned int _windowHeight);
    static glm::vec4* getFrustumWorld();


    // for camera view matrix
    Camera();
    ~Camera();

    // set as used cam
    void use();

    // for transformations
    void setPos(glm::vec3 _position);
    void translate(glm::vec3 _position);
    void updateTransform();
    static glm::vec3 getPos();
    static glm::vec3 getDir();

    // handle input
    static bool input_forward;
    static bool input_backward;
    static bool input_left;
    static bool input_right;
    static void key_callback(int key, int action);
    static void mouse_callback(float posX, float posY);
    static void inputUpdate(float deltaTime);
    static bool resetMouse;
    static void mouseDisable(bool disable);

    // ray cast
    static Ray rayCast(unsigned int screenX, unsigned int screenY);

    // render
    unsigned int getFrameBuffer();
    unsigned int getFrameTexture();
};

#endif
