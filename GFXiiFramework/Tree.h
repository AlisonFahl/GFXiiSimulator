#pragma once

#include <memory>
#include <functional>
#include"OpenGL\OGLShader.h"
#include"OpenGL\OGLTexture.h"
#include "Actor.h"
#include "Terrain.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

class Tree : public Actor
{
private:
	static unsigned int m_vao;
	static unsigned int m_vbo;
	static unsigned int m_ibo;

	unsigned int uniform_model;
	unsigned int uniform_view;
	unsigned int uniform_projection;
	unsigned int uniform_texture;

	std::shared_ptr<OGLShaderProgram> m_shader;
	std::shared_ptr<OGLTexture> m_texture;

	glm::vec3 m_floorOffset;
	glm::vec3 m_scale;

	glm::mat4 m_scaling, m_rotation, m_translation;
	glm::mat4 m_transform;

public:
	Tree(glm::vec3 position);
	~Tree();

	void Update(float dt);
	void Render();

	static void LoadMesh();
	static void UnloadMesh();
};

