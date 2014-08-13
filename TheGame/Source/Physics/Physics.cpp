#include "Physics.h"

bool Physics::CheckAABBCollision(Entity* b1, Entity* b2)
{
	if (b1 == b2)
		return false;

	glm::vec3 min1 = glm::vec3(
		b1->GetPositionWorld().x - b1->COLLIDE_X / 2,
		b1->GetPositionWorld().y - b1->COLLIDE_Y / 2,
		b1->GetPositionWorld().z - b1->COLLIDE_Z / 2);

	glm::vec3 max1 = glm::vec3(
		b1->GetPositionWorld().x + b1->COLLIDE_X / 2,
		b1->GetPositionWorld().y + b1->COLLIDE_Y / 2,
		b1->GetPositionWorld().z + b1->COLLIDE_Z / 2);

	glm::vec3 min2 = glm::vec3(
		b2->GetPositionWorld().x - b2->COLLIDE_X / 2,
		b2->GetPositionWorld().y - b2->COLLIDE_Y / 2,
		b2->GetPositionWorld().z - b2->COLLIDE_Z / 2);

	glm::vec3 max2 = glm::vec3(
		b2->GetPositionWorld().x + b2->COLLIDE_X / 2,
		b2->GetPositionWorld().y + b2->COLLIDE_Y / 2,
		b2->GetPositionWorld().z + b2->COLLIDE_Z / 2);

	return(max1.x > min2.x &&
		min1.x < max2.x &&
		max1.y > min2.y &&
		min1.y < max2.y &&
		max1.z > min2.z &&
		min1.z < max2.z);
}