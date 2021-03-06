#include "MyApplication.h"

#include "MyMatrix.h"
#include "MyStringUtil.h"

#include <GL/glew.h>
#include <GL/glut.h>

#if (MY_DEBUG == 1)
#include <iostream>
#endif // MY_DEBUG

using namespace std;

MyApplication::MyApplication(const char * name)
{
	cameraMoveSpeed = 4.0f;
	cameraRotateSpeed = 60.0f;
	renderWireFrame = false;

	applicationName = MyStringUtil::CopyString(name);
	windowWidth = 0;
	windowHeight = 0;
	windowID = 0;
	totalElapsedTime = 0;
	elapsedTime = 0;
	inputManager = new MyInputManager();

	std::string path = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("images").append(SYSTEM_DELIM).append("sample2.png");
	catTexture = new MyTexture2D(path.c_str());
	path = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("images").append(SYSTEM_DELIM).append("TuxTorso.png");
	tuxTexture = new MyTexture2D(path.c_str());

	shinyMaterial = new MyMaterial(0, MyColorRGBA(0.25f, 0.25f, 0.25f), MyColorRGBA(0.75f, 0.75f, 0.75f), MyColorRGBA(0.5f, 0.5f, 0.5f), 128.0f);
	MyColorRGBA shellyDiffuse = MyColorRGBA(77.0f / 255.0f, 126.0f / 255.0f, 17.0f / 255.0f);
	MyColorRGBA shellyAmbient = MyColorRGBA(shellyDiffuse.GetRed() * 0.25f, shellyDiffuse.GetGreen() * 0.25f, shellyDiffuse.GetBlue() * 0.25f);
	shellyMaterial = new MyMaterial(0, shellyAmbient, shellyDiffuse, MyColorRGBA(0.5f, 0.5f, 0.5f), 128.0f);
	catMaterial = new MyMaterial(catTexture, MyColorRGBA(0.25f, 0.25f, 0.25f), MyColorRGBA(1.0f, 1.0f, 1.0f), MyColorRGBA(0.5f, 0.5f, 0.5f), 128.0f);
	MyColorRGBA shellyTuxDiffuse = MyColorRGBA(60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f);
	MyColorRGBA shellyTuxAmbient = MyColorRGBA(shellyTuxDiffuse.GetRed() * 0.25f, shellyTuxDiffuse.GetGreen() * 0.25f, shellyTuxDiffuse.GetBlue() * 0.25f);
	tuxMaterial = new MyMaterial(0, shellyTuxAmbient, shellyTuxDiffuse, MyColorRGBA(0.5f, 0.5f, 0.5f), 128.0f);
	tuxShoeMaterial = new MyMaterial(0, MyColorRGBA(0.0f, 0.0f, 0.0f), MyColorRGBA(0.1f, 0.1f, 0.1f), MyColorRGBA(0.5f, 0.5f, 0.5f), 256.0f);
	tuxTorsoMaterial = new MyMaterial(tuxTexture, MyColorRGBA(0.25f, 0.25f, 0.25f), MyColorRGBA(1.0f, 1.0f, 1.0f), MyColorRGBA(0.5f, 0.5f, 0.5f), 128.0f);

	primaryLightSource = MyLightSource(100.0f, 100.0f, 100.0f);

	MyVector3D cameraPosition(0.0f, 0.0f, 0.0f);
	MyVector3D cameraLookAt(0.0f, 0.0f, -1.0f);
	MyVector3D cameraUpVector(0.0f, 1.0f, 0.0f);
	MyMatrix4 projectionMatrix = MyMatrix4::SymmetricPerspectiveProjectionMatrix(30.0f, (float)PROJECT_ASPECT_RATIO_X / (float)PROJECT_ASPECT_RATIO_Y, 0.1f, 1000.0f);
	camera = new MyCamera(cameraPosition, cameraLookAt, cameraUpVector, projectionMatrix, true);

	testManikin = new MyManikin();

	numManikins = 16;
	manikinsExist = false;
}

MyApplication::~MyApplication()
{
	MyDelete(testManikin);
	killSomeManikins();

	MyMeshFactory::Cleanup();

	MyDelete(camera);

	MyDelete(shinyMaterial);
	MyDelete(shellyMaterial);
	MyDelete(catMaterial);
	MyDelete(tuxMaterial);
	MyDelete(tuxShoeMaterial);
	MyDelete(tuxTorsoMaterial);

	MyDelete(catTexture);
	MyDelete(tuxTexture);

	MyShaderManager::Cleanup();

	if (windowID != 0)
	{
		glutDestroyWindow(windowID);
	}
	MyDelete(inputManager);
	MyDeleteArray(applicationName);
}

void MyApplication::Initialize(int *argc, char **argv)
{
	glutInit(argc, argv);
	windowWidth = (int)(0.75f * (float)glutGet(GLUT_SCREEN_WIDTH));
	windowHeight = (int)((float)windowWidth / ((float)PROJECT_ASPECT_RATIO_X / (float)PROJECT_ASPECT_RATIO_Y));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((int)((float)(glutGet(GLUT_SCREEN_WIDTH) - windowWidth) / 2.0f),
		(int)((float)(glutGet(GLUT_SCREEN_HEIGHT) - windowHeight) / 4.0f));
	windowID = glutCreateWindow(applicationName);
	totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if (GLenum err = glewInit() != GLEW_OK)
	{
		throw glewGetErrorString(err);
	}

	std::string vertPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("ColorVert.glsl");
	std::string fragPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("ColorFrag.glsl");
	MyShaderProgram *colorShader = MyShaderManager::CreateShader("ColorShader", vertPath.c_str(), fragPath.c_str());
	vertPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("GouraudVert.glsl");
	fragPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("GouraudFrag.glsl");
	MyShaderProgram *gouraudShader = MyShaderManager::CreateShader("GouraudShader", vertPath.c_str(), fragPath.c_str());
	vertPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("PhongVert.glsl");
	fragPath = std::string(PROJECT_RESOURCE_DIRECTORY).append(SYSTEM_DELIM).append("shaders").append(SYSTEM_DELIM).append("PhongFrag.glsl");
	MyShaderProgram *phongShader = MyShaderManager::CreateShader("PhongShader", vertPath.c_str(), fragPath.c_str());

	catTexture->InitializeTexture();
	tuxTexture->InitializeTexture();

	camera->Translate(0.0f, 0.0f, 10.0f);

	// Creating geometry
	MyMeshFactory::CreateQuad("Quad");
	MyMeshFactory::CreateSphere("Sphere", 8, 16);

	testManikin->Initialize(phongShader, shellyMaterial, catMaterial, tuxMaterial, tuxTorsoMaterial, tuxShoeMaterial, MyMeshFactory::GetMesh("Sphere"));
	testManikin->SetHeadMaterial(catMaterial);
	testManikin->Translate(0.0f, 0.0f, 15.0f);
	testManikin->Yaw(-90.0f);

	toggleSomeManikins();

	ShadersUpdateLightSource();
	ShadersUpdateCameraMatrix();
	ShadersUpdateProjectionMatrix();
}

void MyApplication::LoadContent()
{
}

void MyApplication::Update(float const & deltaTime)
{
	//printf("FPS: %d\n", (int)(1.0f / deltaTime));
	bool cameraTransformed = false;
	if (inputManager != 0)
	{
		MyVector3D direction = camera->GetDirection();
		MyVector3D right = camera->GetRightVector();
		MyVector3D up = camera->GetUpVector();
		if (inputManager->Keys['W'] == GLUT_DOWN || inputManager->Keys['w'] == GLUT_DOWN)
		{
			camera->Translate(direction * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['A'] == GLUT_DOWN || inputManager->Keys['a'] == GLUT_DOWN)
		{
			camera->Translate(-right * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['S'] == GLUT_DOWN || inputManager->Keys['s'] == GLUT_DOWN)
		{
			camera->Translate(-direction * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['D'] == GLUT_DOWN || inputManager->Keys['d'] == GLUT_DOWN)
		{
			camera->Translate(right * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['Q'] == GLUT_DOWN || inputManager->Keys['q'] == GLUT_DOWN)
		{
			camera->Translate(up * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['C'] == GLUT_DOWN || inputManager->Keys['c'] == GLUT_DOWN)
		{
			camera->Translate(-up * cameraMoveSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->SpecialKeys[GLUT_KEY_UP] == GLUT_DOWN)
		{
			camera->Pitch(cameraRotateSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->SpecialKeys[GLUT_KEY_LEFT] == GLUT_DOWN)
		{
			camera->Yaw(cameraRotateSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->SpecialKeys[GLUT_KEY_DOWN] == GLUT_DOWN)
		{
			camera->Pitch(-cameraRotateSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->SpecialKeys[GLUT_KEY_RIGHT] == GLUT_DOWN)
		{
			camera->Yaw(-cameraRotateSpeed * deltaTime);
			cameraTransformed = true;
		}
		if (inputManager->Keys['i'] == GLUT_DOWN)
		{
			testManikin->Translate(testManikin->GetDirection() * cameraMoveSpeed * deltaTime);
		}
		if (inputManager->Keys['j'] == GLUT_DOWN)
		{
			testManikin->Yaw(cameraRotateSpeed * deltaTime);
		}
		if (inputManager->Keys['k'] == GLUT_DOWN)
		{
			testManikin->Translate(-(MyVector3D)testManikin->GetDirection() * cameraMoveSpeed * deltaTime);
		}
		if (inputManager->Keys['l'] == GLUT_DOWN)
		{
			testManikin->Yaw(-cameraRotateSpeed * deltaTime);
		}
	}
	camera->Update(deltaTime);
	if (cameraTransformed)
	{
		ShadersUpdateCameraMatrix();
	}

	testManikin->Update(deltaTime);

	for (std::vector<MyManikin *>::iterator it = manikinArmy.begin(); it != manikinArmy.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
}

void MyApplication::Draw()
{
	MyColorRGBA c = MyColors::CornflowerBlue;
	glClearColor(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (renderWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	testManikin->Draw();

	for (std::vector<MyManikin *>::iterator it = manikinArmy.begin(); it != manikinArmy.end(); ++it)
	{
		(*it)->Draw();
	}

	glutSwapBuffers();

	glutPostRedisplay();
}

void MyApplication::Run()
{
	glutMainLoop();
}

char * MyApplication::GetName()
{
	return MyStringUtil::CopyString(applicationName);
}

int MyApplication::GetWindowWidth()
{
	return windowWidth;
}

int MyApplication::GetWindowHeight()
{
	return windowHeight;
}

void MyApplication::RegisterReshapeFunc(void(*callback)(int width, int height))
{
	glutReshapeFunc(callback);
}

void MyApplication::RegisterTimerFunc(void(*callback)(int operation))
{
	glutTimerFunc(FRAME_TIME, callback, TIMER_UPDATE_OPERATION);
}

void MyApplication::RegisterDisplayFunc(void(*callback)())
{
	glutDisplayFunc(callback);
}

void MyApplication::RegisterKeyboardFunc(void(*callback)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(callback);
}

void MyApplication::RegisterKeyboardUpFunc(void(*callback)(unsigned char key, int x, int y))
{
	glutKeyboardUpFunc(callback);
}

void MyApplication::RegisterMouseEntryFunc(void(*callback)(int state))
{
	glutEntryFunc(callback);
}

void MyApplication::RegisterMouseFunc(void(*callback)(int button, int state, int x, int y))
{
	glutMouseFunc(callback);
}

void MyApplication::RegisterMouseMoveFunc(void(*callback)(int x, int y))
{
	glutMotionFunc(callback);
}

void MyApplication::RegisterMouseMovePassiveFunc(void(*callback)(int x, int y))
{
	glutPassiveMotionFunc(callback);
}

void MyApplication::RegisterSpecialFunc(void(*callback)(int key, int x, int y))
{
	glutSpecialFunc(callback);
}

void MyApplication::RegisterSpecialUpFunc(void(*callback)(int key, int x, int y))
{
	glutSpecialUpFunc(callback);
}

void MyApplication::ReshapeFunc(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	// use the full view port
	glViewport(0, 0, windowWidth, windowHeight);

	// use 2D  orthographic parallel projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
}

void MyApplication::TimerFunc(int operation)
{
	int previousTime = totalElapsedTime;
	totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = totalElapsedTime - previousTime;
	switch (operation)
	{
	case TIMER_UPDATE_OPERATION:
		Update((float)elapsedTime / 1000.0f);
		break;
	default:
		break;
	}
}

void MyApplication::DisplayFunc()
{
	Draw();
}

void MyApplication::KeyboardFunc(unsigned char key, int x, int y)
{
	if (inputManager != 0)
	{
		if (inputManager->Keys[key] == GLUT_UP)
		{

		}
		else
		{

		}
		inputManager->Keys[key] = GLUT_DOWN;
	}
}

void MyApplication::KeyboardUpFunc(unsigned char key, int x, int y)
{
	if (inputManager != 0)
	{
		if (inputManager->Keys[key] == GLUT_DOWN)
		{
			if (key == ' ')
			{
				testManikin->TogglePlay();
			}
			if (key == 'L')
			{
				testManikin->ToggleLooping();
			}
			if (key == 13)
			{
				testManikin->Stop();
			}
			if (key == '+')
			{
				testManikin->ChangeSpeed(1.0f / 0.9f);
			}
			if (key == '-')
			{
				testManikin->ChangeSpeed(0.9f);
			}
			if (key == 'f')
			{
				renderWireFrame = !renderWireFrame;
			}
			if (key == 'K')
			{
				toggleSomeManikins();
			}
			if (key == 't')
			{
				shinyMaterial->SetToon(!shinyMaterial->GetToon());
				shellyMaterial->SetToon(!catMaterial->GetToon());
				catMaterial->SetToon(!catMaterial->GetToon());
				tuxMaterial->SetToon(!tuxMaterial->GetToon());
				tuxShoeMaterial->SetToon(!tuxShoeMaterial->GetToon());
				tuxTorsoMaterial->SetToon(!tuxTorsoMaterial->GetToon());
			}
		}
		else
		{

		}
		inputManager->Keys[key] = GLUT_UP;
	}
}

void MyApplication::MouseEntryFunc(int state)
{
	switch (state)
	{
	case GLUT_ENTERED:
		break;
	case GLUT_LEFT:
		break;
	default:
		break;
	}
}

void MyApplication::MouseFunc(int button, int state, int x, int y)
{
	int winWidth = glutGet(GLUT_WINDOW_WIDTH);
	int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
	int glX = x;
	int glY = winHeight - y;

	// mouse press event
	if (inputManager->Mouse[button] == GLUT_UP && state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{

		}
		if (button == GLUT_MIDDLE_BUTTON)
		{

		}
		if (button == GLUT_RIGHT_BUTTON)
		{

		}
	}

	// mouse release event
	if (inputManager->Mouse[button] == GLUT_DOWN && state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{

		}
		if (button == GLUT_MIDDLE_BUTTON)
		{

		}
		if (button == GLUT_RIGHT_BUTTON)
		{

		}
	}

	inputManager->Mouse[button] = state;
	inputManager->MouseLocation.SetX((float &)glX);
	inputManager->MouseLocation.SetY((float &)glY);
}

void MyApplication::MouseMoveFunc(int x, int y)
{
	int winWidth = glutGet(GLUT_WINDOW_WIDTH);
	int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
	int glX = x;
	int glY = winHeight - y;

	if (inputManager->Mouse[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	{

	}
	if (inputManager->Mouse[GLUT_MIDDLE_BUTTON] == GLUT_DOWN)
	{

	}
	if (inputManager->Mouse[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
	{

	}
	inputManager->MouseLocation.SetX((float &)glX);
	inputManager->MouseLocation.SetY((float &)glY);
}

void MyApplication::MouseMovePassiveFunc(int x, int y)
{
	int winWidth = glutGet(GLUT_WINDOW_WIDTH);
	int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
	int glX = x;
	int glY = winHeight - y;
	inputManager->MouseLocation.SetX((float &)glX);
	inputManager->MouseLocation.SetY((float &)glY);
}

void MyApplication::SpecialFunc(int key, int x, int)
{
	if (inputManager != 0)
	{
		if (inputManager->SpecialKeys[key] == GLUT_UP)
		{

		}
		else
		{

		}
		inputManager->SpecialKeys[key] = GLUT_DOWN;
	}
}

void MyApplication::SpecialUpFunc(int key, int x, int)
{
	if (inputManager != 0)
	{
		inputManager->SpecialKeys[key] = GLUT_UP;
	}
}

void MyApplication::ShadersUpdateLightSource()
{
	std::vector<MyShaderProgram *> *shaders = MyShaderManager::GetShaderList();
	for (std::vector<MyShaderProgram *>::iterator it = shaders->begin(); it != shaders->end(); ++it)
	{
		(*it)->BindUniformVector(primaryLightSource, "lightPosition");
		(*it)->BindUniformVector(MyVector4D(primaryLightSource.GetColor()), "lightColor");
	}
	MyDelete(shaders);
}

void MyApplication::ShadersUpdateCameraMatrix()
{
	std::vector<MyShaderProgram *> *shaders = MyShaderManager::GetShaderList();
	for (std::vector<MyShaderProgram *>::iterator it = shaders->begin(); it != shaders->end(); ++it)
	{
		(*it)->BindUniformMatrix(camera->GetViewMatrix(), "view");
	}
	MyDelete(shaders);
}

void MyApplication::ShadersUpdateProjectionMatrix()
{
	std::vector<MyShaderProgram *> *shaders = MyShaderManager::GetShaderList();
	for (std::vector<MyShaderProgram *>::iterator it = shaders->begin(); it != shaders->end(); ++it)
	{
		(*it)->BindUniformMatrix(camera->GetProjectionMatrix(), "projection");
	}
	MyDelete(shaders);
}

void MyApplication::toggleSomeManikins()
{
	if (!manikinsExist)
	{
		if (numManikins > 0)
		{
			int iMax = (int)sqrt(numManikins);
			int jMax = numManikins / (int)sqrt(numManikins);
			for (int i = 0; i < iMax; i++)
			{
				for (int j = 0; j < jMax; j++)
				{
					MyManikin *m = new MyManikin(0, MyVector3D(-(2.0f * jMax) + (4.0f * j) + (((i % 2) * 2) - 1), 0.0f, -4.0f * i), MyVector3D(1.0f, 1.0f, 1.0f), MyVector3D(0.0f, 180.0f, 0.0f));
					m->ChangeSpeed(((float)(rand() % 100) / 100.0f) + 0.5f);
					manikinArmy.push_back(m);
				}
			}
		}

		MyShaderProgram *phongShader = MyShaderManager::GetShader("PhongShader");

		for (std::vector<MyManikin *>::iterator it = manikinArmy.begin(); it != manikinArmy.end(); ++it)
		{
			(*it)->Initialize(phongShader, shinyMaterial, 0, 0, 0, 0, MyMeshFactory::GetMesh("Sphere"));
			(*it)->TogglePlay();
		}

		manikinsExist = true;
	}
	else
	{
		killSomeManikins();
	}
}

void MyApplication::killSomeManikins()
{
	for (std::vector<MyManikin *>::iterator it = manikinArmy.begin(); it != manikinArmy.end(); ++it)
	{
		delete(*it);
	}
	manikinArmy.clear();
	manikinsExist = false;
}
