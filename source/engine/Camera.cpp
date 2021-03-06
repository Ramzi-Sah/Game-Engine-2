#include "Camera.hpp"

//------------------------------------------------------------------------------
// projection
glm::mat4 Camera::projection = glm::mat4(1.0f);
float Camera::nearPlane = Config::Camera::near;
float Camera::farPlane = Config::Game::viewDistance - Config::Terrain::chunksNbrTiles * sqrt(2); // phytaghore's law
// float Camera::farPlane = Config::Game::viewDistance;
float Camera::fov = 60.0f;

float Camera::cascade1 = 100.0f;
float Camera::cascade2 = 200.0f;
float Camera::cascade3 = Config::Game::viewDistance;

void Camera::setProjectionMatrixUniform() {
    // set all shader program's projection uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniformMatrix4fv(
            glGetUniformLocation(it->second, "u_projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );
    };
};

unsigned int Camera::windowWidth, Camera::windowHeight;
void Camera::setProjectionMatrix(unsigned int _windowWidth, unsigned int _windowHeight) {
    // reset windowWidth & windowWidth
    windowWidth = _windowWidth;
    windowHeight = _windowHeight;

    // calculate projection matrix
    projection = glm::perspective(glm::radians(fov), (float)_windowWidth/(float)_windowHeight, nearPlane, farPlane);

    // set projection matrix uniform
    setProjectionMatrixUniform();

    // recalculate frustum
    reClculateFrustum();
};

glm::vec4 Camera::frustum[8];
void Camera::reClculateFrustum() {
    float resolution = (float)windowWidth / (float)windowHeight;
    float fovX = tanf(glm::radians(fov * resolution / 2.0f));
    float fovY = tanf(glm::radians(fov / 2.0f));

    float xn = nearPlane * fovX;
    float yn = nearPlane * fovY;
    float xf = cascade1 * fovX;
    float yf = cascade1 * fovY;

    //-------------------set near frustum------------------------
    frustum[0] = glm::vec4( xn,  yn, nearPlane, 1.0f);   // nearRightUp
    frustum[1] = glm::vec4(-xn,  yn, nearPlane, 1.0f);   // nearLeftUp
    frustum[2] = glm::vec4( xn, -yn, nearPlane, 1.0f);   // nearRightDown
    frustum[3] = glm::vec4(-xn, -yn, nearPlane, 1.0f);   // nearLeftDown

    //--------------------set far frustum-----------------------
    frustum[4] = glm::vec4( xf,  yf, cascade1, 1.0f);    // farRightUp
    frustum[5] = glm::vec4(-xf,  yf, cascade1, 1.0f);    // farLeftUp
    frustum[6] = glm::vec4( xf, -yf, cascade1, 1.0f);    // farRightDown
    frustum[7] = glm::vec4(-xf, -yf, cascade1, 1.0f);    // farLeftDown
};
glm::vec4 Camera::frustumW[8];
void Camera::clculateFrustumWorld() {
    // calculate view mat inverse
    // FIXME: view matrix should not be recalculated
    // glm::mat4 viewMatInv = glm::inverse(usedCam->view);
    glm::mat4 viewMatInv = glm::inverse(glm::lookAt(usedCam->position, usedCam->position - usedCam->front, usedCam->up));

    // calculate frustum in world space
    for (int i = 0; i < 8; i++) {
        frustumW[i] = viewMatInv * frustum[i];
    };
};

glm::vec4* Camera::getFrustumWorld() {
    return frustumW;
};
//------------------------------------------------------------------------------
// view
Camera* Camera::usedCam;
void Camera::use() {
    // set used camera as this one
    usedCam = this;

    // update view matrix uniform
    setViewMatrixUniform();

    // update view pos uniform
    setViewPosUniform();

    // update transform uniforms
    updateTransform();
};
void Camera::setViewMatrixUniform() {
    // set all shader program's u_view uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniformMatrix4fv(
            glGetUniformLocation(it->second, "u_view"),
            1,
            GL_FALSE,
            glm::value_ptr(usedCam->view)
        );
    };
};
void Camera::setViewPosUniform() {
    // set all shader program's u_view uniform
    for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
        glUseProgram(it->second);
        glUniform3f(
            glGetUniformLocation(it->second, "u_viewPos"),
            usedCam->position.x,
            usedCam->position.y,
            usedCam->position.z
        );
    };
};
Camera* Camera::getUsedCam() {
    return usedCam;
};

Camera::Camera() {
    // generate camera's frame buffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //--------------------------------------------------------------------------
    // generate camera's texture buffer
    glGenTextures(1, &texColorBuffer);

    // generate texture
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // attach texture to frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    //--------------------------------------------------------------------------
    // generate camera's render buffer
    glGenRenderbuffers(1, &rbo);

    // handle render buffer object
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);

    // attach rbo to frame buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //--------------------------------------------------------------------------
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Camera frame buffer not redy" << std::endl;

    // use default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};
Camera::~Camera() {
    // delete camera's buffers
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texColorBuffer);
    glDeleteRenderbuffers(1, &rbo);
};

unsigned int Camera::getFrameBuffer() {
    // return frame texture
    return fbo;
};
unsigned int Camera::getFrameTexture() {
    // return frame texture
    return texColorBuffer;
};

//------------------------------------------------------------------------------
// transformations
void Camera::updateTransform() {
    // recalculate view matrix
    view = glm::lookAt(position, position + front, up);

    // update view matrix uniform
    setViewMatrixUniform();
};
void Camera::setPos(glm::vec3 _position) {
    // set position
    position = _position;

    // update view pos uniform
    setViewPosUniform();
};
glm::vec3 Camera::getPos() {
    return usedCam->position;
};
glm::vec3 Camera::getFront() {
    return usedCam->front;
};
float Camera::getYaw() {
    return usedCam->yaw;
};
void Camera::translate(glm::vec3 _position) {
    // set position
    position += _position;

    // update view pos uniform
    setViewPosUniform();
};

//------------------------------------------------------------------------------
// input
bool Camera::input_forward = false;
bool Camera::input_backward = false;
bool Camera::input_left = false;
bool Camera::input_right = false;
void Camera::key_callback(int key, int action) {
    // check if mouse can move
    if (disabledMouse)
        return;

    // move camera
    if (action == 1) {
        if (key == Config::Keymap::FORWARD) {
            input_forward = true;
        };
        if (key == Config::Keymap::BACKWARD) {
            input_backward = true;
        };
        if (key == Config::Keymap::RIGHT) {
            input_right = true;
        };
        if (key == Config::Keymap::LEFT) {
            input_left = true;
        };

        // TODO: should not modify config
        if (key == Config::Keymap::SPEED) {
            Config::Camera::translationSpeed *= Config::Camera::translationSpeedRapidMult;
        };
    } else if (action == 0) {
        if (key == Config::Keymap::FORWARD) {
            input_forward = false;
        };
        if (key == Config::Keymap::BACKWARD) {
            input_backward = false;
        };
        if (key == Config::Keymap::RIGHT) {
            input_right = false;
        };
        if (key == Config::Keymap::LEFT) {
            input_left = false;
        };

        // TODO: should not modify config
        if (key == Config::Keymap::SPEED) {
            Config::Camera::translationSpeed /= Config::Camera::translationSpeedRapidMult;
        };
    };
};
float Camera::translationSpeed;
void Camera::inputUpdate(float deltaTime) {
    // recalculate frustum world space
    clculateFrustumWorld();

    if (usedCam->attached) {
        // transform camera to attached entity
        usedCam->setTransformCurrentAttached();
    } else {
        // check if mouse can move
        if (disabledMouse)
            return;

        // calcultae mouse translation speed
        translationSpeed = Config::Camera::translationSpeed * deltaTime;

        // move camera
        if (input_forward) {
            usedCam->translate(usedCam->front * translationSpeed);
            usedCam->updateTransform();
        };
        if (input_backward) {
            usedCam->translate(-usedCam->front * translationSpeed);
            usedCam->updateTransform();
        };
        if (input_right) {
            usedCam->translate(glm::normalize(glm::cross(usedCam->front, usedCam->up)) * translationSpeed);
            usedCam->updateTransform();
        };
        if (input_left) {
            usedCam->translate(-glm::normalize(glm::cross(usedCam->front, usedCam->up)) * translationSpeed);
            usedCam->updateTransform();
        };
    };
};

float Camera::lastX = Config::Window::width / 2;
float Camera::lastY = Config::Window::height / 2;
bool Camera::resetMouse = true;
void Camera::mouse_callback(float posX, float posY) {
    // check if mouse can move
    if (disabledMouse)
        return;

    if (resetMouse) {
        lastX = posX;
        lastY = posY;
        resetMouse = false;
    };

    usedCam->yaw += (posX - lastX) * Config::Camera::rotationSensitivity;
    usedCam->pitch += (lastY - posY) * Config::Camera::rotationSensitivity;

    lastX = posX;
    lastY = posY;

    if (usedCam->pitch > 89.0f)
        usedCam->pitch = 89.0f;
    if (usedCam->pitch < -89.0f)
        usedCam->pitch = -89.0f;

    usedCam->front = glm::normalize(Geometry::polarToCartezian(1.0f, usedCam->yaw, usedCam->pitch));

    usedCam->updateTransform();
};
bool Camera::disabledMouse;
void Camera::mouseDisable(bool disable) {
    disabledMouse = disable;

    if (!disable)
        resetMouse = true;
};

// for attach
void Camera::attachCurentToEntity(Entity* entity) {
    // set detached flag on last entity
    if (attachedEntity != nullptr)
        attachedEntity->cameraAttached = false;

    // set new attached entity
    attachedEntity = entity;
    attached = true;

    // set atached flag on new entity
    attachedEntity->cameraAttached = true;

    // transform camera to attached entity
    Camera::setTransformCurrentAttached();
};
void Camera::setTransformCurrentAttached() {
    setPos(attachedEntity->getPos() - Geometry::polarToCartezian(usedCam->attachedEntity->cameraDistance, usedCam->yaw, usedCam->pitch) + usedCam->attachedEntity->cameraOffsetPos);
    updateTransform();
};
void Camera::detach() {
    // set detached flag on last entity
    if (attachedEntity != nullptr)
        attachedEntity->cameraAttached = false;

    // clear attached params
    attachedEntity = nullptr;
    attached = false;
};
bool Camera::getAttached() {
    return attached;
};

//------------------------------------------------------------------------------
float Camera::raySize = Config::Game::viewDistance;
Ray Camera::rayCast(unsigned int screenX, unsigned int screenY) {
    // ray cast theory <3 http://antongerdelan.net/opengl/raycasting.html

    // convert to openGL normalized coordinate system
    float x = 2.0f * screenX / windowWidth - 1.0f;
    float y = 1.0f - 2.0f * screenY / windowHeight;

    // vec4 front direction vector
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    // clip to view coordinate
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    // view to world coordinate
    glm::vec4 rayWor = glm::inverse(usedCam->view) * rayEye;

    // ray direction
    glm::vec3 rayEnd = usedCam->position + glm::normalize(glm::vec3(rayWor.x, rayWor.y, rayWor.z)) * raySize;

    return Ray(usedCam->position, rayEnd);
};
