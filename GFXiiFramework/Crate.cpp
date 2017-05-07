#include "Crate.h"
#include "GLEW/include/glew.h"
#include "Scene.h"
#include "ResourceManager.h"

using namespace std;
using namespace glm;

unsigned int	Crate::m_vao = 0;
unsigned int	Crate::m_vbo_verts = 0;
unsigned int	Crate::m_vbo_indices = 0;
unsigned int	Crate::m_totalTriangles = 12;

Crate::Crate(vec3 position, float floorHeight, function<void(glm::vec2)> onHitFloor)
{
	auto resourceManager = ResourceManager::GetInstance();
	m_texture = resourceManager->GetTexture("crate");
	m_shader = resourceManager->GetShader("crate");
	m_currentHeight = position.y - 0.5f;
	onFloor = false;

	m_onHitFloor = onHitFloor;

	uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	uniform_eyePosition = glGetUniformLocation(m_shader->GetProgramHandle(), "eyePosition");

	m_transform = translate(mat4(), position);
	m_floorHeight = floorHeight;
}

Crate::~Crate()
{

}

void Crate::Update(float dt)
{
	if (!onFloor)
	{
		auto fall = -dt * 30.0f;

		auto previousHeight = m_currentHeight;
		m_currentHeight += fall;

		if (m_currentHeight < m_floorHeight)
		{
			fall = m_floorHeight - previousHeight;
			m_currentHeight = m_floorHeight;
			onFloor = true;
		}

		m_transform = translate(m_transform, vec3(0, fall, 0));

		if (onFloor)
		{
			auto position = m_transform[3];
			m_onHitFloor(vec2(position.x, position.z));
		}
	}
	
	
}

void Crate::Render()
{
	m_shader->ActivateShaderProgram();

	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transform));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetViewMatrix())));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
	auto eyePosition = Scene::GetCamera()->GetCameraPosition();
	glUniform3f(uniform_eyePosition, eyePosition.x, eyePosition.y, eyePosition.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_syshandle);


	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Crate::LoadMesh()
{
	VertexAttributes vertices[24] =
	{
		{ vec3(-0.5f, -0.5f, 0.5f), vec3(), vec2(0.0f, 0.0f) },// 0
		{ vec3(0.5f, -0.5f, 0.5f), vec3(), vec2(1.0f, 0.0f) }, // 1
		{ vec3(0.5f, 0.5f, 0.5f), vec3(), vec2(1.0f, 1.0f) }, // 2
		{ vec3(-0.5f, 0.5f, 0.5f), vec3(), vec2(0.0f, 1.0f) }, // 3

		{ vec3(0.5f, -0.5f, 0.5f), vec3(), vec2(0.0f, 0.0f) }, // 4
		{ vec3(0.5f, -0.5f, -0.5f), vec3(), vec2(1.0f, 0.0f) }, // 5
		{ vec3(0.5f, 0.5f, -0.5f), vec3(), vec2(1.0f, 1.0f) }, // 6
		{ vec3(0.5f, 0.5f, 0.5f), vec3(), vec2(0.0f, 1.0f) }, // 7

		{ vec3(0.5f, -0.5f, -0.5f), vec3(), vec2(0.0f, 0.0f) }, // 8
		{ vec3(-0.5f, -0.5, -0.5f), vec3(), vec2(1.0f, 0.0f) }, //9
		{ vec3(-0.5f, 0.5, -0.5f), vec3(), vec2(1.0f, 1.0f) }, // 10
		{ vec3(0.5f, 0.5f, -0.5f), vec3(), vec2(0.0f, 1.0f) }, // 11
		
		{ vec3(-0.5f, -0.5, -0.5f), vec3(), vec2(0.0f, 0.0f) }, //12
		{ vec3(-0.5f, -0.5f, 0.5f), vec3(), vec2(1.0f, 0.0f) },// 13
		{ vec3(-0.5f, 0.5f, 0.5f), vec3(), vec2(1.0f, 1.0f) }, // 14
		{ vec3(-0.5f, 0.5, -0.5f), vec3(), vec2(0.0f, 1.0f) }, // 15
		
		{ vec3(-0.5f, 0.5f, 0.5f), vec3(), vec2(0.0f, 0.0f) }, // 16
		{ vec3(0.5f, 0.5f, 0.5f), vec3(), vec2(1.0f, 0.0f) }, // 17
		{ vec3(0.5f, 0.5f, -0.5f), vec3(), vec2(1.0f, 1.0f) }, // 18
		{ vec3(-0.5f, 0.5, -0.5f), vec3(), vec2(0.0f, 1.0f) }, // 19

		{ vec3(-0.5f, -0.5, -0.5f), vec3(), vec2(0.0f, 0.0f) }, //20
		{ vec3(0.5f, -0.5f, -0.5f), vec3(), vec2(1.0f, 0.0f) }, // 21
		{ vec3(0.5f, -0.5f, 0.5f), vec3(), vec2(1.0f, 1.0f) }, // 22
		{ vec3(-0.5f, -0.5f, 0.5f), vec3(), vec2(0.0f, 1.0f) },// 23
	};

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

	//Calculate Normal
	for (int i = 0; i < 36; i += 3)
	{
		auto v1 = vertices[indices[i]].position;
		auto v2 = vertices[indices[i + 1]].position;
		auto v3 = vertices[indices[i + 2]].position;

		auto face_normal = normalize(cross(v2 - v1, v3 - v1));

		vertices[indices[i]].normal += face_normal;
		vertices[indices[i + 1]].normal += face_normal;
		vertices[indices[i + 2]].normal += face_normal;
	}

	for (int i = 0; i < 8; i++)
	{
		vertices[i].normal = normalize(vertices[i].normal);
	}

	//Copy To Opengl Readable data
	const int vertexSize = (3 + 3 + 2) * sizeof(float);
	const int dataSize = 24 * vertexSize;
	const int dasda = sizeof(vertices);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, dataSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float)* 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float)* 6));
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Crate::UnloadMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo_verts);
	glDeleteBuffers(1, &m_vbo_indices);
}