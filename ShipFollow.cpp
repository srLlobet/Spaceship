#include "ShipFollow.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


#include <chrono>


void ShipFollow::startScript() {

}



void ShipFollow::tickScript(float deltaTime) {
    float speedDelta = speed * deltaTime;

    ComponentHandle<Camera> camera = entityFollow->get<Camera>();
    ComponentHandle<Transform3D> shipTransform = entity->get<Transform3D>();



    glm::vec3 positionFollow = camera->position;
    glm::vec3 orientationFollow = camera->orientation;
    

    shipTransform->position = positionFollow - glm::vec3(0, 3, -10);

    glm::vec3 forwardDirection = -orientationFollow ;
    glm::vec3 upDirection = glm::normalize(camera->up);


    glm::vec3 rightDirection = glm::normalize(glm::cross(forwardDirection, upDirection));
    glm::vec3 newUpDirection = upDirection;
    glm::mat4 rotationMatrix = glm::mat4(glm::vec4(rightDirection, 0.0f),
        glm::vec4(newUpDirection, 0.0f),
        glm::vec4(-forwardDirection, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));


    // Calculate the translation matrix
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), shipTransform->position);
    

    // Calculate the model matrix
    shipTransform->modelMatrix = translationMatrix * rotationMatrix;
}


