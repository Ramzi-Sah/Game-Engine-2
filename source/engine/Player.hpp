#ifndef SAH_PLAYER
#define SAH_PLAYER

#include "Entity.hpp"
#include "terrain/Bioms.hpp"
#include "../common/Geometry.hpp"
#include "Camera.hpp"
#include "../config.hpp"

enum ViewMode {FIRST_PERSON, THIRD_PERSON, SAH_ENUM_LAST_VALUE};

class Player {
private:
    static Entity* playerEntity;

    static glm::vec3 position;
    static glm::vec3 front;
    static glm::vec3 up;

    static float yaw;
    static float pitch;

    static bool input_forward;
    static bool input_backward;
    static bool input_left;
    static bool input_right;

    static ViewMode viewMode;
    static void viewModeApply();

public:
    static void init(glm::vec3 _position);
    static void update(float deltaTime);
    static void render();
    static void renderShadow();
    static Entity* getEntity();

    static void inputUpdate(float deltaTime);
    static void key_callback(int key, int action);
    static void mouse_callback(bool mouseDisabled, float posX, float posY);

    static void setViewMode(ViewMode _viewMode);
    static void basculateViewMode();
};

#endif
