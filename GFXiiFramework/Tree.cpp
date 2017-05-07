#include "Tree.h"

#include <array>

#include "GLEW\include\glew.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Farm.h"

using namespace std;
using namespace glm;

unsigned int Tree::m_vao = 0;
unsigned int Tree::m_vbo = 0;
unsigned int Tree::m_ibo = 0;

Tree::Tree(vec3 position)
{
	m_floorOffset = vec3(0, 10, 0);
	m_translation = translate(mat4(), position + m_floorOffset);
	m_scale = vec3(10, 20, 10);
	m_scaling = scale(mat4(), m_scale);

	m_transform = m_translation * m_rotation * m_scaling;

	auto resourcemanager = ResourceManager::GetInstance();
	m_shader = resourcemanager->GetShader("tree");
	m_texture = resourcemanager->GetTexture("tree");

	uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
}


Tree::~Tree()
{

}

void Tree::Update(float dt)
{

}

void Tree::Render()
{
	m_shader->ActivateShaderProgram();

	glUniform1i(uniform_texture, 0);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transform));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetViewMatrix())));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_syshandle);

	glBindVertexArray(m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Tree::LoadMesh()
{
	const int vertexTotalData = 3 + 2;
	array<float, 8 * vertexTotalData> vertices =
	{
		/*0.0f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
		0.0f, -0.5f, 0.0f, 0.5f, 0.0f,

		0.0f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
		0.0f, -0.5f, 0.0f, 0.5f, 0.0f,*/

		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

		0.0f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.0f, -0.5f, -0.5f, 1.0f, 0.0f,
	};

	array<int, 24> indices =
	{
		//Front 1
		0, 1, 2,
		2, 3, 0,

		//Front 2
		2, 1, 0,
		0, 3, 2,

		//Side 1
		4, 5, 6,
		6, 7, 4,

		//Side 2
		6, 5, 4,
		4, 7, 6,

	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)* vertexTotalData, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* vertexTotalData, (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Tree::UnloadMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}