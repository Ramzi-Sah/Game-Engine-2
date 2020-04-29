#include "Player.hpp"

Entity* Player::playerEntity;

void Player::init(glm::vec3 _position) {
    position = _position;

    // create player entity
    Model* characther = new Model(ModelLoader::getModel("player"));
    PhysicsEntity* charBody = new PhysicsEntity_Box(btVector3(1.0f, 2.8f, 1.0f), 0.0f, btVector3(position.x, position.y, position.z));
    playerEntity = new Entity(characther, charBody);
    playerEntity->setStaticFlag(true);

    // TODO: add on config file
    playerEntity->cameraOffsetPos = glm::vec3(0.0f, 2.0f, 0.0f);
    playerEntity->cameraDistance = 15.0f;
};

void Player::update(float deltaTime) {
    // get input
    inputUpdate(deltaTime);

    // update player
    playerEntity->update();
};
void Player::render() {
    // render player
    playerEntity->model->render();

    // render hitbox
    // playerEntity->pysicsEntity->model->render();
};
void Player::renderShadow() {
    // render player with shadow shader
    playerEntity->model->renderShadow();
};
Entity* Player::getEntity() {
    return playerEntity;
};

//--------------------------- input handlers -----------------------------------
glm::vec3 Player::position;
glm::vec3 Player::front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 Player::up = glm::vec3(0.0f, 1.0f,  0.0f);

void Player::inputUpdate(float deltaTime) {
    // return if camera not attached to player entity
    if (!playerEntity->cameraAttached)
        return;

    // move camera
    if (input_forward) {
        position += Config::Player::speed * front * deltaTime;
    };
    if (input_backward) {
        position -= Config::Player::speed * front * deltaTime;
    };
    if (input_right) {
        position += Config::Player::speed * glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime;
    };
    if (input_left) {
        position -= Config::Player::speed * glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime;
    };

    position.y = Bioms::getHight(position.x, position.z) + 3.0f;

    playerEntity->setKinematicPos(position);
};

bool Player::input_forward = false;
bool Player::input_backward = false;
bool Player::input_left = false;
bool Player::input_right = false;
void Player::key_callback(int key, int action) {
    // return if camera not attached to player entity
    if (!playerEntity->cameraAttached)
        return;

    // move player
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
            Config::Player::speed *= Config::Player::speedRapidMult;
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
            Config::Player::speed /= Config::Player::speedRapidMult;
        };
    };
};
float Player::yaw = 90.0f;
float Player::pitch = 0.0f;
float Player::lastX = Config::Window::width / 2;
float Player::lastY = Config::Window::height / 2;
void Player::mouse_callback(bool mouseDisabled, float posX, float posY) {
    // return if camera not attached to player entity
    if (!playerEntity->cameraAttached)
        return;

    // get camera yaw
    yaw = Camera::getYaw();

    // calculate fornt vector
    front = glm::normalize(Geometry::polarToCartezian(1.0f, yaw, pitch));

    // set entity rotaion
    playerEntity->setRot(glm::angleAxis(glm::radians(yaw -90.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
};
