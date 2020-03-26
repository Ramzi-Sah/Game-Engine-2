#include "DebugDrawer.hpp"

std::vector<Line*> PhysicsDebugDrawer::lines;

void PhysicsDebugDrawer::render() {
	for (int i = 0; i < lines.size(); i++) {
		lines[i]->render();
	};
};
void PhysicsDebugDrawer::dispose() {
	for (int i = 0; i < lines.size(); i++) {
		delete lines[i];
	};
	lines.clear();
};

//------------------------------------------------------------------------------
void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    // std::cout << "physics debug: line should be drawn: " << from.getX() << " " << from.getY() << " " << from.getZ() << '\n';
	Line* line = new Line(
        glm::vec3(from.getX(), from.getY(), from.getZ()),
        glm::vec3(to.getX(), to.getY(), to.getZ())
    );
	line->color = glm::vec3(color.getX(), color.getY(), color.getZ());
	lines.push_back(line);
};

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
	std::cout << "physics debug: contact point should be drawn" << '\n';
};
void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) {};

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString) {
    std::cout << "physics reportErrorWarning: " << warningString << '\n';
};
void PhysicsDebugDrawer::setDebugMode(int debugMode) {m_debugMode = debugMode;};
int PhysicsDebugDrawer::getDebugMode() const {return m_debugMode;};
