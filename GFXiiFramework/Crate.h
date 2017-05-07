#pragma once

#include "Actor.h"
#include "OpenGL\OGLMesh.h"
#include "OpenGL\OGLShader.h"
#include "Renderer.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "camera.h"

#include <memory>
#include <functional>

class Crate: public Actor
{
private:
	static unsigned int	m_vao;
	static unsigned int	m_vbo_verts;
	static unsigned int	m_vbo_indices;
	static unsigned int m_totalTriangles;

	struct VertexAttributes
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texturePosition;
	};

	unsigned int uniform_model;
	unsigned int  uniform_view;
	unsigned int  uniform_projection;
	unsigned int  uniform_eyePosition;

	std::shared_ptr<OGLTexture> m_texture;
	std::shared_ptr<OGLShaderProgram> m_shader;

	glm::mat4 m_transform;
	float m_floorHeight;
	float m_currentHeight;
	bool onFloor;

	std::function<void(glm::vec2)> m_onHitFloor;

public:
	Crate(glm::vec3 position, float floorHeight, std::function<void(glm::vec2)> onHitFloor);
	~Crate();

	static void LoadMesh();
	static void UnloadMesh();

	void Update(float dt);
	void Render();
};

