#pragma once

#include <memory>
#include <vector>
#include "PerspectiveCamera.h"
#include "Actor.h"

class Scene
{
private:
	static std::shared_ptr<Camera> s_camera;
	std::vector<std::shared_ptr<Actor>> m_actors;

public:
	Scene();
	~Scene();

	static inline std::shared_ptr<Camera> GetCamera()
	{
		return s_camera;
	}

	static inline void SetCamera(std::shared_ptr<Camera> camera)
	{
		s_camera = camera;
	}

	void Update(float dt);
	void Render();

	inline void AttachActor(std::shared_ptr<Actor> actor)
	{
		m_actors.push_back(actor);
	}
};

