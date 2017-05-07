#pragma once

#include "Actor.h"
#include "OpenGL\OGLMesh.h"
#include "OpenGL\OGLShader.h"
#include "Renderer.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "camera.h"
#include <memory>

class Fighter: public Actor
{
private:
	std::unique_ptr<Renderer> m_renderer;
	glm::vec3 m_rootScale, m_rootTranslation, m_rootRotation;
	glm::mat4 m_transform, m_rotation, m_translation, m_scale;
	glm::vec3 m_position;

	struct Movement
	{
		bool goingForward, goingBackward, goingLeft, goingRight, goingDown, goingUp;
		bool yawingLeft, yawingRight, pitchingDown, pitchingUp, rollingLeft, rollingRight;
	};
	Movement m_movement;
	float velocity, rotSpeed;

public:
	Fighter();
	~Fighter();

	void Update(float dt);
	void Render();

	glm::vec3 inline GetPosition()
	{
		return m_position;
	}

	void GoingForward(bool value);
	void GoingBackward(bool value);
	void GoingLeft(bool value);
	void GoingRight(bool value);
	void GoingUp(bool value);
	void GoingDown(bool value);
	void PitchingDown(bool value);
	void PitchingUp(bool value);
	void YawingLeft(bool value);
	void YawingRight(bool value);
	void RollingLeft(bool value);
	void RollingRight(bool value);
};

