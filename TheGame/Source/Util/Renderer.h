#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

struct GLFWwindow;

enum ShaderType
{
	SHADER_TEST,
	NUM_SHADERS
};

class Renderer
{
public:
	static void Initialize();

private:
	static GLuint LoadShader(std::string name);
	static std::string LoadFromFile(const std::string filename);

	static GLFWwindow* w;
	static std::vector<unsigned int> shaders;
	static unsigned int currentShader;
};