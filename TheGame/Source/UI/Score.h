#pragma once

#include "UIElement.h"

class Score : public UIElement
{
public:
	Score(glm::vec3 position, glm::vec3 size);
	~Score();
	virtual void Update(float dt);

protected:

private:
};
