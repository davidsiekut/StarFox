#pragma once

#include "UIElement.h"

class WinnerImage : public UIElement
{
public:
	WinnerImage(glm::vec3 position, glm::vec3 size);
	~WinnerImage();
	virtual void Update(float dt);

protected:

private:
};
