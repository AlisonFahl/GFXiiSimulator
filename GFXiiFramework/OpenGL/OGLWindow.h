#pragma once

#include "../RenderWindow.h"
#define GLM_FORCE_RADIANS

#include "OGLCube.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "../Scene.h"
#include "../Mesh.h"
#include "../House.h"
#include "../SkyBox.h"
#include "../Fighter.h"
#include "../Terrain.h"
#include "../Sunflower.h"
#include "../Farm.h"
#include "../Crate.h"
#include "../Lava.h"
#include "../Volcano.h"
#include "../Tree.h"

class OGLWindow : public RenderWindow
{
	private:
		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		int			m_width;
		int			m_height;

		//Texture Sampler
		int						m_texDefaultSampler;
		int						m_texNormalSampler;
		int						m_texSpecSampler;
		int						m_skyBoxSampler;

		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Terrain> m_terrain;
		std::shared_ptr<Volcano> m_volcano;
		std::shared_ptr<Fighter> m_fighter;
		std::vector<std::shared_ptr<Farm>> m_farms;
		std::vector<std::shared_ptr<Crate>> m_crates;

		int m_farmsAided = 0;

		unsigned long m_lastTick;

		int m_lastMouseX, m_lastMouseY;
		glm::vec3 m_cameraRotation;
		bool rotating;

protected:

		HGLRC CreateOGLContext (HDC hdc);
		BOOL DestroyOGLContext();
		void InitOGLState();

	public:
					OGLWindow();
					OGLWindow(HINSTANCE hInstance, int width, int height);
					~OGLWindow();
		
		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);

		void		Render();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		BOOL		MouseLBDown ( int x, int y );
		BOOL		MouseLBUp ( int x, int y );
		BOOL		MouseMove ( int x, int y );
};
