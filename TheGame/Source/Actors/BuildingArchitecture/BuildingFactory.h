#pragma once

#include <glm/glm.hpp>

class BuildingFactory
{
public:

	static BuildingFactory& GetInstance()
	{
		static BuildingFactory instance;
		return instance;
	}

	void GenerateBuilding(glm::vec3 position);

private:
	// Hide constructors
	BuildingFactory();
	BuildingFactory(BuildingFactory const&);
};