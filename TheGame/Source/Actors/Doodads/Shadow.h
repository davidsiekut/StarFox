#pragma once

#include "Entity.h"

class Shadow : public Entity
{
public:
	Shadow(Entity &parent, std::string objPath);	

	virtual void Update(float dt);
	virtual void Draw();
	virtual bool IsOpaque() { return false; }

};