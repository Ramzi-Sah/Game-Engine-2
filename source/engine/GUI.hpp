#ifndef SAH_GUI
#define SAH_GUI

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtx/euler_angles.hpp>

#include "Entity.hpp"
#include "Camera.hpp"
#include "assets/AssetLoader.hpp"
#include "Light.hpp"

class GUI {
private:
    static ImGuiContext* context;
    static ImGuiIO* io;
    static ImGuiStyle* style;
    static void setTheme();

    static bool isHoovered;

public:
    static void init(GLFWwindow* window);
    static void update(float deltaTime);

    static void LoadingMenu(std::string loadingMessage, float progress, float maxVal);

    static bool isHovered();

    static bool mouseDisabled;
    static unsigned int nbrEntities;
    static unsigned int nbrRenderedEntities;
    static Entity* selectedEntity;
    static Entity* lastSelectedEntity;

    // menus
    static bool show_debug_menu;
};

#endif
