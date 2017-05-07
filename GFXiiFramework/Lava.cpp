#include "Lava.h"

#include "GLEW\include\glew.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <array>

using namespace std;
using namespace glm;

Lava::Lava(glm::vec3 position, float initialLength, std::function<float(glm::vec2)> terrainHeightGetter)
{
	m_transformation = translate(mat4(), position);

	vector<array<float, 5>> vertices;
	vector<int> indices;

	vec3 center(0, 0, 0);
	array<float, 5> vertex = { center.x, center.y, center.z, 0.5, 0.5 };
	vertices.push_back(vertex);
	float radius = 80;
	for (int i = 0; i < 360; i++)
	{
		float cosine = cosf(i);
		float sine = sinf(-i);
		float x = cosine * radius;
		float y = sine * radius;
		float u = 0.5 + (cosine * 0.5);
		float v = 0.5 + (sine * 0.5);

		array<float, 5> vertex = { x, 0, y, u, v };
		vertices.push_back(vertex);

		if (i != 0)
		{
			if (i == (360 - 1))
			{
				indices.push_back(0);
				indices.push_back(i);
				indices.push_back(1);
			}
			else
			{
				indices.push_back(0);
				indices.push_back(i - 1);
				indices.push_back(i);
			}
		}
	}

	////Right
	//auto branch = CreateBranch(vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(1, 0), initialLength);

	//int lastIndex = vertices.size();

	//for (int i = 0; i < branch.vertices.size(); i+=2)
	//{
	//	array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//	vertices.push_back(v);
	//}
	//for each (auto& index in branch.indices)
	//{
	//	indices.push_back(index + lastIndex);
	//}
	//	//Right
	//	{
	//		branch = CreateBranch(vec2(0.5 * initialLength - 1, -0.5), vec2(0.5 * initialLength, -0.5), normalize(vec2(1, -1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}
	//	//Left
	//	{
	//		branch = CreateBranch(vec2(0.5 * initialLength, 0.5), vec2(0.5 * initialLength - 1, 0.5), normalize(vec2(1, 1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}

	////Left
	//	branch = CreateBranch(vec2(-0.5, 0.5), vec2(-0.5, -0.5), vec2(-1, 0), initialLength);

	//lastIndex = vertices.size();

	//for (int i = 0; i < branch.vertices.size(); i += 2)
	//{
	//	array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//	vertices.push_back(v);
	//}
	//for each (auto& index in branch.indices)
	//{
	//	indices.push_back(index + lastIndex);
	//}

	////Right
	//	{
	//		branch = CreateBranch(vec2(-0.5 * initialLength - 1, -0.5), vec2(-0.5 * initialLength, -0.5), normalize(vec2(-1, -1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}
	//	//Left
	//	{
	//		branch = CreateBranch(vec2(-0.5 * initialLength, 0.5), vec2(-0.5 * initialLength - 1, 0.5), normalize(vec2(-1, 1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}

	////Forward
	//	branch = CreateBranch(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0, -1), initialLength);

	//lastIndex = vertices.size();

	//for (int i = 0; i < branch.vertices.size(); i += 2)
	//{
	//	array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//	vertices.push_back(v);
	//}
	//for each (auto& index in branch.indices)
	//{
	//	indices.push_back(index + lastIndex);
	//}

	////Right
	//	{
	//		branch = CreateBranch(vec2(0.5, -0.5 * initialLength - 1), vec2(0.5, -0.5 * initialLength), normalize(vec2(1, -1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}
	//	//Left
	//	{
	//		branch = CreateBranch(vec2(-0.5, -0.5 * initialLength), vec2(-0.5, -0.5 * initialLength - 1), normalize(vec2(-1, -1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}

	////Backward
	//branch = CreateBranch(vec2(0.5, 0.5), vec2(-0.5, 0.5), vec2(0, 1), initialLength);

	//lastIndex = vertices.size();

	//for (int i = 0; i < branch.vertices.size(); i += 2)
	//{
	//	array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//	vertices.push_back(v);
	//}
	//for each (auto& index in branch.indices)
	//{
	//	indices.push_back(index + lastIndex);
	//}

	////Right
	//	{
	//		branch = CreateBranch(vec2(0.5, 0.5 * initialLength - 1), vec2(0.5, 0.5 * initialLength), normalize(vec2(1, 1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}
	//	//Left
	//	{
	//		branch = CreateBranch(vec2(-0.5, 0.5 * initialLength), vec2(-0.5, 0.5 * initialLength - 1), normalize(vec2(-1, 1)), initialLength * 0.3f);

	//		lastIndex = vertices.size();

	//		for (int i = 0; i < branch.vertices.size(); i += 2)
	//		{
	//			array<float, 5> v = { branch.vertices[i].x, 0, branch.vertices[i].y, branch.vertices[i + 1].x, branch.vertices[i + 1].y };
	//			vertices.push_back(v);
	//		}
	//		for each (auto& index in branch.indices)
	//		{
	//			indices.push_back(index + lastIndex);
	//		}
	//	}

	m_totalTriangles = indices.size() / 3;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 5, vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	auto resourcemanager = ResourceManager::GetInstance();
	m_shader = resourcemanager->GetShader("lava");
	m_texture = resourcemanager->GetTexture("lava");
}


Lava::~Lava()
{

}

Lava::Branch Lava::CreateBranch(vec2 v1, vec2 v2, vec2 direction, float length)
{
	auto branch = Lava::Branch();

	for (int i = 0; i < floor(length); i++)
	{
		auto subV1 = v1 + (direction * (float)i);
		auto subV2 = v2 + (direction * (float)i);
		branch.endV1 = subV1 + (direction * 1.0f);
		branch.endV2 = subV2 + (direction * 1.0f);

		branch.vertices.push_back(subV1); branch.vertices.push_back(vec2(0, 0));
		branch.vertices.push_back(subV2); branch.vertices.push_back(vec2(0, 1));
		branch.vertices.push_back(branch.endV1); branch.vertices.push_back(vec2(1, 0));
		branch.vertices.push_back(branch.endV2);  branch.vertices.push_back(vec2(1, 1));

		auto firstIndex = i * 4;
		//First Triangle
		branch.indices.push_back(firstIndex + 0);
		branch.indices.push_back(firstIndex + 1);
		branch.indices.push_back(firstIndex + 3);

		//Second Triangle
		branch.indices.push_back(firstIndex + 3);
		branch.indices.push_back(firstIndex + 2);
		branch.indices.push_back(firstIndex + 0);
	}

	return branch;
}

void Lava::Update(float dt)
{
	m_textureDisplacement += (dt * 0.1f);
	if (m_textureDisplacement >= 1)
	{
		m_textureDisplacement = m_textureDisplacement - 1;
	}
}

void Lava::Render()
{
	m_shader->ActivateShaderProgram();

	auto uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	auto uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	auto uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	auto uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
	auto uniform_texDisp = glGetUniformLocation(m_shader->GetProgramHandle(), "texDisp");

	glUniform1i(uniform_texture, 0);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transformation));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetViewMatrix())));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
	glUniform1f(uniform_texDisp, m_textureDisplacement);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_syshandle);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_totalTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}