#pragma once

class GameTime
{
public:
	static void Update();
	static float GetFrameTime() { return frameTime; }
private:
	static float frameTime;
	static double frameTimePrev;
};