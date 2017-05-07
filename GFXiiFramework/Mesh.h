#pragma once

#include <windows.h>
#include <vector>
#include "Triangle.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

class Mesh
{
private:
	unsigned int m_vao;
	unsigned int m_vbo_verts;
	int m_numtriangles;

	struct MeshVertex
	{
		float position[3];
		float normal[3];
		float texCoord[2];
		float tangent[3];
	};

	void LoadAndBuildMeshFromOBJFile(LPCWSTR filename);
	void CrateMeshFromData(std::vector<Triangle> data);

public:
	Mesh(){};
	Mesh(LPCWSTR filename);
	Mesh(std::vector<Triangle> data);
	virtual ~Mesh();

	void LoadAndBuildMeshFromOBJFileEx(LPCWSTR filename);

	void BindAttributes();
	void Draw();
};

