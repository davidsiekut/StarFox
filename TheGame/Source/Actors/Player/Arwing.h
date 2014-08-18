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

	void SetMovingForwards(bool forward) { movingForward = forward; }
	void Flash() { isFlashing = !isFlashing; }

	void TiltLeft(float dt);
	void TiltRight(float dt);
	void TiltComplete(float dt);

	// Movement constants
	float rotationAngle;
	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool movingForward;
	bool isTiltingLeft;
	bool isTiltingRight;
	bool isBarrelRolling;
	float barrelRollTimer;
	bool iddqd;
	float invicibilityFrames = 0.f;

protected:

private:
	ParticleSystem* booster;
	ParticleSystem* burnBabyBurn;
	void resolveHit(float damage);
};