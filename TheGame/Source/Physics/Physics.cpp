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

bool Physics::CheckOBBCollision(Entity *b1, Entity *b2)
{
	OBB a = b1->obb;
	OBB b = b2->obb;

	glm::mat3 rotationMatrix, rotationMatrixAbs;
	float ra, rb;

	// rotation matrix (b in a’s coord frame)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrix[i][j] = glm::dot(a.u[i], b.u[j]);
		}
	}

	// translation
	glm::vec3 t = b.c - a.c;
	//glm::vec3 t = b2->GetPositionWorld() - b1->GetPositionWorld();

	// translation (a’s coord frame)
	t = glm::vec3(glm::dot(t, a.u[0]), glm::dot(t, a.u[2]), glm::dot(t, a.u[2]));

	// absolute values that will be reused often
	// FLT_EPSILON is to offset any errors if there are two parallel
	// edges whose cross product is almost 0
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrixAbs[i][j] = glm::abs(rotationMatrix[i][j]) + FLT_EPSILON;
		}
	}

	// perform all our seperating axis tests

	// axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++)
	{
		ra = a.e[i];
		rb = b.e[0] * rotationMatrixAbs[i][0] + b.e[1] * rotationMatrixAbs[i][1] + b.e[2] * rotationMatrixAbs[i][2];
		if (glm::abs(t[i]) > ra + rb)
			return false;
	}

	// axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		ra = a.e[0] * rotationMatrixAbs[0][i] + a.e[1] * rotationMatrixAbs[1][i] + a.e[2] * rotationMatrixAbs[2][i];
		rb = b.e[i];
		if (glm::abs(t[0] * rotationMatrix[0][i] + t[1] * rotationMatrix[1][i] + t[2] * rotationMatrix[2][i]) > ra + rb)
			return false;
	}

	// axis L = A0 x B0
	ra = a.e[1] * rotationMatrixAbs[2][0] + a.e[2] * rotationMatrixAbs[1][0];
	rb = b.e[1] * rotationMatrixAbs[0][2] + b.e[2] * rotationMatrixAbs[0][1];
	if (glm::abs(t[2] * rotationMatrix[1][0] - t[1] * rotationMatrix[2][0]) > ra + rb)
		return false;

	// axis L = A0 x B1
	ra = a.e[1] * rotationMatrixAbs[2][1] + a.e[2] * rotationMatrixAbs[1][1];
	rb = b.e[0] * rotationMatrixAbs[0][2] + b.e[2] * rotationMatrixAbs[0][0];
	if (glm::abs(t[2] * rotationMatrix[1][1] - t[1] * rotationMatrix[2][1]) > ra + rb)
		return false;
	
	// axis L = A0 x B2
	ra = a.e[1] * rotationMatrixAbs[2][2] + a.e[2] * rotationMatrixAbs[1][2];
	rb = b.e[0] * rotationMatrixAbs[0][1] + b.e[1] * rotationMatrixAbs[0][0];
	if (glm::abs(t[2] * rotationMatrix[1][2] - t[1] * rotationMatrix[2][2]) > ra + rb)
		return false;
	
	// axis L = A1 x B0
	ra = a.e[0] * rotationMatrixAbs[2][0] + a.e[2] * rotationMatrixAbs[0][0];
	rb = b.e[1] * rotationMatrixAbs[1][2] + b.e[2] * rotationMatrixAbs[1][1];
	if (glm::abs(t[0] * rotationMatrix[2][0] - t[2] * rotationMatrix[0][0]) > ra + rb)
		return false;
	
	// axis L = A1 x B1
	ra = a.e[0] * rotationMatrixAbs[2][1] + a.e[2] * rotationMatrixAbs[0][1];
	rb = b.e[0] * rotationMatrixAbs[1][2] + b.e[2] * rotationMatrixAbs[1][0];
	if (glm::abs(t[0] * rotationMatrix[2][1] - t[2] * rotationMatrix[0][1]) > ra + rb)
		return false;
	
	// axis L = A1 x B2
	ra = a.e[0] * rotationMatrixAbs[2][2] + a.e[2] * rotationMatrixAbs[0][2];
	rb = b.e[0] * rotationMatrixAbs[1][1] + b.e[1] * rotationMatrixAbs[1][0];
	if (glm::abs(t[0] * rotationMatrix[2][2] - t[2] * rotationMatrix[0][2]) > ra + rb)
		return false;
	
	// axis L = A2 x B0
	ra = a.e[0] * rotationMatrixAbs[1][0] + a.e[1] * rotationMatrixAbs[0][0];
	rb = b.e[1] * rotationMatrixAbs[2][2] + b.e[2] * rotationMatrixAbs[2][1];
	if (glm::abs(t[1] * rotationMatrix[0][0] - t[0] * rotationMatrix[1][0]) > ra + rb)
		return false;
	
	// axis L = A2 x B1
	ra = a.e[0] * rotationMatrixAbs[1][1] + a.e[1] * rotationMatrixAbs[0][1];
	rb = b.e[0] * rotationMatrixAbs[2][2] + b.e[2] * rotationMatrixAbs[2][0];
	if (glm::abs(t[1] * rotationMatrix[0][1] - t[0] * rotationMatrix[1][1]) > ra + rb)
		return false;
	
	// axis L = A2 x B2
	ra = a.e[0] * rotationMatrixAbs[1][2] + a.e[1] * rotationMatrixAbs[0][2];
	rb = b.e[0] * rotationMatrixAbs[2][1] + b.e[1] * rotationMatrixAbs[2][0];
	if (glm::abs(t[1] * rotationMatrix[0][2] - t[0] * rotationMatrix[1][2]) > ra + rb)
		return false;
	
	// no seperating axis found, there is intersection
	return true;
}