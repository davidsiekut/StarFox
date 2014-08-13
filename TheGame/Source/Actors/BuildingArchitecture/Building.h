#pragma once

#include "Entity.h"

#define BUILDING_SIZE_X 10.f
#define BUILDING_SIZE_Y 60.f
#define BUILDING_SIZE_Z 10.f
#define BUILDING_SINK_SPEED 25.f
#define BUILDING_SHAKE_AMPLITUDE 0.2f
#define BUILDING_SHAKE_SPEED 50.f

class Building : public Entity
{
public:
	Building(Entity* parent, std::string lSystem);

	virtual void Draw();
	virtual void Update(float dt);
	void OnCollision(Entity* other);

	void SetPosition(glm::vec3 position);

private:
	void Parse(char c);

	float timeElapsed = 0.f;
	float verticalAxis;
	std::vector<Entity*> blocks;
};