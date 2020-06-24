#include "Player.hpp"

Entity* Player::playerEntity;

void Player::init(glm::vec3 _position) {
    position = _position;

    // create player entity
    Model* characther = new Model(ModelLoader::getModel("player"));
    PhysicsEntity* charBody = new PhysicsEntity_Box(btVector3(1.0f, 2.8f, 1.0f), 0.0f, btVector3(position.x, position.y, position.z));
    playerEntity = new Entity(characther, charBody);
    playerEntity->setStaticFlag(true);

    // set view mode
    setViewMode(THIRD_PERSON);
};

void Player::update(float deltaTime) {
    // get input
    inputUpdate(deltaTime);

    // update player
    playerEntity->update(deltaTime);
};
void Player::render() {
    // do not render body entities[i]if in first person mode
    if (viewMode == FIRST_PERSON && playerEntity->cameraAttached)
        return;

    // render player
    playerEntity->model->render();

    // render skeleton
    // playerEntity->model->animationManager.skeletonRender(0);

    // render collider
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

    playerEntity->model->animationManager.setAnimation(-1);

    // move
    if (input_forward) {
        position += Config::Player::speed * front * deltaTime;
        playerEntity->model->animationManager.setAnimation("walk");
    };
    if (input_backward) {
        position -= Config::Player::speed * front * deltaTime;
        playerEntity->model->animationManager.setAnimation("walk");
    };
    if (input_right) {
        position += Config::Player::speed * glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime;
        playerEntity->model->animationManager.setAnimation("walk");
    };
    if (input_left) {
        position -= Config::Player::speed * glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime;
        playerEntity->model->animationManager.setAnimation("walk");
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

        // basculate view mod
        if (key == Config::Keymap::BASCULATE_VIEW_MOD) {
            Player::basculateViewMode();
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

//------------------------------------------------------------------------------
ViewMode Player::viewMode = THIRD_PERSON;
void Player::setViewMode(ViewMode _viewMode) {
    viewMode = _viewMode;

    // apply view mode
    viewModeApply();
};
void Player::basculateViewMode() {
    // select next view mode
    viewMode = static_cast<ViewMode>(static_cast<int>(viewMode) + 1);

    // check if last view mod and select first
    if (viewMode == SAH_ENUM_LAST_VALUE) viewMode = static_cast<ViewMode>(0);

    // apply view mode
    viewModeApply();
};
void Player::viewModeApply() {
    if (viewMode == FIRST_PERSON){
        // first person
        playerEntity->cameraOffsetPos = glm::vec3(0.0f, 3.0f, 0.0f);
        playerEntity->cameraDistance = 0.0f;
    } else if (viewMode == THIRD_PERSON) {
        // third person
        playerEntity->cameraOffsetPos = glm::vec3(0.0f, 2.0f, 0.0f);
        playerEntity->cameraDistance = 15.0f;
    };
};
