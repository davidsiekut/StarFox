#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

struct GLFWwindow;

enum ShaderType
{
	SHADER_SOLID_COLOR,
	SHADER_GOURAUD,
	SHADER_PHONG,
	SHADER_TEXTURED,
	SHADER_PHONG_TEXTURED,
	SHADER_BLOOM,
	SHADER_PARTICLES,
	SHADER_BLUR_VERTICAL,
	SHADER_BLUR_HORIZONTAL,
	SHADER_SHADOW,
	SHADER_INVISIBLE,
	NUM_SHADERS
};

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	void Initialize();
	void BeginFrame();
	void EndFrame();
	void Shutdown();
	int GetCurrentShader() { if (currentShader < 5) return currentShader; else return -1; }
	unsigned int GetShaderProgramID(int type) { return shaders[type]; }
	unsigned int GetShaderProgramID(ShaderType type) { return shaders[static_cast<int>(type)]; }
	void NextShader() { if (++currentShader >= NUM_SHADERS) currentShader = -1; };

private:
	// Hide constructors
	Renderer();
	Renderer(Renderer const&);

	GLuint LoadShader(std::string name);
	std::string LoadFromFile(const std::string filename);

	GLFWwindow* w;
	std::vector<unsigned int> shaders;
	int currentShader;
};