#pragma once

#include<vector>
#include<memory>
#include<functional>

#include "Actor.h"
#include "Sunflower.h"
#include "House.h"

class Farm: public Actor
{
private:
	std::shared_ptr<House> m_house;
	std::vector<std::shared_ptr<Sunflower>> m_sunflowers;

	glm::mat4 m_transformation;
	glm::vec3 m_position;
	float m_height, m_width;

	bool m_aided;

public:
	Farm(glm::vec3 position, float orientation, std::function<float(glm::vec2)> terrainHeightGetter, std::function<glm::vec3()> playerPositionGetter);
	~Farm();

	void Update(float dt);
	void Render();

	bool isInsideAABB(glm::vec2 point, glm::vec2 size);

	glm::mat4 GetTransformation();
	inline bool GetAided()
	{
		return m_aided;
	}
	inline void SetAided(bool value)
	{
		m_aided = value;
	}
};

