#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

struct GLFWwindow;

enum ShaderType
{
	SHADER_SOLID_COLOR,
	NUM_SHADERS
};

class Renderer
{
public:
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
	static unsigned int GetShaderProgramID(ShaderType type) { return shaders[static_cast<int>(type)]; }

private:
	static GLuint LoadShader(std::string name);
	static std::string LoadFromFile(const std::string filename);

	static GLFWwindow* w;
	static std::vector<unsigned int> shaders;

	static GLuint Renderer::LoadShaders(const char * vertex_shader_path, const char * fragment_shader_path);
};