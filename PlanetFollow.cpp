#include "PlanetFollow.h"
#include <chrono>


void PlanetFollow::startScript() {

}

void PlanetFollow::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;


	ComponentHandle<Transform3D> planetFollowTransform = entityFollow->get<Transform3D>();
	ComponentHandle<Transform3D> planetTransform = entity->get<Transform3D>();

	glm::vec3 positionFollow = planetFollowTransform->position;
	glm::vec3 desiredPosition = planetTransform->position;

	desiredPosition.x = positionFollow.x + 25;
	desiredPosition.y = -positionFollow.y + 5;
	desiredPosition.z = positionFollow.z + 5;

	planetTransform->position = desiredPosition;
}

