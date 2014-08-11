#pragma once

#include "Entity.h"

class Cube : public Entity
{
public:
	Cube(Entity *parent, glm::vec3 size);
	~Cube();
	void Update(float dt);

protected:

private:

	static std::vector<Vertex>* bluePrint;
};