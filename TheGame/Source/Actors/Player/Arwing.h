//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki	  (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// This file contributions: David
//

#pragma once

#include "Cube.h"
#include "Entity.h"
#include "ParticleSystem.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);

	void Update(float dt);
	void OnCollision(Entity* other);

	void SetMovingForward(bool forward) { isMovingForward = forward; }
	void TiltLeft(float dt);
	void TiltRight(float dt);
	void TiltComplete(float dt);

	void Flash() { isFlashing = !isFlashing; }
	bool IsFlashing() { return isFlashing; }

	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool isMovingForward;
	bool isTiltingLeft;
	bool isTiltingRight;
	bool isBarrelRolling;
	float barrelRollTimer;
	float invicibilityFrames;
	bool iddqd;
	bool isFlashing;
protected:

private:
	ParticleSystem* booster;
	ParticleSystem* burnBabyBurn;
	void resolveHit(float damage);
};