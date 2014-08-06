#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "WindowManager.h"

int main(int argc, char*argv[])
{
	WindowManager::Initialize();
	Renderer::Initialize();

	// defer loading to here

	do
	{
		InputManager::Update();

		GameTime::Update();
		float dt = GameTime::GetFrameTime();
		// update scene here

		// draw scene here

	} while (WindowManager::ExitRequested() == false);

	// should prolly do cleanup here i dont know

	return 0;
}
