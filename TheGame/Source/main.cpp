#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "Scene.h"

#include <GLFW/glfw3.h>

int main(int argc, char*argv[])
{
	WindowManager::Initialize();
	Renderer::Initialize();

	Scene* scene = new Scene();
	scene->Initialize();

	InputManager::Initialize(scene, scene->GetPlayer());

	do
	{
		InputManager::Update();
		GameTime::Update();
		float dt = GameTime::GetFrameTime();
		scene->Update(dt);
		scene->Draw();
	} while (WindowManager::ExitRequested() == false);

	// should prolly do cleanup here i dont know
	Renderer::Shutdown();
	glfwTerminate();

	return 0;
}
