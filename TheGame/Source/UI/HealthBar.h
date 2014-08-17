#pragma once

#include "UIElement.h"

class HealthBar : public UIElement
{
public:
	HealthBar(glm::vec3 position, glm::vec3 size, std::string entityType);
	~HealthBar();
	virtual void Update(float dt);
	std::string entityType;

protected:

private:
};
