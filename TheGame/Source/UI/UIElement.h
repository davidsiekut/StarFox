#pragma once

#include "Entity.h"

class UIElement : public Entity
{
public:
	UIElement();
	~UIElement();
	virtual glm::mat4 GetWorldMatrix() const;

protected:

private:
};
