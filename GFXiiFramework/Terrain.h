#pragma once

#include <memory>
#include <vector>
#include"OpenGL\OGLShader.h"
#include"OpenGL\OGLTexture.h"
#include "Actor.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

class Terrain: public Actor
{
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_totalTriangles;

	float m_width, m_height;
	float m_scale;

	std::shared_ptr<OGLShaderProgram> m_shader;
	std::shared_ptr<OGLTexture> m_texture;

	struct SurfacePoint
	{
		float height;
		//glm::vec3 normal;
	};
	std::vector<std::vector<SurfacePoint>> m_surfacePoints;

public:
	Terrain(float scale, float maximumHeight);
	~Terrain();

	void Update(float dt);
	void Render();

	float GetHeight(glm::vec2 point);
};

