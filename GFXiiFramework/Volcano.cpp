#include "Volcano.h"

#include <vector>
#include "GLEW\include\glew.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "AssetImport\FreeImage\FreeImage.h"

using namespace std;
using namespace glm;

Volcano::Volcano(float scale, float maximumHeight)
{
	m_scale = scale;

	auto heightMap = FreeImage_Load(FIF_PNG, "../asset/texture/volcano_heightmap.png");

	float width = FreeImage_GetWidth(heightMap);
	float height = FreeImage_GetHeight(heightMap);
	m_width = width;
	m_height = height;

	m_surfacePoints.reserve(m_height);

	vector<float> vertices;
	vertices.reserve(width * height * (3 + 2 + 3));
	float startX = -(width / 2.0f) * scale;
	float startY = -(height / 2.0f) * scale;
	float strideX = scale;
	float strideY = scale;

	for (int i = 0; i < height; i++)
	{
		auto row = vector<SurfacePoint>();
		row.reserve(width);
		m_surfacePoints.push_back(row);

		for (int j = 0; j < width; j++)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(heightMap, j, i, &pixel);

			float positionX = startX + (j * strideX);
			float positionY = startY + (i * strideY);
			float positionZ = (pixel.rgbRed / 255.0f) * maximumHeight;
			float positionU = j / (width * 0.05f);
			float positionV = i / (height * 0.05f);

			if (positionX == 0 && positionY == -20)
			{
				int debug = 0;
			}

			vertices.push_back(positionX);//X
			vertices.push_back(positionZ);//Y
			vertices.push_back(positionY);//Z
			vertices.push_back(positionU);//U
			vertices.push_back(positionV);//V

			m_surfacePoints[m_surfacePoints.size() - 1].push_back({ positionZ });
		}
	}

	FreeImage_Unload(heightMap);

	vector<int> indices;
	//int totalTiles = (width - 1) * (height - 1);//((width * height) / 4);//Each tile (square) is formed by 4 vertices
	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			int a = (i * width) + j;//v1
			int b = ((i + 1) * width) + j;//v2
			int c = ((i + 1) * width) + j + 1;//v3
			int d = (i * width) + j + 1;//v4

			//left triangle from tile
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);

			//right triangle from tile
			indices.push_back(c);
			indices.push_back(d);
			indices.push_back(a);
		}
	}
	m_totalTriangles = indices.size() / 3;

	//Vertex/Surface Normals
	vector<vec3> normalList((int)(width * height), vec3());
	for (int i = 0; i < m_totalTriangles; i++)
	{
		int first = i * 3;
		vec3 v1 = vec3(vertices[indices[first] * 5], vertices[indices[first] * 5 + 1], vertices[indices[first] * 5 + 2]);
		first++;
		vec3 v2 = vec3(vertices[indices[first] * 5], vertices[indices[first] * 5 + 1], vertices[indices[first] * 5 + 2]);
		first++;
		vec3 v3 = vec3(vertices[indices[first] * 5], vertices[indices[first] * 5 + 1], vertices[indices[first] * 5 + 2]);

		auto normal = normalize(cross(v2 - v1, v3 - v1));

		for (int j = first; j > (first - 3); j--)
		{
			normalList.data()[indices[j]] += normal;
		}

	}
	for (int i = 0; i < normalList.size(); i++)
	{
		normalList.data()[i] = normalize(normalList.data()[i]);

		vertices.push_back(normalList[i].x);
		vertices.push_back(normalList[i].y);
		vertices.push_back(normalList[i].z);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)(sizeof(float) * (int)(width * height * 5)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	auto resourcemanager = ResourceManager::GetInstance();
	m_shader = resourcemanager->GetShader("terrain");
	m_texture = resourcemanager->GetTexture("volcano");
}


Volcano::~Volcano()
{

}

void Volcano::Update(float dt)
{

}

void Volcano::Render()
{
	m_shader->ActivateShaderProgram();

	auto uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	auto uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	auto uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	auto uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
	auto uniform_eyePosition = glGetUniformLocation(m_shader->GetProgramHandle(), "eyePosition");
	auto uniform_specularLightColor = glGetUniformLocation(m_shader->GetProgramHandle(), "specularLightColor");

	glUniform1i(uniform_texture, 0);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(mat4()));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetViewMatrix())));
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
	auto eyePosition = Scene::GetCamera()->GetCameraPosition();
	glUniform3f(uniform_eyePosition, eyePosition.x, eyePosition.y, eyePosition.z);
	glUniform3f(uniform_specularLightColor, 0.6f, 0.6f, 0.6f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->m_syshandle);

	glBindVertexArray(m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_totalTriangles * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

float Volcano::GetHeight(glm::vec2 point)
{
	//Bilinear Interpolation
	//http://technicalc.org/tiplist/en/files/pdf/tips/tip6_28.pdf

	float x = (point.x / m_scale) + (m_width / 2.0f);
	int x1 = floor(x);
	int x2 = x1 + 1;
	float y = (point.y / m_scale) + (m_height / 2.0f);
	int y1 = floor(y);
	int y2 = y1 + 1;

	auto z1 = m_surfacePoints[y1][x1].height;
	auto z2 = m_surfacePoints[y1][x2].height;
	auto z3 = m_surfacePoints[y2][x1].height;
	auto z4 = m_surfacePoints[y2][x2].height;

	float a = z3 - z1;
	float b = z2 - z1;
	float c = z1 - z2 - z3 + z4;
	float d = z1;

	float t = (x - x1) / (x2 - x1);
	float u = (y - y1) / (y2 - y1);

	//interpolate the final value
	auto finalHeight = t * a + u * b + c * u * t + d;

	return finalHeight;
}