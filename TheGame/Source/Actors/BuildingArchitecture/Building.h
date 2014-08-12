#pragma once

#include "Entity.h"

#define BUILDING_SIZE_X 10.f
#define BUILDING_SIZE_Y 60.f
#define BUILDING_SIZE_Z 10.f

class Building : public Entity
{
public:
	Building(Entity* parent, std::string lSystem);

	virtual void Draw();

private:
	void Parse(char c);

	std::vector<Entity*> blocks;
};