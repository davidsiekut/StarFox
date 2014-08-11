#pragma once

#include "BuildingFactory.h"
#include "Scene.h"
#include "MirroredCubes.h"
#include "Cube.h"
#include "Chunk.h"

BuildingFactory::BuildingFactory()
{

}

void BuildingFactory::GenerateBuilding(glm::vec3 position)
{
	int s = (rand() % 40) - 20;
	// add level geometry to this chunk
	//Cube* u = new Cube(NULL, glm::vec3(5.f, 20.f, 5.f));

	float x = (rand() % 4 + 1.f) / 4.f;
	float y = (rand() % 4 + 1.f) / 4.f;

	MirroredCubes* u = new MirroredCubes(NULL);
	u->SetPosition(glm::vec3(s, 10.f, position.z + Chunk::CHUNK_DEPTH/2));
	u->SetScaleOfCubes(x, y);
	Scene::GetInstance().AddEntity(u);

	Cube* c = new Cube(NULL, glm::vec3(3.f, 20.f, 5.f));
	c->SetPosition(glm::vec3(s, 10.f, position.z + Chunk::CHUNK_DEPTH/2));
	Scene::GetInstance().AddEntity(c);
	//printf("[Building] Creating cube at (%f, %f, %f)\n", u->GetPositionWorld().x, u->GetPositionWorld().y, u->GetPositionWorld().z);
}