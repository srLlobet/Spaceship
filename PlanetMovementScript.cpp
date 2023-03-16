#include "PlanetMovementScript.h"
#include <chrono>


void PlanetMovementScript::startScript() {

}

void PlanetMovementScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;
	

	ComponentHandle<Transform3D> planetTransform = entity->get<Transform3D>();

	glm::vec3 currentPosition = planetTransform->position;
	glm::vec3 desiredPosition = planetTransform->position;

	if (desiredPosition.z <  (-10)) {

		planetTransform->position = (glm::vec3(10, 25, 30));
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			desiredPosition += speedDelta * -(0, 0, 0.5);
		}
		planetTransform->position = desiredPosition;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			desiredPosition += speedDelta * (0., 0, 0.5);
		}
		planetTransform->position = desiredPosition;
	}




}