#ifndef SAH_ENTITIES
#define SAH_ENTITIES

#include <vector>

#include "../engine/Entity.hpp"
#include "../engine/GUI.hpp"

class Entities {
private:
    static Entity* hooveredEntity;

public:
    static std::vector<Entity*> entities;

    // getters
    static Entity* getEntity(btRigidBody* _rigidBody);
    static Entity* getEntity(Model* _model);
    static bool checkEntity(Entity* _entity);

    static void update();
    static void render();
    static void renderShadows();

    static void addEntity(Entity* entity);
    static void disposeEntity(Entity* entity);
    static void disposeAllEntities();

    static void mouse_button_callback(bool mouseDesabled, int button, int action, int mouseXPos, int mouseYPos);
    static void mouse_callback(bool mouseDesabled, int mouseXPos, int mouseYPos);
    static void key_callback(int button, int action);
};

#endif
