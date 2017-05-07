#include "Farm.h"

using namespace std;
using namespace glm;

Farm::Farm(vec3 position, float orientation, function<float(vec2)> terrainHeightGetter, function<vec3()> playerPositionGetter)
{
	m_transformation = translate(mat4(), position);
	m_position = position;
	m_aided = false;

	m_house = make_shared<House>(vec3(0, 0, 0), this);

	float startX, startZ;
	//Front Plantation
	startX = -3 * 3;
	startZ = -10;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			float x = startX + 3 * i;
			float z = startZ - 3 * j;
			m_sunflowers.push_back(make_shared<Sunflower>(vec3(x, terrainHeightGetter(vec2(x + position.x, z + +position.z)) - position.y, z), this, playerPositionGetter));
		}
	}
	

	//Back Plantation
	startX = -3 * 3;
	startZ = 10;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			float x = startX + 3 * i;
			float z = startZ + 3 * j;
			m_sunflowers.push_back(make_shared<Sunflower>(vec3(x, terrainHeightGetter(vec2(x + position.x, z + +position.z)) - position.y, z), this, playerPositionGetter));
		}
	}

	//Left Plantation
	startX = -10;
	startZ = -3 * 3;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			float x = startX - 3 * j;
			float z = startZ + 3 * i;
			m_sunflowers.push_back(make_shared<Sunflower>(vec3(x, terrainHeightGetter(vec2(x + position.x, z + +position.z)) - position.y, z), this, playerPositionGetter));
		}
	}

	//Right Plantation
	startX = 10;
	startZ = -3 * 3;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			float x = startX + 3 * j;
			float z = startZ + 3 * i;
			m_sunflowers.push_back(make_shared<Sunflower>(vec3(x, terrainHeightGetter(vec2(x + position.x, z + +position.z)) - position.y, z), this, playerPositionGetter));
		}
	}

	m_width = 18 + (3 * 20);
	m_height = 18 + (3 * 20);
}


Farm::~Farm()
{

}

void Farm::Update(float dt)
{
	m_house->Update(dt);

	for (int i = 0; i < m_sunflowers.size(); i++)
	{
		auto& sunflower = m_sunflowers[i];

		sunflower->Update(dt);
	}
}

void Farm::Render()
{
	m_house->Render();

	for (int i = 0; i < m_sunflowers.size(); i++)
	{
		auto& sunflower = m_sunflowers[i];

		sunflower->Render();
	}
}

bool Farm::isInsideAABB(vec2 point, vec2 size)
{
	auto leftA = point.x - size.x / 2;
	auto rightA = point.x + size.x / 2;
	auto bottomA = point.y - size.y / 2;
	auto topA = point.y + size.y / 2;

	auto leftB = m_position.x - m_width / 2;
	auto rightB = m_position.x + m_width / 2;
	auto bottomB = m_position.z - m_height / 2;
	auto topB = m_position.z + m_height / 2;

	return leftB < leftA && rightB > rightA && bottomB < bottomA && topB > topA;
}

mat4 Farm::GetTransformation()
{
	return m_transformation;
}