//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 22/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Entity.h"

class SphereModel : public Entity
{
public:

	SphereModel(glm::vec3 lAttenuation, glm::vec4 materialProperties);
	virtual ~SphereModel();

	virtual void Update(float dt);
	virtual void Draw();

private:

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int numVertices;
};
