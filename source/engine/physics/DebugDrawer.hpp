#ifndef SAH_PHYSICSDEBUGDRAWER
#define SAH_PHYSICSDEBUGDRAWER

#include <iostream>
#include <vector>

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>

#include "../Line.hpp"
#include "../../common/Ray.hpp"

class PhysicsDebugDrawer : public btIDebugDraw {
private:
    int m_debugMode;
    static std::vector<Line*> lines;

public:
    static void render();
    static void dispose();
    static void addLine(Line* line);
    static void drawRay(Ray ray, glm::vec3 color);

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color);
	// virtual void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color);



    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    void draw3dText(const btVector3& location, const char* textString);

    void reportErrorWarning(const char* warningString);
    void setDebugMode(int debugMode);
    int getDebugMode() const;
};

#endif
