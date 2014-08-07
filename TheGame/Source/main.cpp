#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "Scene.h"
#include "sphere.h"

#include <GLFW/glfw3.h>

int main(int argc, char*argv[])
{
	WindowManager::Initialize();
	Renderer::Initialize();

	Scene scene = Scene();
	scene.Initialize();

	// defer loading to here

	SphereModel* c = new SphereModel(glm::vec3(0,0,0), glm::vec4(0,0,0,0));
	c->SetPosition(glm::vec3(0, 0, 0));
	scene.AddEntity(c);

	do
	{
		InputManager::Update();
		GameTime::Update();
		float dt = GameTime::GetFrameTime();
		scene.Update(dt);
		scene.Draw();
	} while (WindowManager::ExitRequested() == false);

	// should prolly do cleanup here i dont know
	Renderer::Shutdown();
	glfwTerminate();

	return 0;
}
