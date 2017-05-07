//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
	protected:
		glm::mat4						m_viewMatrix;				//View Matrix
		glm::mat4						m_projectionMatrix;			//Projection Matrix
		glm::vec3						m_position;					//Position
		glm::vec3						m_upVector;					//up vector
		glm::vec3						m_rightVector;				//right vector
		glm::vec3						m_viewVector;				//view vector
		float							m_aspectRatio;				//
		float							m_near;
		float							m_far;

	public:
	
		Camera();
		virtual ~Camera();

		inline const glm::vec3 GetCameraPosition() const 
		{
			return m_position;
		}

		void LookAtPoint(const glm::vec3& point);

		inline const glm::vec3 GetCameraUpVector() const
		{
			return m_upVector;
		}

		inline const glm::vec3 GetViewVector() const
		{
			return m_viewVector;
		}

		inline float GetCameraAspectRatio()
		{
			return m_aspectRatio;
		}

		inline float GetNear()
		{
			return m_near;
		}

		inline float GetFar()
		{
			return m_far;
		}

		void UpdateViewMatrix();

		virtual void UpdateProjectionMatrix() = 0;

		inline const glm::mat4* GetViewMatrix() const
		{
			return &m_viewMatrix;
		}

		inline const glm::mat4* GetProjectionMatrix() const
		{
			return &m_projectionMatrix;
		}

		glm::quat GetCameraRotation();

		void SetPosition(glm::vec3 position);

		void Transform(glm::mat3 transformation);

		//TODO: Implement the following camera movement
		void							StrafeCamera(float amount);
		void							DollyCamera(float amount);
		void							PedCamera(float amount);
		void							RotateCamera(float yaw, float pitch, float roll);
		void							ZoomCamera(float amount);
};

#endif
