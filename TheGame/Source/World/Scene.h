#pragma once

#include "Arwing.h"
#include "GameplayCamera.h"
#include "Chunk.h"
#include "EnemyFactory.h"
#include "Entity.h"
#include "UIElement.h"

class Entity;
class EnemyFactory;
class UltraBoss;

class Scene
{
public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}

	void Initialize();
	void Update(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	void AddChunk(glm::vec3 pos);

	Arwing* GetPlayer() { return a; }
	UltraBoss* GetBoss() { return boss; }
	GameplayCamera* GetGPCamera() { return static_cast<GameplayCamera*>(camera); }
	int ChangeLightingMode() { ++lightingMode > 1 ? lightingMode = 0 : lightingMode = 1; return lightingMode; };

	int score = 0;
	void GameOver();
	bool IsGameOver() { return gameOver; }
	void GameWon();
	bool IsGameWon() { return gameWon; }

private:
	// Hide constructors
	Scene();
	Scene(Scene const&);

	std::vector<Entity*> entities;
	std::vector<Entity*> transparentEntities; // Particle systems should be drawn after always.
	std::vector<Entity*> queued; // queued entities such as pewpews that need to be added
	std::vector<Chunk*> chunks;
	std::vector<UIElement*> uiElements;

	Camera* camera;
	Arwing* a;
	UltraBoss* boss;
	EnemyFactory* enemyFactory;

	int lightingMode = 0; // 0 or 1
	int lastChunk = 0;

	void buildUI();
	void loadTextures();
	void updateEnemyAttack();
	void updateEntities(std::vector<Entity*> &entities, float dt);
	void updateUIElements(std::vector<UIElement*> &uielements, float dt);
	void drawEntities(std::vector<Entity*> &entities);
	void drawUI(std::vector<UIElement*> &uielem);
	bool checkVisible(const glm::mat4, const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);

	bool bossSpawned = false;
	bool gameOver = false;
	bool gameWon = false;
};