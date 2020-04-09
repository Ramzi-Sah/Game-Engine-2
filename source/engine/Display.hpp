#ifndef SAH_DISPLAY
#define SAH_DISPLAY

#include <iostream>
#include <random>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "GUI.hpp"
#include "../logic/Entities.hpp"

namespace debug {
	void spaceBarPressed();
	void f2Pressed();
	void f4Pressed();
	void renderCameraFrustum();
};

class Display {
private:
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static double mouseXPos;
    static double mouseYPos;
    static void setDisableMouse();

public:
    Display(std::string title, unsigned int window_width, unsigned int window_height);
    ~Display() {};

    // window pointer
    static GLFWwindow* window;

	static unsigned int windowWidth;
	static unsigned int windowHeight;
	static bool mouseDisabled;
};

#endif
