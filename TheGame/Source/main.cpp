#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "Scene.h"
#include "Cube.h"

int main(int argc, char*argv[])
{
	WindowManager::Initialize();
	Renderer::Initialize();

	Scene scene = Scene();
	scene.Initialize();

	// defer loading to here

	Cube* c = new Cube();
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

	return 0;
}
