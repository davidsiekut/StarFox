//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
// Quang Tran
//

#pragma once

#include "BuildingFactory.h"
#include "Building.h"
#include "Scene.h"
#include "Chunk.h"

#define BUILD_RANGE 100

BuildingFactory::BuildingFactory()
{
	grammar.AddRule("B", "BsM");
	grammar.AddRule("M", "rsM");
}

void BuildingFactory::GenerateBuilding(glm::vec3 position)
{
	/*for (int i = -BUILD_RANGE + (rand() % 30); i < BUILD_RANGE; i += 40)
	{
		if (i > 2 || i < -2)
		{*/
			int i = rand() % BUILD_RANGE - BUILD_RANGE/2;
			int iterations = rand() % 5;
			std::string system = grammar.Run(iterations, "B");
			Building* building = new Building(NULL, system);
			building->SetPosition(glm::vec3(i, 0.f, position.z + Chunk::CHUNK_DEPTH / 2));
			Scene::GetInstance().AddEntity(building);
			//printf("[Building] Creating cube at (%f, %f, %f)\n", building->GetPositionWorld().x, building->GetPositionWorld().y, building->GetPositionWorld().z);
		/*}
	}*/
}