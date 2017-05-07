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

class Lava: public Actor
{
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_totalTriangles;

	struct Branch
	{
		std::vector<int> indices;
		std::vector<glm::vec2> vertices;
		glm::vec2 endV1, endV2;
	};

	Branch CreateBranch(glm::vec2 v1, glm::vec2 v2, glm::vec2 direction, float length);

	std::shared_ptr<OGLShaderProgram> m_shader;
	std::shared_ptr<OGLTexture> m_texture;

	float m_textureDisplacement = 0;

	glm::mat4 m_transformation;

public:
	Lava(glm::vec3 position, float width, std::function<float(glm::vec2)> terrainHeightGetter);
	~Lava();

	void Update(float dt);
	void Render();
};

