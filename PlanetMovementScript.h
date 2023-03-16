#pragma once

#include "Script.h"

using namespace std;

class PlanetMovementScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime);

private:


    float speed = 0.01f;

};