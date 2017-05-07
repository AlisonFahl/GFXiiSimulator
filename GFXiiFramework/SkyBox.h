#pragma once

#include "Actor.h"
#include "OpenGL\OGLMesh.h"
#include "OpenGL\OGLShader.h"
#include "Renderer.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "camera.h"
#include <memory>

class SkyBox: public Actor
{
private:
	struct SimpleVertex
	{
		float position[3];
		float uv[2];
	};

private:
	unsigned int	m_vao;
	unsigned int	m_vbo_verts;
	unsigned int	m_vbo_indices;

	std::shared_ptr<OGLTexture> m_texture[6];
	std::shared_ptr<OGLShaderProgram> m_shader;
	glm::mat4 m_transform;

	void			InitUnitCube();

public:
	SkyBox();
	~SkyBox();

	void Update(float dt);
	void Render();
};

