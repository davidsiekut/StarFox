#pragma once

#include <string>
#include <map>

class LSystem
{
public:
	LSystem();
	~LSystem();

	std::string Parse(int iterations, std::string root);

	void AddRule(std::string LHS, std::string RHS);

private:
	std::map<std::string, std::string> rules;
};
