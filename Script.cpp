#include "Script.h"

Script::Script() {

}

Script::Script(GLFWwindow* window, Entity* entity) {
	this->window = window;
	this->entity = entity;
}


Script::Script(Entity* entityFollow, Entity* entity)
{
	this->entityFollow = entityFollow;
	this->entity = entity;
}

Script::Script(GLFWwindow* window, World* world, Entity* entity) {
	this->window = window;
	this->entity = entity;
	this->world = world;
}

void Script::startScript()
{
	
	cout << "start" << endl;
	
}

void Script::tickScript(float deltaTime)
{
	cout << "update" << endl;
}

void Script::Delete() {
	delete this;
}