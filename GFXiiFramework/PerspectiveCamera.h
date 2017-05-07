#pragma once
#include "camera.h"
class PerspectiveCamera : public Camera
{
private:
	float m_fovy;

public:
	PerspectiveCamera(float ratio, float fov, float near, float far);
	virtual ~PerspectiveCamera();

	void UpdateProjectionMatrix();
};

