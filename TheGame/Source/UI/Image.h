#pragma once

#include "UIElement.h"

class Image : public UIElement
{
public:
	Image(glm::vec3 position, glm::vec3 size);
	~Image();
	virtual void Update(float dt);

protected:

private:
};
