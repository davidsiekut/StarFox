#pragma once

#include "Arwing.h"
#include "Cube.h"
#include "Entity.h"

class SkyBox : public Cube
{
public:
	SkyBox(Entity *parent, Arwing *arwing);
	~SkyBox();
	void Update(float dt);

	static const float SKYBOX_X;
	static const float SKYBOX_Y;

protected:

private:
	Arwing* arwing;
};