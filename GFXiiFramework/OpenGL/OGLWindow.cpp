#include "OGLWindow.h"
#include "../Resource.h"
#include "../GLEW/include/glew.h"
#include "../PerspectiveCamera.h"
#include "../ResourceManager.h"
#include <vector>
#include <chrono>
#include <random>

OGLWindow::OGLWindow()
{
	m_lastTick = 0;
	m_lastMouseX = 0;
	m_lastMouseY = 0;
	rotating = false;
}

OGLWindow::~OGLWindow()
{
	//Free Resources
	ResourceManager::Destroy();

	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers( 1, (GLuint*)(&m_texDefaultSampler) );

	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;

	glewInit();

	InitOGLState();

	m_width = width;
	m_height = height;

	//Load Resources
	ResourceManager::Initialize();
	auto resourceManager = ResourceManager::GetInstance();

	//Create Scene
	m_scene = std::make_shared<Scene>();
	Scene::SetCamera(std::make_shared<PerspectiveCamera>(width / (float)height, 60, 1, 1000));

	//Create Objects
	m_fighter = std::make_shared<Fighter>();

	m_terrain = std::make_shared<Terrain>(5, 250);

	m_volcano = std::make_shared<Volcano>(2, 100);

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand0 generator(seed);
	std::vector<glm::vec2> spawnLocations;
	for (int i = 64; i <= (5 * 512 - 64); i += 64)
	{
		for (int j = 64; j <= (5 * 512 - 64); j += 64)
		{
			auto spawnX = (-5 * 512 / 2) + i;
			auto spawnY = (-5 * 512 / 2) + j;
			if (!(spawnX > -200 && spawnX < 200 && spawnY > -200 && spawnY < 200))
			{
				spawnLocations.push_back(glm::vec2(spawnX, spawnY));
			}
		}
	}
	for (int i = 0; i < 12; i++)
	{
		int selected = (generator() % spawnLocations.size());
		auto location = spawnLocations[selected];
		m_farms.push_back(std::make_shared<Farm>(glm::vec3(location.x, m_terrain->GetHeight(location), location.y), 0, std::bind(&Terrain::GetHeight, &(*m_terrain), std::placeholders::_1), std::bind(&Fighter::GetPosition, &(*m_fighter))));
		spawnLocations.erase(spawnLocations.begin() + selected);
	}
	
	//Populate Scene
	m_scene->AttachActor(std::make_shared<SkyBox>());
	m_scene->AttachActor(m_terrain);
	m_scene->AttachActor(m_volcano);
	m_scene->AttachActor(std::make_shared<Lava>(glm::vec3(-16, 70, 12), 60, std::bind(&Terrain::GetHeight, &(*m_terrain), std::placeholders::_1)));
	m_scene->AttachActor(m_fighter);
	for (auto& farm : m_farms)
	{
		m_scene->AttachActor(farm);
	}
	for (int i = 0; i < 3000; i++)
	{
		int selected = (generator() % spawnLocations.size());
		auto location = spawnLocations[selected];
		float selectedX = location.x + (((int)(generator() % 64)) - 32);
		float selectedZ = location.y + (((int)(generator() % 64)) - 32);

		m_scene->AttachActor(std::make_shared<Tree>(glm::vec3(selectedX, m_terrain->GetHeight(glm::vec2(selectedX, selectedZ)), selectedZ)));
	}

	return TRUE;
}

void OGLWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindSampler(0, m_texDefaultSampler);
	glBindSampler(1, m_texNormalSampler);
	glBindSampler(2, m_texSpecSampler);

	auto tick = GetTickCount();
	float timeElapsed = 0;
	//Check if it is the first frame
	if (m_lastTick == 0)
	{
		m_lastTick = tick;
	}
	else
	{
		timeElapsed = (tick - m_lastTick) / 1000.0f;
	}
	if (timeElapsed > 0)
	{
		m_lastTick = tick;

		//Forward
		if (GetAsyncKeyState(0x57) & 0x8000)
		{
			m_fighter->GoingForward(true);
			//cam->DollyCamera(timeElapsed * -3.0f);
		}
		else
		{
			m_fighter->GoingForward(false);
		}

		//Backward
		if (GetAsyncKeyState(0x53) & 0x8000)
		{
			m_fighter->GoingBackward(true);
			//cam->DollyCamera(timeElapsed * 3.0f);
		}
		else
		{
			m_fighter->GoingBackward(false);
		}

		//Right
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			m_fighter->GoingRight(true);
			//cam->StrafeCamera(timeElapsed * 3.0f);
		}
		else
		{
			m_fighter->GoingRight(false);
		}

		//Left
		if (GetAsyncKeyState(0x41) & 0x8000)
		{
			m_fighter->GoingLeft(true);
			//cam->StrafeCamera(timeElapsed * -3.0f);
		}
		else
		{
			m_fighter->GoingLeft(false);
		}

		//Down
		if (GetAsyncKeyState(0x5A) & 0x8000)
		{
			m_fighter->GoingDown(true);
			//cam->PedCamera(timeElapsed * -3.0f);
		}
		else
		{
			m_fighter->GoingDown(false);
		}

		//Up
		if (GetAsyncKeyState(0x58) & 0x8000)
		{
			m_fighter->GoingUp(true);
			//cam->PedCamera(timeElapsed * 3.0f);
		}
		else
		{
			m_fighter->GoingUp(false);
		}

		//Roll Left
		if (GetAsyncKeyState(0x51) & 0x8000)
		{
			m_fighter->RollingLeft(true);
		}
		else
		{
			m_fighter->RollingLeft(false);
		}

		//Roll Right
		if (GetAsyncKeyState(0x45) & 0x8000)
		{
			m_fighter->RollingRight(true);
		}
		else
		{
			m_fighter->RollingRight(false);
		}

		//Yaw Left
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_fighter->YawingLeft(true);
		}
		else
		{
			m_fighter->YawingLeft(false);
		}

		//Yaw Right
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_fighter->YawingRight(true);
		}
		else
		{
			m_fighter->YawingRight(false);
		}

		//Pitch Down
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_fighter->PitchingDown(true);
		}
		else
		{
			m_fighter->PitchingDown(false);
		}

		//Pitch Up
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_fighter->PitchingUp(true);
		}
		else
		{
			m_fighter->PitchingUp(false);
		}

		//Drop Crate
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			auto spawnLocation = m_fighter->GetPosition();
			auto florHeight = m_terrain->GetHeight(glm::vec2(spawnLocation.x, spawnLocation.z));

			auto crate = std::make_shared<Crate>(spawnLocation, florHeight, [this](glm::vec2 cratePosition) 
			{
				for (auto& farm : m_farms)
				{
					if (!farm->GetAided())
					{
						if (farm->isInsideAABB(cratePosition, glm::vec2(1, 1)))
						{
							farm->SetAided(true);
							m_farmsAided++;
							printf("Well Drone! You have aided a farm.\n");
							printf("Total Farms Aided: %d.\nRemaining: %d.\n\n", m_farmsAided, 12 - m_farmsAided);
						}
					}
				}
			});
			m_crates.push_back(crate);

			m_scene->AttachActor(crate);
		}


		//cam->UpdateViewMatrix();

		m_scene->Update(timeElapsed);
	}
	m_scene->Render();

	SwapBuffers(m_hdc);
}

void OGLWindow::Resize( int width, int height )
{
	glViewport( 0, 0, width, height );

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Create a texture sampler
	glGenSamplers( 1, (GLuint*)(&m_texDefaultSampler) );
	
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_S, GL_REPEAT);  
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MIN_FILTER , GL_LINEAR);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MAG_FILTER , GL_LINEAR);

	//Create a normal sampler
	glGenSamplers(1, (GLuint*)(&m_texNormalSampler));

	glSamplerParameteri(m_texNormalSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_texNormalSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_texNormalSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_texNormalSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Create a specular sampler
	glGenSamplers(1, (GLuint*)(&m_texSpecSampler));

	glSamplerParameteri(m_texSpecSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_texSpecSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_texSpecSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_texSpecSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Create a skybox sampler
	glGenSamplers(1, (GLuint*)(&m_skyBoxSampler));

	glSamplerParameteri(m_skyBoxSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_skyBoxSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_skyBoxSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_skyBoxSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	rotating = true;

	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	rotating = false;

	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	auto movementX = x - m_lastMouseX;
	auto movementY = y - m_lastMouseY;

	if (rotating && (movementX != 0 || movementY != 0))
	{
		m_cameraRotation += glm::vec3(movementX / 20.0f, 0, movementY / 10.0f);
		//cam->RotateCamera(m_cameraRotation.x, m_cameraRotation.y, m_cameraRotation.z);
		//cam->UpdateViewMatrix();
	}

	m_lastMouseX = x;
	m_lastMouseY = y;

	return TRUE;
}
