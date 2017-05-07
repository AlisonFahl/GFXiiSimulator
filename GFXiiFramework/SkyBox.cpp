#include "SkyBox.h"

#include <windows.h>
#include "GLEW/include/glew.h"
#include "ResourceManager.h"
#include "Scene.h"

using namespace std;
using namespace glm;

SkyBox::SkyBox()
{
	InitUnitCube();

	auto resourceManager = ResourceManager::GetInstance();
	m_texture[0] = resourceManager->GetTexture("sky_rt");
	m_texture[1] = resourceManager->GetTexture("sky_ft");
	m_texture[2] = resourceManager->GetTexture("sky_lt");
	m_texture[3] = resourceManager->GetTexture("sky_bk");
	m_texture[4] = resourceManager->GetTexture("sky_tp");
	m_texture[5] = resourceManager->GetTexture("sky_bt");
	m_shader = resourceManager->GetShader("sky_box");
}

SkyBox::~SkyBox()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo_verts);
	glDeleteBuffers(1, &m_vbo_indices);
}

void SkyBox::InitUnitCube()
{
	SimpleVertex corners[] =
	{
		{ -0.5f, 0.5f, 0.5f, 0.0f, 1.0f }, // 3
		{ 0.5f, 0.5f, 0.5f, 1.0f, 1.0f }, // 2
		{ 0.5f, -0.5f, 0.5f, 1.0f, 0.0f }, // 1
		{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f },// 0

		{ 0.5f, 0.5f, 0.5f, 0.0f, 1.0f }, // 7
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f }, // 6
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 5
		{ 0.5f, -0.5f, 0.5f, 0.0f, 0.0f }, // 4
		
		{ 0.5f, 0.5f, -0.5f, 0.0f, 1.0f }, // 11
		{ -0.5f, 0.5, -0.5f, 1.0f, 1.0f }, // 10
		{ -0.5f, -0.5, -0.5f, 1.0f, 0.0f }, //9
		{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f }, // 8

		{ -0.5f, 0.5, -0.5f, 0.0f, 1.0f }, // 15
		{ -0.5f, 0.5f, 0.5f, 1.0f, 1.0f }, // 14
		{ -0.5f, -0.5f, 0.5f, 1.0f, 0.0f },// 13
		{ -0.5f, -0.5, -0.5f, 0.0f, 0.0f }, //12
		
		{ -0.5f, 0.5, -0.5f, 0.0f, 1.0f }, // 19
		{ 0.5f, 0.5f, -0.5f, 1.0f, 1.0f }, // 18
		{ 0.5f, 0.5f, 0.5f, 1.0f, 0.0f }, // 17
		{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f }, // 16

		{ -0.5f, -0.5f, 0.5f, 0.0f, 1.0f },// 23
		{ 0.5f, -0.5f, 0.5f, 1.0f, 1.0f }, // 22
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f }, // 21
		{ -0.5f, -0.5, -0.5f, 0.0f, 0.0f } //20
		
	};

	//We will use triangles instead of triangle strips
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 14, 15,
		12, 13, 14,

		16, 18, 19,
		16, 17, 18,

		20, 22, 23,
		20, 21, 22
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(SimpleVertex), corners, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 12));
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SkyBox::Update(float dt)
{
	m_transform = mat4_cast(Scene::GetCamera()->GetCameraRotation()) * scale(mat4(), vec3(999, 999, 999)) * mat4();
}

void SkyBox::Render()
{
	glBindSampler(0, 4);

	m_shader->ActivateShaderProgram();

	auto uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	auto uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");

	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)glm::value_ptr(m_transform));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)glm::value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));

	unsigned int texHandle = m_texture[0]->m_syshandle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	glBindVertexArray(m_vao);

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 14, 15,
		12, 13, 14,

		16, 18, 19,
		16, 17, 18,

		20, 22, 23,
		20, 21, 22
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]->m_syshandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices[6]);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]->m_syshandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices[12]);
	glBindTexture(GL_TEXTURE_2D, m_texture[3]->m_syshandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices[18]);
	glBindTexture(GL_TEXTURE_2D, m_texture[4]->m_syshandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices[24]);
	glBindTexture(GL_TEXTURE_2D, m_texture[5]->m_syshandle);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices[30]);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0);

	glBindSampler(0, 1);
}
