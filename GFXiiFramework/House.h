#pragma once

#include <memory>

#include "Actor.h"
#include "OpenGL\OGLMesh.h"
#include "OpenGL\OGLShader.h"
#include "Renderer.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "Scene.h"

class Farm;

class House : public Actor
{
private:
	std::unique_ptr<Renderer> m_renderer;
	glm::mat4 m_transform;
	glm::vec3 m_position, m_floorOffset;

	Farm* m_farm;

public:
	House(glm::vec3 position, Farm* farm);
	~House();

	void Update(float dt);
	void Render();
};

