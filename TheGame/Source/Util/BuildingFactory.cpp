#pragma once

#include "BuildingFactory.h"
#include "Building.h"
#include "Scene.h"
#include "Chunk.h"

BuildingFactory::BuildingFactory()
{
	grammar.AddRule("B", "BsM");
	grammar.AddRule("M", "rsM");
}

void BuildingFactory::GenerateBuilding(glm::vec3 position)
{
	int x = (rand() % 40) - 20;
	// add level geometry to this chunk
	int iterations = rand() % 5;

	std::string system = grammar.Run(iterations, "B");
	Building* building = new Building(NULL, system);
	building->SetPosition(glm::vec3(x, 0.f, position.z + Chunk::CHUNK_DEPTH / 2));
	Scene::GetInstance().AddEntity(building);
	//printf("[Building] Creating cube at (%f, %f, %f)\n", u->GetPositionWorld().x, u->GetPositionWorld().y, u->GetPositionWorld().z);
}