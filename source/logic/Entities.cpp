#include "Entities.hpp"

std::vector<Entity*> Entities::entities;

Entity* Entities::getEntity(btRigidBody* _rigidBody) {
    // if null return
    if (_rigidBody == nullptr) {
        // std::cout << "no entity clicked." << '\n';
        return nullptr;
    };

    // chuck entities vector for rigidBody
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->pysicsEntity->rigidBody == _rigidBody) {
            // std::cout << "clicked : " << entities[i] << '\n';
            return entities[i];
        };
    };

    // not in entity list
    // std::cout << "clicked not an entity." << '\n';

    // not an entity
    return nullptr;
};
Entity* Entities::getEntity(Model* _model) {
    // chuck entities vector for model id
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->model == _model)
            return entities[i];
    };

    // entity not found
    return nullptr;
};
bool Entities::checkEntity(Entity* _entity) {
    // chuck entities vector if entity exists
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] == _entity)
            return true;
    };

    // entity not found
    return false;
};
//------------------------------------------------------------------------------
void Entities::update(glm::vec3 camPos) {
    // update all entities
    for (int i = 0; i < entities.size(); i++) {
        // check if in with view distance
        entities[i]->enabled = Geometry::distance2d(entities[i]->getPos(), camPos) <= Config::Game::viewDistance ? true : false;

        // check if enabled
        if (!entities[i]->enabled)
            continue;

        entities[i]->update();
    };
};
void Entities::render() {
    // render all entities
    for (int i = 0; i < entities.size(); i++) {
        // check if enabled
        if (!entities[i]->enabled)
            continue;

        // render model
        entities[i]->model->render();

        // render physics box
        if (entities[i]->renderCollider)
            entities[i]->pysicsEntity->model->render();
    };
};
void Entities::renderShadows() {
    // render all entities
    for (int i = 0; i < entities.size(); i++) {
        // check if enabled
        if (!entities[i]->enabled)
            continue;

        entities[i]->model->renderShadow();
    };
};

//------------------------------------------------------------------------------
void Entities::addEntity(Entity* entity) {
    entities.push_back(entity);

    // handle gui nbrEntities
    GUI::nbrEntities = entities.size();
};
void Entities::disposeAllEntities() {
    // deselect entity first
    GUI::selectedEntity = nullptr;
    GUI::lastSelectedEntity = nullptr;

    // loop trought all entities
    for (int i = 0; i < entities.size(); i++) {
        // delete entity
        delete entities[i];
    };

    // clear entity list
    entities.clear();
};
void Entities::disposeEntity(Entity* entity) {
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] == entity) {

            // deselect entity first
            if (GUI::selectedEntity == entities[i])
                GUI::selectedEntity = nullptr;
            if (GUI::lastSelectedEntity == entities[i])
                GUI::lastSelectedEntity = nullptr;

            // delete entity
            delete entities[i];

            // remove from entities list
            entities.erase(entities.begin() + i);
        };
	};

    // handle gui nbrEntities
    GUI::nbrEntities = entities.size();
};
//------------------------------------------------------------------------------
Entity* Entities::hooveredEntity = nullptr;
void Entities::mouse_callback(bool mouseDesabled, int mouseXPos, int mouseYPos) {
    // reset hoover state of last entity if is not deleted
    if (checkEntity(hooveredEntity))
        hooveredEntity->model->hoovered = false;

    // return if mouse on gui
    if (GUI::isHovered()) {
        return;
    };

    if (!mouseDesabled) {
        // ray cast & get entity
        hooveredEntity = getEntity(Physics::rayCollisionCheck(mouseXPos, mouseYPos));

        // check if hovered something
        if (hooveredEntity == nullptr) {
            return;
        } else {
            hooveredEntity->model->hoovered = true;
        };
    };
};
void Entities::mouse_button_callback(bool mouseDesabled, int button, int action, int mouseXPos, int mouseYPos) {
    // return if mouse on gui
    if (GUI::isHovered()) {
        return;
    };

    if (!mouseDesabled) {
        if (action == 1) {
            // std::cout << "mouse clicked: " << mouseXPos << " " << mouseYPos << '\n';
            Entity* clickedEntity = getEntity(Physics::rayCollisionCheck(mouseXPos, mouseYPos));

            // set entity as not selected
            if (GUI::selectedEntity != nullptr)
                GUI::selectedEntity->model->selected = false;

            // handle gui selectedEntity
            GUI::selectedEntity = nullptr;

            // check if clicked something
            if (clickedEntity == nullptr) {
                return;
            } else {
                // set entity as selected
                clickedEntity->model->selected = true;

                // handle gui selectedEntity
                GUI::selectedEntity = clickedEntity;

                // disposeEntity(clickedEntity);
            };
        };
    };
};
void Entities::key_callback(int button, int action) {
    // escape
    if (action == 1) {
        if (button == 256) {
            // reset hoover state of last entity if is not deleted
            if (checkEntity(hooveredEntity))
                hooveredEntity->model->hoovered = false;

            // set entity as not selected
            // if (GUI::selectedEntity != nullptr)
                // GUI::selectedEntity->model->selected = false;

            // handle gui selectedEntity
            // GUI::selectedEntity = nullptr;
        };
    };
};
