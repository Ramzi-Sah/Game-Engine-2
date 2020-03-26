/*
	File: main.cpp
	Author: Ramzi Sah

	Description:
		program main entry point
*/
#include <vector>

// for DEBUG
#include <bits/stdc++.h>
#define Debug(func) \
	clock_t start, end; \
	start = clock(); \
	func; \
	end = clock(); \
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC) * 1000; \
	std::cout << "Time taken by func is : " << std::fixed << time_taken << std::setprecision(5) << " ms " << std::endl;

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "engine/AssetLoader.hpp"
#include "engine/Display.hpp"
#include "engine/GUI.hpp"
#include "engine/Light.hpp"
#include "logic/Entities.hpp"
#include "logic/world/terrain/Terrain.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// mesh generation
Model* createPlane();
Model* createFrustum(glm::vec4* frustum);

// create cubes
int nbrCubes = 200;
std::vector<Entity*> cubes;
void createCubes(int nbr);
void deleteCubes();
namespace debug {
	void spaceBarPressed() {
		deleteCubes();
		createCubes(nbrCubes);
	};
	void f2Pressed() {
		Physics::debugDraw();
	};
	static Entity* frustumEnt = nullptr;
	void renderCameraFrustum() {
		//----------------------------------------------------------------------
		Model* frustum = createFrustum(Camera::getFrustumWorld());
		frustum->setPolygoneMode(GL_LINES);
		frustum->meshGroups[0]->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		frustum->meshGroups[0]->material.ambient = glm::vec3(2.0f, 1.8f, 0.0f);
		frustum->meshGroups[0]->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
		frustum->meshGroups[0]->material.opacity = 0.5f;
		frustum->castShadow = false;
		frustum->faceCulled = false;
		// frustum->setPolygoneMode(GL_LINE);
		PhysicsEntity* frustumBody = new PhysicsEntity_Box(btVector3(0.0f, 0.0f, 0.0f), 0.0f, btVector3(0.0f, 0.0f, 0.0f));

		if (frustumEnt != nullptr) Entities::disposeEntity(frustumEnt);
		frustumEnt = new Entity(frustum, frustumBody);
		Entities::addEntity(frustumEnt);
		//----------------------------------------------------------------------
		//----------------------------------------------------------------------
		// Light::recalculateLightSpaceMat(Camera::getFrustumWorld());
		// Model* frustum1 = createFrustum(Light::getFrustumLight());
		// frustum1->setPolygoneMode(GL_LINES);
		// frustum1->material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		// frustum1->material.ambient = glm::vec3(0.0f, 0.2f, 1.0f);
		// frustum1->material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
		// frustum1->material.opacity = 0.5f;
		// frustum1->castShadow = false;
		// frustum1->faceCulled = false;
		// // frustum->setPolygoneMode(GL_LINE);
		// btRigidBody* frustumBody1 = Physics::addBox(btVector3(0.0f, 0.0f, 0.0f), 0.0f, btVector3(0.0f, 0.0f, 0.0f));
		//
		// Entity* frustumEnt1 = new Entity(frustum1, frustumBody1);
		// Entities::addEntity(frustumEnt1);
		// cubes.push_back(frustumEnt1);
		//----------------------------------------------------------------------
	};
};

// render
void renderScean();
void renderSceanShadow();

// delta time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float currentFrame = 0.0f; // temp Time of frame

int main() {
	std::cout << "SAH game V" << Config_Window::version << '\n';
	// init window
	Display display(Config_Window::title, Config_Window::width, Config_Window::height);

	// init GUI
	GUI::init(display.window);

	// load assets
	AssetLoader::init();

	// main menu thread
	// while (GUI::show_main_menu) {
	// 	GUI::mainMenu();
	//
	// 	// swap openGL buffers
    // 	glfwSwapBuffers(display.window);
	//
	// 	// handle glfw events
    //     glfwPollEvents();
	// };

	// set light
	Light::setAmbientLight(glm::vec3(0.4f, 0.4f, 0.8f));
	Light::createDirectionalLight(glm::vec3(-4.0f, 6.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Light::initShadowMap();

	//--------------------------------------------------------------------------
	Physics::init();

	//--------------------------------------------------------------------------
	// init camera
    Camera::setProjectionMatrix(Config_Window::width, Config_Window::height);
	Camera freeCam = Camera();
	freeCam.use();

	//--------------------------------------------------------------------------
	// init terrain
	glm::vec3 cameraPos = Camera::getPos();
	Terrain::create(cameraPos.x, cameraPos.z, display.window);
/*
	// glfwMakeContextCurrent(NULL);
	// std::thread createTerrain_thread(Terrain::create, cameraPos.x, cameraPos.z, display.window);
	while (Terrain::creationStatus < 100.0f) {
		// glfwMakeContextCurrent(NULL);
		// glfwMakeContextCurrent(display.window);
		// std::cout << "from thread: " << Terrain::creationStatus << '\n';

    	// update GUI
        glClearColor(0.661f, 0.720f, 0.959f, 1.00f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	GUI::update(0.0f);

		// swap openGL buffers
    	glfwSwapBuffers(display.window);

		// handle glfw events
        glfwPollEvents();
	};
	// createTerrain_thread.join();
	// glfwMakeContextCurrent(display.window);
*/

	//--------------------------------------------------------------------------
	// create some cubes
	// createCubes(nbrCubes);

	// center of the world
	Model* frameQuad = createPlane();
	frameQuad->setMainShaderProgram(ShaderLoader::getShader("ScreenQuad"));

	// world center axis
	Model* center = new Model(ModelLoader::getModel("axis"));
	PhysicsEntity* centerBody = new PhysicsEntity_Box(btVector3(0.2f, 3.5f, 0.2f), 1000.0f, btVector3(0.0f, 0.0f, 0.0f));
    Entity* centerEnt = new Entity(center, centerBody);
    centerEnt->setStaticFlag(true);
	Entities::addEntity(centerEnt);

	// tank
	Model* tank = new Model(ModelLoader::getModel("t72"));
	tank->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
	tank->updateTransform();
	PhysicsEntity* tankBody = new PhysicsEntity_Box(btVector3(6.5f, 1.65f, 3.25f), 1000.0f, btVector3(-20.0f, 10.0f, 0.0f));
	Entities::addEntity(new Entity(tank, tankBody));

	// trees
	// for (int i = 0; i < static_cast<int>(Config_Terrain::islandSize)/4; i++) {
	// 	Model* tree = new Model(ModelLoader::getModel("tree"));
	// 	float posX = rand() % int(Config_Terrain::islandSize);
	// 	float posZ = rand() % int(Config_Terrain::islandSize);
	// 	PhysicsEntity* treeBody = new PhysicsEntity_Box(btVector3(1.0f, 5.0f, 1.0f), 30.0f, btVector3(posX, Bioms::getHight(posX, posZ), posZ));
    //     Entity* treeEnt = new Entity(tree, treeBody);
    //     treeEnt->setStaticFlag(true);
	// 	Entities::addEntity(treeEnt);
	// };

	// trees 1
	for (int i = 0; i < static_cast<int>(Config_Terrain::islandSize)/4; i++) {
		Model* tree = new Model(ModelLoader::getModel("tree1"));
		tree->setScale(glm::vec3(1.5f, 1.5f, 1.5f));
		tree->updateTransform();
		float posX = rand() % int(Config_Terrain::islandSize);
		float posZ = rand() % int(Config_Terrain::islandSize);
		PhysicsEntity* treeBody = new PhysicsEntity_Box(btVector3(1.0f, 5.0f, 1.0f), 0.0f, btVector3(posX, Bioms::getHight(posX, posZ), posZ));
        Entity* treeEnt = new Entity(tree, treeBody);
        treeEnt->setStaticFlag(true);
		Entities::addEntity(treeEnt);
	};

	// glocs
	// for (int i = 0; i < 100; i++) {
	// 	Model* glocs = new Model(ModelLoader::getModel("gloc"));
	// 	glocs->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	// 	glocs->updateTransform();
	// 	PhysicsEntity* glocsBody = new PhysicsEntity_Box(btVector3(0.1f, 0.2f, 0.5f), 30.0f, btVector3(rand() % 100, 10.0f, rand() % 100));
	// 	Entities::addEntity(new Entity(glocs, glocsBody));
	// };

	// ak47
	// Model* ak47 = new Model(ModelLoader::getModel("ak47"));
	// PhysicsEntity* ak47Body = new PhysicsEntity_Box(btVector3(0.2f, 0.8f, 3.5f), 500.0f, btVector3(-10.0f, 10.0f, -20.0f));
	// Entities::addEntity(new Entity(ak47, ak47Body));

    // charachter
    Model* characther = new Model(ModelLoader::getModel("char"));
    PhysicsEntity* charBody = new PhysicsEntity_Box(btVector3(0.5f, 0.0f, 0.5f), 30.0f, btVector3(-20.0f, 1.0f, -20.0f));
    Entities::addEntity(new Entity(characther, charBody));

	// test cube
	Model* cube = new Model(ModelLoader::getModel("cube"));
	cube->meshGroups[0]->material.diffuse = glm::vec3(2.0f, 2.0f, 2.0f);
	cube->meshGroups[0]->material.diffuseMap = TextureLoader::getTexture("containerDefuse");
	cube->meshGroups[0]->material.specularMap = TextureLoader::getTexture("containerSpecular");
	// cube->meshGroups[0]->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	// cube->meshGroups[0]->material.diffuseMap = freeCam.getFrameTexture();
	// cube->meshGroups[0]->material.specularMap = freeCam.getFrameTexture();
	PhysicsEntity* cubeBody = new PhysicsEntity_Box(btVector3(1.0f, 1.0f, 1.0f), 50.0f, btVector3(5.0f, 1.0f, 0.0f));
	Entities::addEntity(new Entity(cube, cubeBody));

	// test suzanne
	Model* suzanne = new Model(ModelLoader::getModel("suzanne"));
	PhysicsEntity* suzanneBody = new PhysicsEntity_Box(btVector3(1.0f, 1.0f, 1.0f), 1.0f, btVector3(5.0f, 10.0f, -10.0f));
	Entities::addEntity(new Entity(suzanne, suzanneBody));

    //----------------------------------------------------------------------------
	while (!glfwWindowShouldClose(display.window)) {
		// calculate delta time
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//-----------------------------------------------------------------------
		// light test
		Light::createDirectionalLight(Geometry::polarToCartezian(1.0f, glfwGetTime() * 30, 45), glm::vec3(1.0f, 1.0f, 1.0f));

		// test::spaceBarPressed();

		//-----------------------------------------------------------------------
		//---------------- update inputs ------------------
		// camera movement
		Camera::inputUpdate(deltaTime);
		cameraPos = Camera::getPos();

		//---------------- update physics ------------------
		// update physics simulation
		// Debug(Physics::update(deltaTime))
		Physics::update(deltaTime);

		// update terrain
		Terrain::update(cameraPos.x, cameraPos.z);

		// update entities
		Entities::update();

		//------------------------- render --------------------------------------
		//-----------------------------------------------------------------------
		// shadow depth frame
		glBindFramebuffer(GL_FRAMEBUFFER, Light::getDepthFrame());
		glViewport(0, 0, Light::shadowMapResolution, Light::shadowMapResolution);

		// recalculate light space matrix to camera pos
		Light::recalculateLightSpaceMat(Camera::getFrustumWorld());

		// render scean
		renderSceanShadow();

		// set shadowmap uniform
		Light::setShadowMapUniform();
/*
		//----------------------------------------------------------------------
		// camera frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, freeCam.getFrameBuffer());
		glViewport(0, 0, Display::windowWidth, Display::windowHeight);

		// render scean
		renderScean();
*/
		// -----------------------------------------------------------------------
		// reset main buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Display::windowWidth, Display::windowHeight);

		renderScean();
		// Debug(renderScean())

		// draw frame quad
		// frameQuad->renderFrame(freeCam.getFrameTexture());
		// frameQuad->renderFrame(Light::getDepthMap());

		// update GUI
		GUI::update(deltaTime);

		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		// swap openGL buffers
    	glfwSwapBuffers(display.window);

		// handle glfw events
        glfwPollEvents();
	};

	Entities::disposeAllEntities();
	Terrain::dispose();
	Physics::dispose();
	// glfwTerminate();

	return 0;
};
void renderScean() {
	// clear openGL back buffer
	glClearColor(0.661f, 0.720f, 0.959f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// debug draw
	PhysicsDebugDrawer::render();

	// render terrain
	Terrain::render();

	// render entities
	Entities::render();
};
void renderSceanShadow() {
	glEnable(GL_DEPTH_TEST);

	// clear openGL depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// render terrain FIXME: use shadow shader first
	Terrain::renderShadow();

	// render entities
	Entities::renderShadows();

	glCullFace(GL_BACK);
};

//-----------------------------------------------------------------------------
Model* createPlane() {
	float resolution = (float)Display::windowHeight / (float)Display::windowWidth;

	// init model
	Model* plane = new Model();

	float vertices[] = {
		resolution * -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
        resolution * -1.0f, -1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
		resolution *  1.0f, -1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,

		resolution * -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
        resolution *  1.0f, -1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
		resolution *  1.0f,  1.0f,  1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f
    };

	unsigned int nbrtVerts = 3 * 2;
	unsigned int indices[nbrtVerts];
	for (int i = 0; i < nbrtVerts; i++) {
		plane->addVertex(
			glm::vec3(vertices[i*8], vertices[i*8+1], vertices[i*8+2]),
			glm::vec3(vertices[i*8+3], vertices[i*8+4], vertices[i*8+5]),
			glm::vec2(vertices[i*8+6], vertices[i*8+7])
		);

		indices[i] = i;
	};
	plane->loadVertecies(indices, nbrtVerts, Material());

	return plane;
};
Model* createFrustum(glm::vec4* frustumPos) {
	// init model
	Model* frustum = new Model();

	float vertices[] = {
        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        frustumPos[1].x, frustumPos[1].y, frustumPos[1].z,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        frustumPos[0].x, frustumPos[0].y, frustumPos[0].z,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        frustumPos[0].x, frustumPos[0].y, frustumPos[0].z,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        frustumPos[2].x, frustumPos[2].y, frustumPos[2].z,    0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

        frustumPos[7].x, frustumPos[7].y, frustumPos[7].z,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        frustumPos[6].x, frustumPos[6].y, frustumPos[6].z,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        frustumPos[5].x, frustumPos[5].y, frustumPos[5].z,    0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
        frustumPos[7].x, frustumPos[7].y, frustumPos[7].z,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

        frustumPos[5].x, frustumPos[5].y, frustumPos[5].z,   -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[1].x, frustumPos[1].y, frustumPos[1].z,   -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,   -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,   -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[7].x, frustumPos[7].y, frustumPos[7].z,   -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        frustumPos[5].x, frustumPos[5].y, frustumPos[5].z,   -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,

        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[6].x, frustumPos[6].y, frustumPos[6].z,    1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        frustumPos[2].x, frustumPos[2].y, frustumPos[2].z,    1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[2].x, frustumPos[2].y, frustumPos[2].z,    1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[0].x, frustumPos[0].y, frustumPos[0].z,    1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    1.0f,  0.0f,  0.0f,     0.0f,  1.0f,

        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[2].x, frustumPos[2].y, frustumPos[2].z,    0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        frustumPos[6].x, frustumPos[6].y, frustumPos[6].z,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[6].x, frustumPos[6].y, frustumPos[6].z,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[7].x, frustumPos[7].y, frustumPos[7].z,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
        frustumPos[3].x, frustumPos[3].y, frustumPos[3].z,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,

        frustumPos[1].x, frustumPos[1].y, frustumPos[1].z,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        frustumPos[5].x, frustumPos[5].y, frustumPos[5].z,    0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[4].x, frustumPos[4].y, frustumPos[4].z,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        frustumPos[0].x, frustumPos[0].y, frustumPos[0].z,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
        frustumPos[1].x, frustumPos[1].y, frustumPos[1].z,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f
    };

	unsigned int nbrtVerts = 6 * 6;
	unsigned int indices[nbrtVerts];
	for (int i = 0; i < nbrtVerts; i++) {
		frustum->addVertex(
			glm::vec3(vertices[i*8], vertices[i*8+1], vertices[i*8+2]),
			glm::vec3(vertices[i*8+3], vertices[i*8+4], vertices[i*8+5]),
			glm::vec2(vertices[i*8+6], vertices[i*8+7])
		);

		indices[i] = i;
	};
	frustum->loadVertecies(indices, nbrtVerts, Material());

	return frustum;
};
void createCubes(int nbr) {
	for (int i = 0; i < nbr; i++) {
		Model* model = new Model(ModelLoader::getModel("cube"));

		glm::vec3 pos = glm::vec3(
			int(Config_Terrain::islandSize) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
			int(Config_Terrain::islandSize) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
			int(Config_Terrain::islandSize) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))
		);
		/*
		cubes[i]->setRot(
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 180,
			glm::vec3(
				static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
				static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
				static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
			)
		);
		*/
		// float size = 5 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		glm::vec3 size = glm::vec3(
			5 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
			5 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
			5 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
		);
		model->setScale(
			glm::vec3(
				size.x,
				size.y,
				size.z
			)
		);
		model->updateTransform();

		// enable depth testing shader
		// model->setShaderProgram(ShaderLoader::getShader("Select"));

		// init physics
		PhysicsEntity* body = new PhysicsEntity_Box(btVector3(size.x, size.y, size.z), glm::length(size) * 30, btVector3(pos.x, pos.y, pos.z));

		// set material
		glm::vec3 color = glm::vec3(
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
		);

		model->meshGroups[0]->material.ambient = color;
		model->meshGroups[0]->material.diffuse = color;
		model->meshGroups[0]->material.specular = color;

		// create entity
		Entity* cube = new Entity(model, body);

		// push to cubes vector
		Entities::addEntity(cube);

		// add to cubes list
		cubes.push_back(cube);
	};
};
void deleteCubes() {
	// Entities::disposeAllEntities();
	for (int i = 0; i < cubes.size(); i++) {
		Entities::disposeEntity(cubes[i]);
	};
};

//-----------------------------------------------------------------------------
