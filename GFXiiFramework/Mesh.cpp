#include "Mesh.h"
#include "OBJFileReader.h"
#include "GLEW/include/glew.h"

using namespace std;
using namespace glm;

Mesh::Mesh(LPCWSTR filename)
{
	LoadAndBuildMeshFromOBJFile(filename);
}

Mesh::Mesh(vector<Triangle> data)
{
	CrateMeshFromData(data);
}

Mesh::~Mesh()
{

}

void Mesh::LoadAndBuildMeshFromOBJFileEx(LPCWSTR filename)
{
	Triangle* mesh;

	m_numtriangles = importOBJMesh(filename, &mesh);

	vector<MeshVertex> vertices;
	//tangents.reserve(m_numtriangles * );

	//http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html
	for (unsigned int i = 0; i < m_numtriangles; i ++) {
		vec3 v0 = vec3(mesh[i].m_vertices[0].m_position[0], mesh[i].m_vertices[0].m_position[1], mesh[i].m_vertices[0].m_position[2]);
		vec3 v1 = vec3(mesh[i].m_vertices[1].m_position[0], mesh[i].m_vertices[1].m_position[1], mesh[i].m_vertices[1].m_position[2]);
		vec3 v2 = vec3(mesh[i].m_vertices[2].m_position[0], mesh[i].m_vertices[2].m_position[1], mesh[i].m_vertices[2].m_position[2]);

		vec2 v0Tex = vec2(mesh[i].m_vertices[0].m_texcoords[0], mesh[i].m_vertices[0].m_texcoords[1]);
		vec2 v1Tex = vec2(mesh[i].m_vertices[1].m_texcoords[0], mesh[i].m_vertices[1].m_texcoords[1]);
		vec2 v2Tex = vec2(mesh[i].m_vertices[2].m_texcoords[0], mesh[i].m_vertices[2].m_texcoords[1]);

		vec3 Edge1 = v1 - v0;
		vec3 Edge2 = v2 - v0;

		float DeltaU1 = v1Tex.x - v0Tex.x;
		float DeltaV1 = v1Tex.y - v0Tex.y;
		float DeltaU2 = v2Tex.x - v0Tex.x;
		float DeltaV2 = v2Tex.y - v0Tex.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		vec3 Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		/*Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);*/

		//v0.m_tangent += Tangent;
		//v1.m_tangent += Tangent;
		//v2.m_tangent += Tangent;
		vertices.push_back(MeshVertex{
			{v0.x, v0.y, v0.z},
			{ mesh[i].m_vertices[0].m_normal[0], mesh[i].m_vertices[0].m_normal[1], mesh[i].m_vertices[0].m_normal[2] },
			{ v0Tex.x, v0Tex.y },
			{ Tangent.x, Tangent.y, Tangent.z}
		});
		vertices.push_back(MeshVertex{
			{ v1.x, v1.y, v1.z },
			{ mesh[i].m_vertices[1].m_normal[0], mesh[i].m_vertices[1].m_normal[1], mesh[i].m_vertices[1].m_normal[2] },
			{ v1Tex.x, v1Tex.y },
			{ Tangent.x, Tangent.y, Tangent.z }
		});
		vertices.push_back(MeshVertex{
			{ v2.x, v2.y, v2.z },
			{ mesh[i].m_vertices[2].m_normal[0], mesh[i].m_vertices[2].m_normal[1], mesh[i].m_vertices[2].m_normal[2] },
			{ v2Tex.x, v2Tex.y },
			{ Tangent.x, Tangent.y, Tangent.z }
		});
	}        

	int offset = sizeof(Vector3);
	int stride = sizeof(MeshVertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_numtriangles * sizeof(MeshVertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* 6));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* 8));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	delete[] mesh;
}

void Mesh::LoadAndBuildMeshFromOBJFile(LPCWSTR filename)
{
	Triangle* mesh;

	m_numtriangles = importOBJMesh(filename, &mesh);

	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_numtriangles * sizeof(Vertex), &(mesh[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + 2 * offset));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	delete[] mesh;
}

void Mesh::CrateMeshFromData(vector<Triangle> data)
{
	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	m_numtriangles = data.size();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_numtriangles * sizeof(Vertex), &(data[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + 2 * offset));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::BindAttributes()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
}

void Mesh::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, m_numtriangles * 3);
}
