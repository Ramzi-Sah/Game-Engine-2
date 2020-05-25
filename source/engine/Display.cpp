#include "Display.hpp"

GLFWwindow* Display::window;
unsigned int Display::windowWidth;
unsigned int Display::windowHeight;

Display::Display(std::string title, unsigned int window_width, unsigned int window_height) {
    // set window size
    windowWidth = window_width;
    windowHeight = window_height;

    // init glfw
    if (!glfwInit()) {
        std::cout << "ERROR: init failed." << std::endl;
    };

    // setting some glfw attribs
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    // GLFWwindow is a struct containing window attribs
    window = glfwCreateWindow(window_width, window_height, title.c_str(), NULL, NULL);
    if (!window) {
        std::cout << "ERROR: can't open a window." << std::endl;
    };


    // register glfw functions
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // set openGL context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Failed to initialize OpenGL, install your graphics card drivers." << std::endl;
    };

    // set icon
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("data/textures/icon.png", &icons[0].width, &icons[0].height, nullptr, 0);
    glfwSetWindowIcon(window, 1, icons);
	stbi_image_free(icons[0].pixels);

    //--------------------- openGL render configs ------------------------------
    // default polygone mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_POINT, GL_LINE, GL_FILL

    // enable depth testing
    glEnable(GL_DEPTH_TEST); // enable depth test

    // enable cull face
    glEnable(GL_CULL_FACE); // enable face culling
    // glFrontFace(GL_CW); // set face culling to clock wise

    // enable blending for transparency
    glEnable(GL_BLEND); // enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // set blending function

    // anti aliasing
    glEnable(GL_MULTISAMPLE);

    // enable sentencil
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //--------------------------------------------------------------------------

    // disable mouse
    setDisableMouse();
};

/*---------------------------- callback events ------------------------------------*/
void Display::error_callback(int error, const char* description) {
    fprintf(stderr, "glfw Error: %s\n", description);
};

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // set window size
    windowWidth = width;
    windowHeight = height;

    // set openGL viewPort
    glViewport(0, 0, width, height);

    // reset projection matrix
    Camera::setProjectionMatrix(width, height);

    // std::cout << "width: " << width << " | height:" << height<< '\n';
};

// user input
void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // std::cout << "key: " << key << " | action:" << action<< '\n';

    // handle input
    Camera::key_callback(key, action);
    Entities::key_callback(key, action);
    Player::key_callback(key, action);

    // escape
    if (action == 1) {
        if (key == 256) {
            GUI::show_debug_menu = false;
            setDisableMouse();
        };
    };

    // space for test
    if (action == 1) {
        if (key == 32) {
            debug::spaceBarPressed();
        };
    };

    // F1 open gui demo
    if (action == 1) {
        if (key == 290) {
            GUI::show_debug_menu = !GUI::show_debug_menu;

            mouseDisabled = GUI::show_debug_menu;
            setDisableMouse();
        };
        // F2
        if (key == 291) {
            debug::f2Pressed();
        };
        // F3
        if (key == 292) {
            debug::renderCameraFrustum();
        };
        // F4
        if (key == 293) {
            debug::f4Pressed();
        };
        // F5
        if (key == 294) {
            debug::f5Pressed();
        };
    };
};

double Display::mouseXPos;
double Display::mouseYPos;
void Display::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // std::cout << "xpos: " << xpos << " | ypos:" << ypos<< '\n';

    // reset mouse pos var members
    mouseXPos = xpos;
    mouseYPos = ypos;

    // Camera input for rotation
    Camera::mouse_callback(mouseXPos, mouseYPos);

    // handle entities mouse moved event
    Entities::mouse_callback(mouseDisabled, mouseXPos, mouseYPos);
    Player::mouse_callback(mouseDisabled, mouseXPos, mouseYPos);
};

void Display::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // std::cout << "button: " << button << " | action:" << action << '\n';

    // handle entities mouse click input event
    Entities::mouse_button_callback(mouseDisabled, button, action, mouseXPos, mouseYPos);

    // handle buttons
    if (!mouseDisabled)
        return;

    if (action == 1) {
        if (button == 0) {
            debug::shoot();
        };
    };
};

bool Display::mouseDisabled = false;
void Display::setDisableMouse() {
    if (mouseDisabled) {
        Camera::mouseDisable(true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        Camera::mouseDisable(false);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    mouseDisabled = !mouseDisabled;

    // handle gui mouseDisabled
    GUI::mouseDisabled = mouseDisabled;
};
