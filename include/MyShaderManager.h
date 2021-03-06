#ifndef MYSHADERMANAGER_H
#define MYSHADERMANAGER_H

#include "MyShaderProgram.h"

#include <map>
#include <vector>

class MyShaderManager
{
public:
	static MyShaderProgram *CreateShader(const char *shaderName, const char *vertShaderFileName, const char *fragShaderFileName);

	static MyShaderProgram *GetShader(const char *shaderName);
	static std::vector<MyShaderProgram *> *GetShaderList();

	static void Cleanup();

private:
	static std::map<const char *, MyShaderProgram *> *shaders;

};

#endif // MYSHADERMANAGER_H
