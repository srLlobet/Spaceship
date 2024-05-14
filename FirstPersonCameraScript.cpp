#include "FirstPersonCameraScript.h"
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void FirstPersonCameraScript::startScript() {

}

void FirstPersonCameraScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;
	glm::vec3 rotation(0.f, 0.01f, 0.f);
	float angle = 0.01f;
	
	ComponentHandle<Camera> cam = entity->get<Camera>();

	glm::vec3 currentPosition = cam->position;
	glm::vec3 currentOrientation = cam->orientation;
	glm::vec3 desiredOrientation = cam->orientation;
	glm::vec3 desiredPosition = cam->position;


	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);

		glm::mat3 rotationMatrix(cosAngle, 0, -sinAngle,
									0, 1, 0,
								sinAngle, 0, cosAngle);

		desiredOrientation = rotationMatrix * desiredOrientation;
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up));

	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		float cosAngle = cos(-angle);
		float sinAngle = sin(-angle);

		// Compute the rotation matrix around the camera's up vector
		glm::mat3 rotationMatrix(cosAngle, 0, -sinAngle,
			0, 1, 0,
			sinAngle, 0, cosAngle);

		desiredOrientation = rotationMatrix * desiredOrientation;
		desiredPosition += (speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up)));


	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);

		// Compute the rotation matrix around the camera's right vector
		glm::mat3 verticalRotationMatrix(glm::vec3(1, 0, 0),
			glm::vec3(0, cosAngle, -sinAngle),
			glm::vec3(0, sinAngle, cosAngle));

		desiredOrientation = verticalRotationMatrix * desiredOrientation;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		float cosAngle = cos(-angle);
		float sinAngle = sin(-angle);

		glm::mat3 verticalRotationMatrix(glm::vec3(1, 0, 0),
			glm::vec3(0, cosAngle, -sinAngle),
			glm::vec3(0, sinAngle, cosAngle));

		desiredOrientation = verticalRotationMatrix * desiredOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * cam->orientation;
		
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		desiredPosition -= speedDelta * cam->orientation;
	}
	
	cam->orientation = desiredOrientation;
	cam->position = desiredPosition;

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

		glm::vec3 pos = ent->get<Transform3D>()->position;

		//Desired position inside cube
		if (desiredPosition.x < pos.x + cubeColl->width && desiredPosition.x > pos.x - cubeColl->width &&
			desiredPosition.y < pos.y + cubeColl->height && desiredPosition.y > pos.y - cubeColl->height &&
			desiredPosition.z < pos.z + cubeColl->length && desiredPosition.z > pos.z - cubeColl->length) {

			time_t result = time(NULL);

			char str[26];
			ctime_s(str, sizeof str, &result);

			if (currentPosition.x <= pos.x - cubeColl->width) desiredPosition.x = pos.x - cubeColl->width;
			if (currentPosition.x >= pos.x + cubeColl->width) desiredPosition.x = pos.x + cubeColl->width;
			if (currentPosition.z <= pos.z - cubeColl->length) desiredPosition.z = pos.z - cubeColl->length;
			if (currentPosition.z >= pos.z + cubeColl->length) desiredPosition.z = pos.z + cubeColl->length;
			if (currentPosition.y <= pos.y - cubeColl->height) desiredPosition.y = pos.y - cubeColl->height;
			if (currentPosition.y >= pos.y + cubeColl->height) desiredPosition.y = pos.y + cubeColl->height;
		}

	});



	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(cam->orientation, cam->up)));

		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), cam->up);

		cam->orientation = m * m2 * glm::vec4(cam->orientation, 1.);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

}