#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "ParticleSystem.h"

using namespace glm;

struct Particle;
const int maxParticles = 500;

Particle * Container[maxParticles];

void SortParticles()
{
	std::sort(&Container[0], &Container[maxParticles]);
}