#ifndef SAH_PHYSICSDEBUGDRAWER
#define SAH_PHYSICSDEBUGDRAWER

#include <iostream>
#include <vector>

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>

#include "../Line.hpp"

class PhysicsDebugDrawer : public btIDebugDraw {
private:
    int m_debugMode;
    static std::vector<Line*> lines;

public:
    static void render();
    static void dispose();

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    virtual void draw3dText(const btVector3& location, const char* textString);

    virtual void reportErrorWarning(const char* warningString);
    virtual void setDebugMode(int debugMode);
    virtual int getDebugMode() const;
};

#endif
