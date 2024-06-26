#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "FirstPersonCameraScript.h"
#include "ShipFollow.h"
#include "Script.h"

#include "ECS.h"
#include "SkyboxScript.h"

using std::cout; 
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 1000;
const unsigned int height = 1000;

float t = 0;
time_t current_time;

World* world;

void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 800x800
	window = glfwCreateWindow(1000, 1000, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity2D(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, 
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default") {
	Entity* ent = world->create();
	ent->assign<Transform2D>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return ent;
}

Entity* CreateEntity3DWithMesh(glm::vec3 position, glm::vec3 rotation, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) *
		rotationMatrix *
		glm::scale(glm::mat4(1.0f), glm::vec3(scale));


	ent->assign<Transform3D>(position, rotation, scale, modelMatrix);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}

Entity* CreateEntity3DEmpty() {
	Entity* ent = world->create();

	return ent;
}

Entity* CreateCamera(glm::vec3 position) {
	Entity* ent = world->create();
	ent->assign<Camera>(position, glm::vec3(0., -0.1, 1.), glm::vec3(0., 1., 0.));

	return ent;
}

Entity* CreateSkybox(const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Skybox>(texFilepath, meshFilepath);

	return ent;
}

void SetupWorld() {

	RenderSystem* rs = new RenderSystem(width, height);

	world = World::createWorld();
	world->registerSystem(rs);
	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();

	Entity* camera = CreateCamera(glm::vec3(0, 3, -10));

	FirstPersonCameraScript* fps = new FirstPersonCameraScript(window, world, camera);
	camera->assign<ScriptComponent>(scriptManager->AddScript(fps));


	rs->setCamera(camera);

	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/BackdropBlackLittleSparkBlack.png");

	Entity* radar = CreateEntity2D(glm::vec2(100., 100.), 0.f, 1.f, "Textures/radar1.png", glm::vec3(1., 1., 1.), false, glm::vec2(100., 100.));
	Entity* ship = CreateEntity3DWithMesh(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 2, "Meshes/ship.obj", "Textures/space.png");
	Entity* planet = CreateEntity3DWithMesh(glm::vec3(10, 25, 30), glm::vec3(0, 0, 0), 4, "Meshes/planet.obj", "Textures/shipmaterial2.png");
	Entity* planet2 = CreateEntity3DWithMesh(glm::vec3(1, 0, 20), glm::vec3(0,0,0), 4, "Meshes/planet.obj", "Textures/planet_preview.png");
	Entity* planet3 = CreateEntity3DWithMesh(glm::vec3(50, 10, 20), glm::vec3(0, 0, 0), 4, "Meshes/planet.obj", "Textures/shipmaterial2.png");
	Entity* planet4 = CreateEntity3DWithMesh(glm::vec3(23, -10, 50), glm::vec3(0, 0, 0), 4, "Meshes/planet.obj", "Textures/planet_preview.png");

	ShipFollow* pfs = new ShipFollow(camera, ship);
	ship->assign<ScriptComponent>(scriptManager->AddScript(pfs));

	ship->assign<CubeCollider>(5, 5, 5);
	planet->assign<CubeCollider>(5, 5, 5);
	planet2->assign<CubeCollider>(5, 5, 5);
	planet3->assign<CubeCollider>(5, 5, 5);
	planet4->assign<CubeCollider>(5, 5, 5);

	

}

int main() {
	
	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupWorld();

	float dt = 0;
	float time = clock();

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			world->tick(dt);
		}

		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}