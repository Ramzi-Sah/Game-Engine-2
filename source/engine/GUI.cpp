#include "GUI.hpp"

bool GUI::isHoovered = false;

ImGuiContext* GUI::context;
ImGuiIO* GUI::io;
ImGuiStyle* GUI::style;
void GUI::init(GLFWwindow* window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    context = ImGui::CreateContext();

    // getVars
    io = &ImGui::GetIO();
    style = &ImGui::GetStyle();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // ovverride styles to our theme
    setTheme();
};
void GUI::LoadingMenu(std::string loadingMessage, float progress, float maxVal) {
    // clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(io->DisplaySize.x/2 - 150, io->DisplaySize.y/2 - 25), ImGuiCond_Always);
    ImGui::Begin("Loading Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground);
    ImGui::Text("%s %.0f / %.0f", loadingMessage.c_str(), progress, maxVal);
    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap openGL buffers
    glfwSwapBuffers(glfwGetCurrentContext());

    // handle glfw events
    glfwPollEvents();
};

bool GUI::show_demo_window = false;
void GUI::update(float deltaTime) {
	// feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_overlay = true;

	// calculate fps
	static int FPS = 0;
	static int nbrFrames = 0;
	static float fpsTime = 0.0f;
	if (fpsTime > 1) {
        FPS = nbrFrames;
		nbrFrames = 0;
		fpsTime = 0.0f;
	} else {
		fpsTime += deltaTime;
		nbrFrames++;
	};
    if (show_overlay) {
        //--------------------------------------------------------------------------
        // debug overlay panel
        static const float DISTANCE = 10.0f;
        static int corner = 1;
        ImVec2 window_pos = ImVec2((corner & 1) ? io->DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io->DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::Begin("debug overlay", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

        ImGui::Text("SAH Engine V%.2f", Config::Window::version);
        // ImGui::Separator();
        ImGui::Text("FPS: %i (%.2f ms)", FPS, deltaTime*1000);
        ImGui::Text("Number of Entities: %u", nbrEntities);

        ImGui::End();
        //--------------------------------------------------------------------------
    };

    //--------------------------------------------------------------------------
    // entity config panel

    // reset kinematic status
    if (lastSelectedEntity != nullptr) {
        if (lastSelectedEntity != selectedEntity)
            lastSelectedEntity->setKinematicTemp(lastSelectedEntity->getIsKinematic());
    };

    if (selectedEntity != nullptr) {
        show_overlay = false;
        static const float size = 300.0f;
        ImGui::SetNextWindowPos(ImVec2(io->DisplaySize.x - size, 0));
        ImGui::SetNextWindowSize(ImVec2(size, io->DisplaySize.y));
        ImGui::GetStyle().WindowRounding = 0.0f;

        ImGui::Begin("Entity config panel", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        isHoovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

        // set as kinematic
        if (lastSelectedEntity == selectedEntity)
            selectedEntity->setKinematicTemp(true);

        if (selectedEntity == nullptr) {
            ImGui::Text("no entity selected.");
        } else {
            ImGui::Text("Entity %p", selectedEntity);

            // transforms ------------------------------------------------------------------------
            ImGui::Separator();
            ImGui::Text("Transforms:");

            // position
            glm::vec3 position = selectedEntity->getPos();
            float vec3fPosition[] = {position.x, position.y, position.z};
            ImGui::DragFloat3("Position", vec3fPosition, 0.1f);
            selectedEntity->setPos(glm::vec3(vec3fPosition[0], vec3fPosition[1], vec3fPosition[2]));

            // rotation
            glm::vec3 rotation = glm::degrees(glm::eulerAngles(selectedEntity->getRot()));
            float vec3fRotation[] = {rotation.x, rotation.y, rotation.z};
            ImGui::DragFloat3("Rotation", vec3fRotation, 1.0f);
            selectedEntity->setRot(glm::quat(glm::radians(glm::vec3(vec3fRotation[0], vec3fRotation[1], vec3fRotation[2]))));

            // Pysics propraities ------------------------------------------------------------------------
            ImGui::Separator();
            ImGui::Text("Physics proprities:");

            // is static flag
            bool isStatic = selectedEntity->getIsStatic();
            ImGui::Checkbox("Static", &isStatic);
            selectedEntity->setStaticFlag(isStatic);

            // is kinematic flag
            bool isKinematic = selectedEntity->getIsKinematic();
            ImGui::Checkbox("Kinematic", &isKinematic);
            selectedEntity->setKinematicFlag(isKinematic);


            // show collider
            ImGui::Checkbox("Show Collider", &selectedEntity->renderCollider);
            if (selectedEntity->renderCollider)
                selectedEntity->pysicsEntity->rigidBody->setCollisionFlags(selectedEntity->pysicsEntity->rigidBody->getCollisionFlags() & ~btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
            else
                selectedEntity->pysicsEntity->rigidBody->setCollisionFlags(selectedEntity->pysicsEntity->rigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);


            // set mass
            // float mass = selectedEntity->getMass();
            // ImGui::DragFloat("Mass", &mass, 0.01f);
            // selectedEntity->setMass(mass);

            // material propraities ------------------------------------------------------------------------
            ImGui::Separator();
            ImGui::Text("Material proprities:");

            // mesh groups
            ImGui::Text("Number of mesh groups: %lu", selectedEntity->model->meshGroups.size());
            const char* modelMeshGroups[selectedEntity->model->meshGroups.size()];
            static int modelMeshGroup_current = 0;
            if (selectedEntity != lastSelectedEntity) modelMeshGroup_current = 0; // reset current meshGroup if changed entity
            for (int i = 0; i < selectedEntity->model->meshGroups.size(); i++) {
                std::string meshGroupName = "meshgroup " + std::to_string(i);
                modelMeshGroups[i] = meshGroupName.c_str();
            };
            ImGui::Combo("Mesh Groups", &modelMeshGroup_current, modelMeshGroups, selectedEntity->model->meshGroups.size());

            // materials
            glm::vec3 ambient = selectedEntity->model->meshGroups[modelMeshGroup_current]->material.ambient;
            ImVec4 ambientColor = ImVec4(ambient.r, ambient.g, ambient.b, 0.0f);
            glm::vec3 diffuse = selectedEntity->model->meshGroups[modelMeshGroup_current]->material.diffuse;
            ImVec4 diffuseColor = ImVec4(diffuse.r, diffuse.g, diffuse.b, 0.0f);
            glm::vec3 specular = selectedEntity->model->meshGroups[modelMeshGroup_current]->material.specular;
            ImVec4 specularColor = ImVec4(specular.r, specular.g, specular.b, 0.0f);

            ImGui::ColorEdit3("Ambient", (float*)&ambientColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::ColorEdit3("Diffuse", (float*)&diffuseColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::ColorEdit3("Specular", (float*)&specularColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel);

            selectedEntity->model->meshGroups[modelMeshGroup_current]->material.ambient = glm::vec3(ambientColor.x, ambientColor.y, ambientColor.z);
            selectedEntity->model->meshGroups[modelMeshGroup_current]->material.diffuse = glm::vec3(diffuseColor.x, diffuseColor.y, diffuseColor.z);
            selectedEntity->model->meshGroups[modelMeshGroup_current]->material.specular = glm::vec3(specularColor.x, specularColor.y, specularColor.z);

            float shininess = selectedEntity->model->meshGroups[modelMeshGroup_current]->material.shininess;
            ImGui::SliderFloat("Shininess", &shininess, 1.0f, 50.0f, "%.1f");
            selectedEntity->model->meshGroups[modelMeshGroup_current]->material.shininess = shininess;

            float opacity = selectedEntity->model->meshGroups[modelMeshGroup_current]->material.opacity;
            ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f, "%.2f");
            selectedEntity->model->meshGroups[modelMeshGroup_current]->material.opacity = opacity;

            // more propraities ------------------------------------------------------------------------
            ImGui::Separator();
            ImGui::Text("Other proprities:");

            // polygone mode
            static const char*  polygoneModes[] = {"Fill", "Lines", "Points"};
            unsigned int curentPolygoneMode = selectedEntity->model->getPolygoneMode();
            static int polygoneMode_curent;
            if (curentPolygoneMode == GL_FILL)
                polygoneMode_curent = 0;
            else if (curentPolygoneMode == GL_LINE)
                polygoneMode_curent = 1;
            else if (curentPolygoneMode == GL_POINT)
                polygoneMode_curent = 2;
            ImGui::Combo("Draw mode", &polygoneMode_curent, polygoneModes, 3);
            if (polygoneMode_curent == 0)
                selectedEntity->model->setPolygoneMode(GL_FILL);
            else if (polygoneMode_curent == 1)
                selectedEntity->model->setPolygoneMode(GL_LINE);
            else if (polygoneMode_curent == 2)
                selectedEntity->model->setPolygoneMode(GL_POINT);

            // cast shadow
            ImGui::Checkbox("Cast Shadows", &selectedEntity->model->castShadow);

            // set shader
            const char* shaders[ShaderLoader::shaderPrograms.size()];
            int shaders_i = 0;
            static int shader_current = 0;
            for (auto it = ShaderLoader::shaderPrograms.cbegin(); it != ShaderLoader::shaderPrograms.cend(); ++it) {
                if (it->first != "Default" && it->first != "DefaultPotato" && it->first != "Depth" && it->first != "UV" && it->first != "Normal") continue; // remove some shaders
                if (selectedEntity->model->getMainShaderProgram() == it->second) shader_current = shaders_i; // reset selected shader to model shader
                shaders[shaders_i] = (it->first).c_str();
                shaders_i++;
            };
            ImGui::Combo("Shader", &shader_current, shaders, shaders_i);
            selectedEntity->model->setMainShaderProgram(ShaderLoader::getShader(shaders[shader_current]));

            // attach camera
            if (!selectedEntity->cameraAttached){
                if (ImGui::Button("Attach camera"))
                    Camera::getUsedCam()->attachCurentToEntity(selectedEntity);
            } else {
                if (ImGui::Button("Detach camera"))
                    Camera::getUsedCam()->detach();
            }
        };

        lastSelectedEntity = selectedEntity;
        ImGui::End();
    } else{
        show_overlay = true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // demo panel
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    };
    //--------------------------------------------------------------------------

    if (mouseDisabled) {
        // disable imGUI mouse input
        io->ConfigFlags = ImGuiConfigFlags_NoMouse;
        //--------------------------------------------------------------------------
    } else {
        // enable imGUI mouse input
        io->ConfigFlags = !ImGuiConfigFlags_NoMouse;

        //--------------------------------------------------------------------------
    };

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

bool GUI::isHovered() {
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || isHoovered;
};

//------------------------------------------------------------------------------
void GUI::setTheme() {
    // set font
    ImFont* font = io->Fonts->AddFontFromFileTTF("data/fonts/Roboto-Regular.ttf", 15.0f);

    /*------------------------------------ sizes ------------------------------------*/
    style->WindowPadding = ImVec2(10.0f, 10.0f);
    style->FramePadding = ImVec2(10.0f, 5.0f);
    style->ItemSpacing = ImVec2(10.0f, 5.0f);
    style->ItemInnerSpacing = ImVec2(10.0f, 5.0f);
    style->TouchExtraPadding = ImVec2(0.0f, 0.0f);
    style->IndentSpacing = 20.0f;
    style->ScrollbarSize = 15.0f;
    style->GrabMinSize = 10.0f;

    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize = 0.0f;
    style->PopupBorderSize = 0.0f;
    style->FrameBorderSize = 0.0f;
    style->TabBorderSize = 0.0f;

    style->WindowRounding = 5.0f;
    style->ChildRounding = 0.0f;
    style->FrameRounding = 0.0f;
    style->PopupRounding = 5.0f;
    style->ScrollbarRounding = 0.0f;
    style->GrabRounding = 0.0f;
    style->TabRounding = 4.0f;

    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style->SelectableTextAlign = ImVec2(0.0f, 0.0f);
    style->DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);

    /*------------------------------------ colors ------------------------------------*/
    #define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
    #define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
    #define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
    #define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
    #define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    style->Colors[ImGuiCol_Text]                  = TEXT(0.78f);
    style->Colors[ImGuiCol_TextDisabled]          = TEXT(0.28f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style->Colors[ImGuiCol_ChildWindowBg]         = BG( 0.58f);
    style->Colors[ImGuiCol_PopupBg]               = BG( 0.9f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]               = BG( 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]        = MED( 0.78f);
    style->Colors[ImGuiCol_FrameBgActive]         = MED( 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = LOW( 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]         = HI( 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = BG( 0.75f);
    style->Colors[ImGuiCol_MenuBarBg]             = BG( 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg]           = BG( 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = MED( 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = MED( 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered]         = MED( 0.86f);
    style->Colors[ImGuiCol_ButtonActive]          = MED( 1.00f);
    style->Colors[ImGuiCol_Header]                = MED( 0.76f);
    style->Colors[ImGuiCol_HeaderHovered]         = MED( 0.86f);
    style->Colors[ImGuiCol_HeaderActive]          = HI( 1.00f);
    style->Colors[ImGuiCol_Separator]             = TEXT(0.8f);
    style->Colors[ImGuiCol_SeparatorHovered]      = MED( 0.78f);
    style->Colors[ImGuiCol_SeparatorActive]       = MED( 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = MED( 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive]      = MED( 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = TEXT(0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = MED( 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = TEXT(0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = MED( 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = MED( 0.43f);
};
//------------------------------------------------------------------------------
bool GUI::mouseDisabled = false;
unsigned int GUI::nbrEntities = 0;
Entity* GUI::selectedEntity = nullptr;
Entity* GUI::lastSelectedEntity = nullptr;
