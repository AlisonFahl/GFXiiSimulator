#include "ResourceManager.h"
#include "GLEW\include\glew.h"
#include "AssetImport\FreeImage\FreeImage.h"
#include "Crate.h"
#include "Sunflower.h"
#include "Tree.h"

using namespace std;

ResourceManager* ResourceManager::m_singleton = nullptr;

void ResourceManager::Initialize()
{
	m_singleton = new ResourceManager();

	m_singleton->LoadTextures();
	m_singleton->LoadShaders();
	m_singleton->LoadMeshes();
}

ResourceManager* ResourceManager::GetInstance()
{
	return m_singleton;
}

void ResourceManager::Destroy()
{
	delete m_singleton;
}

void ResourceManager::LoadTextures()
{
	OGLTexture* tex_aux;
	FIBITMAP* img_aux;

	//Texture House
	tex_aux = new OGLTexture();
	tex_aux->CreateTextureFromFile("../asset/texture/house_diffuse.tga");
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("house", shared_ptr<OGLTexture>(tex_aux)));

	tex_aux = new OGLTexture();
	tex_aux->CreateTextureFromFile("../asset/texture/house_normal.tga");
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("houseNormal", shared_ptr<OGLTexture>(tex_aux)));

	tex_aux = new OGLTexture();
	tex_aux->CreateTextureFromFile("../asset/texture/house_spec.tga");
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("houseSpecular", shared_ptr<OGLTexture>(tex_aux)));

	FreeImage_Initialise();

	//Textures Skybox
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_bk.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_bk", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_bt.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_bt", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_ft.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_ft", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_lt.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_lt", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_rt.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_rt", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_TARGA, "../asset/texture/sky_tp.tga");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sky_tp", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Terrain
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_PNG, "../asset/texture/grass.png");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("grass", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Sunflower
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_PNG, "../asset/texture/sunflower.png");
	img_aux = FreeImage_ConvertTo32Bits(img_aux);//If it is not a Power of Two image
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("sunflower", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Crate
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_PNG, "../asset/texture/wooden_crate.png");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("crate", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Lava
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_JPEG, "../asset/texture/lava.jpg");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGR);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("lava", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Volcano
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_JPEG, "../asset/texture/volcano.jpg");
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGR);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("volcano", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);

	//Texture Tree
	tex_aux = new OGLTexture();
	img_aux = FreeImage_Load(FIF_PNG, "../asset/texture/tree.png");
	img_aux = FreeImage_ConvertTo32Bits(img_aux);//If it is not a Power of Two image
	tex_aux->CreateTextureFromData(FreeImage_GetBits(img_aux), GL_UNSIGNED_BYTE, FreeImage_GetWidth(img_aux), FreeImage_GetHeight(img_aux), GL_BGRA);
	m_textures.insert(pair<string, shared_ptr<OGLTexture>>("tree", shared_ptr<OGLTexture>(tex_aux)));
	FreeImage_Unload(img_aux);
}

void ResourceManager::LoadShaders()
{
	OGLShaderProgram* shader_aux;

	// Baisc Shader
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "inNormal");
	shader_aux->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("basic", shared_ptr<OGLShaderProgram>(shader_aux)));

	// Shader House
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/house.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/house.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "inNormal");
	shader_aux->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("house", shared_ptr<OGLShaderProgram>(shader_aux)));

	//Shader Sky Box
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky_box.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky_box.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "inUV");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("sky_box", shared_ptr<OGLShaderProgram>(shader_aux)));

	//Shader Terrain
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/terrain.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/terrain.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "textureCoord");
	shader_aux->BindAttributeLocation(2, "normal");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("terrain", shared_ptr<OGLShaderProgram>(shader_aux)));

	// Shader Sunflower
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sunflower.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sunflower.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "Normal");
	shader_aux->BindAttributeLocation(2, "textureCoord");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("sunflower", shared_ptr<OGLShaderProgram>(shader_aux)));

	//Shader Crate
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/crate.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/crate.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "normal");
	shader_aux->BindAttributeLocation(2, "textureCoord");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("crate", shared_ptr<OGLShaderProgram>(shader_aux)));

	//Shader Lava
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/lava.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/lava.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "texCoord");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("lava", shared_ptr<OGLShaderProgram>(shader_aux)));

	//Shader Tree
	shader_aux = new OGLShaderProgram();

	shader_aux->CreateShaderProgram();
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/tree.vert", SHADER_VERTEX);
	shader_aux->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/tree.frag", SHADER_FRAGMENT);

	shader_aux->BindAttributeLocation(0, "position");
	shader_aux->BindAttributeLocation(1, "texCoord");

	glBindFragDataLocation(shader_aux->GetProgramHandle(), 0, "outFrag");

	shader_aux->BuildShaderProgram();

	m_shaders.insert(pair<string, shared_ptr<OGLShaderProgram>>("tree", shared_ptr<OGLShaderProgram>(shader_aux)));
}

void ResourceManager::LoadMeshes()
{
	//Mesh House
	auto houseMesh = make_shared<Mesh>();
	houseMesh->LoadAndBuildMeshFromOBJFileEx(L"../asset/models/house.obj");
	m_meshes.insert(pair<string, shared_ptr<Mesh>>("house", houseMesh));

	//Mesh Plane
	m_meshes.insert(pair<string, shared_ptr<Mesh>>("plane", make_shared<Mesh>(L"../asset/models/space_frigate_6.obj")));

	//Mesh Crate
	Crate::LoadMesh();

	//Mesh Sunflower
	Sunflower::LoadMesh();

	//Mesh Tree
	Tree::LoadMesh();
}

std::shared_ptr<OGLTexture> ResourceManager::GetTexture(std::string name)
{
	return m_textures[name];
}

std::shared_ptr<OGLShaderProgram> ResourceManager::GetShader(std::string name)
{
	return m_shaders[name];
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(std::string name)
{
	return m_meshes[name];
}