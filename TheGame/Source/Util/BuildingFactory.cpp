#pragma once

#include "BuildingFactory.h"
#include "Scene.h"
#include "MirroredCubes.h"
#include "FilledMirroredCubes.h"
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

	FilledMirroredCubes* u = new FilledMirroredCubes(NULL);
	u->SetPosition(glm::vec3(s, 0.f, position.z + Chunk::CHUNK_DEPTH / 2));
	Scene::GetInstance().AddEntity(u);
	//printf("[Building] Creating cube at (%f, %f, %f)\n", u->GetPositionWorld().x, u->GetPositionWorld().y, u->GetPositionWorld().z);
}