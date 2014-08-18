#pragma once

#include "UIElement.h"

class Digit : public UIElement
{
public:
	Digit(glm::vec3 position, glm::vec3 size, int placeValue);
	~Digit();
	int Digit::GetDigit(int i);
	virtual void Update(float dt);
	int placeValue;

protected:

private:
};
