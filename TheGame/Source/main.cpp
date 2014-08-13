#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "Scene.h"

#include <GLFW/glfw3.h>

int main(int argc, char*argv[])
{
	WindowManager::Initialize();

	Renderer& renderer = Renderer::GetInstance();
	renderer.Initialize();

	Scene& scene = Scene::GetInstance();
	scene.Initialize();

	InputManager::Initialize(scene.GetPlayer());

	do
	{
		GameTime::Update();
		float dt = GameTime::GetFrameTime();
		InputManager::Update(dt);
		scene.Update(dt);
		scene.Draw();
	} while (WindowManager::ExitRequested() == false);

	// should prolly do cleanup here i dont know
	Renderer::GetInstance().Shutdown();
	glfwTerminate();

	return 0;
}
