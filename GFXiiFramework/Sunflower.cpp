#include "Sunflower.h"

#include <array>

#include "GLEW\include\glew.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Farm.h"

using namespace std;
using namespace glm;

unsigned int Sunflower::m_vao = 0;
unsigned int Sunflower::m_vbo = 0;
unsigned int Sunflower::m_ibo = 0;

Sunflower::Sunflower(vec3 position, Farm* farm, function<vec3()> playerPositionGetter)
{
	m_farm = farm;
	m_playerPositionGetter = playerPositionGetter;
	m_floorOffset = vec3(0, 1.5, 0);
	m_translation = translate(mat4(), position + m_floorOffset);
	m_scale = vec3(2, 3, 1);
	m_scaling = scale(mat4(), m_scale);

	auto resourcemanager = ResourceManager::GetInstance();
	m_shader = resourcemanager->GetShader("sunflower");
	m_texture = resourcemanager->GetTexture("sunflower");

	uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	uniform_viewVec = glGetUniformLocation(m_shader->GetProgramHandle(), "viewVec");
	uniform_rootScale = glGetUniformLocation(m_shader->GetProgramHandle(), "rootScale");
	uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
}


Sunflower::~Sunflower()
{

}

void Sunflower::Update(float dt)
{
	if (!calculatedFinalTransform)
	{
		m_transform = m_farm->GetTransformation() * m_translation * m_scaling;

		calculatedFinalTransform = true;
	}
}

void Sunflower::Render()
{
	m_shader->ActivateShaderProgram();

	glUniform1i(uniform_texture, 0);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transform));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetViewMatrix())));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
	auto viewVec = Scene::GetCamera()->GetViewVector();
	glUniform3f(uniform_viewVec, viewVec.x, viewVec.y, viewVec.z);
	glUniform3f(uniform_rootScale, m_scale.x, m_scale.y, m_scale.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_syshandle);

	glBindVertexArray(m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Sunflower::LoadMesh()
{
	const int vertexTotalData = 3 + 3 + 2;
	array<float, 4 * vertexTotalData> vertices =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};//4 vertices each with position + UV + normal

	array<int, 12> indices =
	{
		//Front Face
		0, 1, 2,
		2, 3, 0,

		//Back Face
		2, 1, 0,
		0, 3, 2
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)* vertexTotalData, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* vertexTotalData, (void *)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)* vertexTotalData, (void *)(sizeof(float)* 6));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Sunflower::UnloadMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}