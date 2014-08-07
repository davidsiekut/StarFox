#include "Renderer.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

GLFWwindow* Renderer::w = NULL;
std::vector<unsigned int> Renderer::shaders;

void Renderer::Initialize()
{
	w = WindowManager::GetWindow();
	glfwMakeContextCurrent(w);

	/*
	GLEW obtains information on the supported extensions from the graphics driver. Experimental
	or pre-release drivers, however, might not report every available extension through the
	standard mechanism, in which case GLEW will report it unsupported. To circumvent this situation,
	the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling
	glewInit(), which ensures that all extensions with valid entry points will be exposed.
	*/
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "[Renderer] Error in glewInit()\n");
		exit(-1);
	}

	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	shaders.push_back(LoadShader("SolidColor"));
}

GLuint Renderer::LoadShader(std::string name)
{
	//http://www.opengl.org/wiki/Shader_Compilation

	printf("[Renderer] Loading shader '%s'\n", (const GLchar *)name.c_str());

	//Read our shaders into the appropriate buffers
	std::string vertexSource = LoadFromFile("../Source/Shaders/" + name + ".vert"); //Get source code for vertex shader.
	std::string fragmentSource = LoadFromFile("../Source/Shaders/" + name + ".frag"); //Get source code for fragment shader.

	//Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Send the vertex shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	const GLchar *source = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	printf("[Renderer] Compiling vertex...\n");
	//Compile the vertex shader
	glCompileShader(vertexShader);

	int Result;
	GLint InfoLogLength;

	// Check Vertex Shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("[Renderer] %s\n", &VertexShaderErrorMessage[0]);
	}

	//Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Send the fragment shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	printf("[Renderer] Compiling fragment...\n");
	//Compile the fragment shader
	glCompileShader(fragmentShader);

	// Check Fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("[Renderer] %s\n", &FragmentShaderErrorMessage[0]);
	}

	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	GLuint program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	printf("[Renderer] Linking...\n");
	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		//return;
		getchar();
		exit(-1);
	}
	printf("[Renderer] Detaching...\n");
	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

std::string Renderer::LoadFromFile(const std::string filename)
{
	std::ifstream file;
	file.open(filename.c_str());

	if (!file.good())
	{
		printf("[Renderer] Error loading shader file\n");
		getchar();
		exit(-1);
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	std::string source = stream.str();

	return source;
}

void Renderer::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame()
{
	glfwSwapBuffers(w);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		char* error;
		switch (err)
		{
		case GL_INVALID_ENUM:					error = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:					error = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:				error = "GL_INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		case GL_OUT_OF_MEMORY:					error = "GL_OUT_OF_MEMORY"; break;
		case GL_STACK_UNDERFLOW:				error = "GL_STACK_UNDERFLOW"; break;
		case GL_STACK_OVERFLOW:					error = "GL_STACK_OVERFLOW"; break;
		default:								error = "UNKNOWN_ERROR"; break;
		}
		fprintf(stderr, "OpenGL Error %s\n", error);
	}
}

void Renderer::Shutdown()
{
	// Shaders
	for (std::vector<unsigned int>::iterator it = shaders.begin(); it < shaders.end(); ++it)
	{
		glDeleteProgram(*it);
	}
	shaders.clear();

	// Managed by WindowManager
	w = NULL;
}