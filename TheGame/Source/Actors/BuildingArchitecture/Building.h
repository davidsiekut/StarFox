#pragma once

#include "Entity.h"

class Building : public Entity
{
public:
	Building(Entity* parent, std::string lSystem);

	virtual void Draw();

private:
	void Parse(char c);

	std::vector<Entity*> blocks;
};