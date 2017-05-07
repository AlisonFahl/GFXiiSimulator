//Created for Graphics I and II
//Author: Minsi Chen

#include "camera.h"

using namespace glm;

Camera::Camera()
{
	m_viewVector = vec3(0, 0, -1);
	m_upVector = vec3(0, 1, 0);
	m_rightVector = cross(m_viewVector, vec3(0, -1, 0));

	m_position = vec3(0, 0, 0);
	UpdateViewMatrix();
}

Camera::~Camera()
{

}

void Camera::LookAtPoint(const vec3& point)
{
	m_viewVector = normalize(point - m_position);
	m_rightVector = normalize(cross(m_viewVector, vec3(0, 1, 0)));
	m_upVector = cross(m_rightVector, m_viewVector);
}

void Camera::UpdateViewMatrix()
{
	m_viewMatrix[0][0] = m_rightVector.x;
	m_viewMatrix[1][0] = m_rightVector.y;
	m_viewMatrix[2][0] = m_rightVector.z;
	m_viewMatrix[0][1] = m_upVector.x;
	m_viewMatrix[1][1] = m_upVector.y;
	m_viewMatrix[2][1] = m_upVector.z;
	m_viewMatrix[0][2] = -m_viewVector.x;
	m_viewMatrix[1][2] = -m_viewVector.y;
	m_viewMatrix[2][2] = -m_viewVector.z;
	m_viewMatrix[3][0] = -dot(m_rightVector, m_position);
	m_viewMatrix[3][1] = -dot(m_upVector, m_position);
	m_viewMatrix[3][2] = dot(m_viewVector, m_position);
	m_viewMatrix[3][3] = 1;
}

void Camera::StrafeCamera(float amount)
{
	m_position += (amount * m_rightVector);
}

void Camera::DollyCamera(float amount)
{
	m_position += (amount * m_viewVector);
}

void Camera::PedCamera(float amount)
{
	m_position += (amount * m_upVector);
}

void Camera::RotateCamera(float yaw, float pitch, float roll)
{
	//quat rot = quat(radians(yaw), 0, 1, 0) * quat(radians(pitch), 0, 0, 1) * quat(radians(roll), 1, 0, 0);

	//auto viewMat = mat4(vec4(m_rightVector, 0), vec4(m_upVector, 0), vec4(m_viewVector, 0), vec4(0, 0, 0, 1));
	//viewMat = translate(viewMat, m_position);
	//viewMat = mat4_cast(rot) * viewMat;

	//m_rightVector = vec3(column(viewMat, 0));
	//m_upVector = vec3(column(viewMat, 1));
	//m_viewVector = vec3(column(viewMat, 2));

	//mat4 rola(vec4(m_rightVector, 0), vec4(m_upVector, 0), vec4(m_viewVector, 0), vec4());

	//m_viewVector = vec3(0, 0, -1);
	//m_rightVector = normalize(cross(m_viewVector, vec3(0, 1, 0)));
	//m_upVector = normalize(cross(m_rightVector, m_viewVector));

	//m_rightVector = normalize(rotate(rot, m_rightVector));
	//m_upVector = normalize(rotate(rot, m_upVector));
	//m_viewVector = normalize(rotate(rot, m_viewVector));

	//mat4 rotator = mat4_cast(quat(vec3(radians(roll), radians(yaw), radians(pitch))));
	//m_viewVector = normalize(vec3(rotator * vec4(m_viewVector, 0)));
	//m_rightVector = normalize(vec3(rotator * vec4(m_rightVector, 0)));
	//m_upVector = normalize(vec3(rotator * vec4(m_upVector, 0)));
}

void Camera::ZoomCamera(float amount)
{

}

glm::quat Camera::GetCameraRotation()
{
	return quat(mat3(m_viewMatrix));
}

void Camera::SetPosition(vec3 position)
{
	m_position = position;
}

void Camera::Transform(mat3 transformation)
{
	m_rightVector = column(transformation, 0);
	m_upVector = column(transformation, 1);
	m_viewVector = column(transformation, 2);
}