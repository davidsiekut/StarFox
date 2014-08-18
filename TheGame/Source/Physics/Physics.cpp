#include "Physics.h"

bool Physics::CheckAABBCollision(Entity* b1, Entity* b2)
{
	if (b1 == b2)
		return false;

	glm::vec3 c1 = b1->GetCollider();
	glm::vec3 c2 = b2->GetCollider();

	glm::vec3 min1 = glm::vec3(
		b1->GetPositionWorld().x - c1.x / 2,
		b1->GetPositionWorld().y - c1.y / 2,
		b1->GetPositionWorld().z - c1.z / 2);

	glm::vec3 max1 = glm::vec3(
		b1->GetPositionWorld().x + c1.x / 2,
		b1->GetPositionWorld().y + c1.y / 2,
		b1->GetPositionWorld().z + c1.z / 2);

	glm::vec3 min2 = glm::vec3(
		b2->GetPositionWorld().x - c2.x / 2,
		b2->GetPositionWorld().y - c2.y / 2,
		b2->GetPositionWorld().z - c2.z / 2);

	glm::vec3 max2 = glm::vec3(
		b2->GetPositionWorld().x + c2.x / 2,
		b2->GetPositionWorld().y + c2.y / 2,
		b2->GetPositionWorld().z + c2.z / 2);

	return(max1.x > min2.x &&
		min1.x < max2.x &&
		max1.y > min2.y &&
		min1.y < max2.y &&
		max1.z > min2.z &&
		min1.z < max2.z);
}