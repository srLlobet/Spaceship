#pragma once

#include<iostream>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include "ECS.h"
#include "Components.h"

using namespace std;
using namespace ECS;

class Script
{
public:

    Script();

    Script(GLFWwindow* window, Entity* entity);

    Script(Entity* entityFollow, Entity* entity);

    Script(GLFWwindow* window, World* world,  Entity* entity);

    virtual void startScript();
    
    virtual void tickScript(float deltaTime);

    void Delete();

protected:
    Entity* entity;
    Entity* entityFollow;
    GLFWwindow* window;
    World* world;
};