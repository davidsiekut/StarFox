#pragma once

#include "LSystem.h"

LSystem::LSystem()
{

}

LSystem::~LSystem()
{

}

std::string LSystem::Run(int iterations, std::string root)
{
	std::string finalValue = root;
	for (int i = 0; i < iterations; i++)
	{
		finalValue = "";
		for (unsigned int j = 0; j < root.size(); j++)
		{
			std::string current = root.substr(j, 1);
			if (rules.find(current) != rules.end())
			{
				finalValue += rules[current];
			}
		}
		root = finalValue;
	}

	return finalValue;
}

void LSystem::AddRule(std::string LHS, std::string RHS)
{
	rules.insert(std::pair<std::string, std::string>(LHS, RHS));
}