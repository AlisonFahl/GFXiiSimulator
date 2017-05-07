#include "PerspectiveCamera.h"

using namespace glm;

PerspectiveCamera::PerspectiveCamera(float ratio, float fovy, float near, float far):
Camera()
{
	m_aspectRatio = ratio;
	m_fovy = fovy;
	m_near = near;
	m_far = far;

	UpdateProjectionMatrix();
}


PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::UpdateProjectionMatrix()
{
	m_projectionMatrix = perspective(radians(m_fovy), m_aspectRatio, m_near, m_far);
}