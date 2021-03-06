#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "MyCamera.h"
#include "MyIncludes.h"
#include "MyInputManager.h"
#include "MyLightSource.h"
#include "MyManikin.h"
#include "MyMaterial.h"
#include "MyMeshFactory.h"
#include "MyShaderManager.h"

class MyApplication
{
public:
	MyApplication(const char *name = "My Application");
	~MyApplication();

	void Initialize(int *argc, char **argv);
	void LoadContent();
	void Update(float const & deltaTime);
	void Draw();

	void Run();

	// Getters
	char *GetName();
	int GetWindowWidth();
	int GetWindowHeight();

	// Callback Function Registers
	void RegisterReshapeFunc(void(*callback)(int width, int height));
	void RegisterTimerFunc(void(*callback)(int operation));
	void RegisterDisplayFunc(void(*callback)());
	void RegisterKeyboardFunc(void(*callback)(unsigned char key, int x, int y));
	void RegisterKeyboardUpFunc(void(*callback)(unsigned char key, int x, int y));
	void RegisterMouseEntryFunc(void(*callback)(int state));
	void RegisterMouseFunc(void(*callback)(int button, int state, int x, int y));
	void RegisterMouseMoveFunc(void(*callback)(int x, int y));
	void RegisterMouseMovePassiveFunc(void(*callback)(int x, int y));
	void RegisterSpecialFunc(void(*callback)(int key, int x, int y));
	void RegisterSpecialUpFunc(void(*callback)(int key, int x, int y));

	// Callback Functions
	void ReshapeFunc(int width, int height);
	void TimerFunc(int operation);
	void DisplayFunc();
	void KeyboardFunc(unsigned char key, int x, int y);
	void KeyboardUpFunc(unsigned char key, int x, int y);
	void MouseEntryFunc(int state);
	void MouseFunc(int button, int state, int x, int y);
	void MouseMoveFunc(int x, int y);
	void MouseMovePassiveFunc(int x, int y);
	void SpecialFunc(int key, int x, int);
	void SpecialUpFunc(int key, int x, int);

private:
	// Application Specific Information
	char *applicationName;
	int windowWidth;
	int windowHeight;
	int windowID;
	int totalElapsedTime;
	int elapsedTime;

	// Input Manager
	MyInputManager *inputManager;

	// Textures
	MyTexture2D *catTexture;
	MyTexture2D *tuxTexture;

	// Materials
	MyMaterial *shinyMaterial;
	MyMaterial *shellyMaterial;
	MyMaterial *catMaterial;
	MyMaterial *tuxMaterial;
	MyMaterial *tuxShoeMaterial;
	MyMaterial *tuxTorsoMaterial;

	// Lights
	MyLightSource primaryLightSource;

	// Camera
	MyCamera *camera;

	// Objects
	MyManikin *testManikin;
	std::vector<MyManikin *> manikinArmy;

	// Private Functions
	void ShadersUpdateLightSource();
	void ShadersUpdateCameraMatrix();
	void ShadersUpdateProjectionMatrix();

	// Arbitrary Values
	float cameraMoveSpeed;
	float cameraRotateSpeed;
	bool renderWireFrame;
	int numManikins;
	bool manikinsExist;

	// Temp Functions
	void toggleSomeManikins();
	void killSomeManikins();

};

#endif // MYAPPLICATION_H
