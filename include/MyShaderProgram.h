#ifndef MYSHADERPROGRAM_H
#define MYSHADERPROGRAM_H

#include "MyMatrix.h"

class MyShaderProgram
{
public:
	MyShaderProgram(const char *vertShaderFileName = 0, const char *fragShaderFileName = 0);
	~MyShaderProgram();

	void InitializeShaderProgram(const char *vertShaderFileName = 0, const char *fragShaderFileName = 0);

	// Getters
	unsigned int GetShaderProgram();

	int BindUniformMatrix(MyMatrix4 const & matrix, const char *uniformName);
	int BindUniformVector(MyVector2D const & vector, const char *uniformName);
	int BindUniformVector(MyVector3D const & vector, const char *uniformName);
	int BindUniformVector(MyVector4D const & vector, const char *uniformName);
	int BindUniformFloat(float const & f, const char *uniformName);
	int BindUniformInt(int const & i, const char *uniformName);

private:
	char *vertexShaderFileName;
	char *fragmentShaderFileName;
	unsigned int shaderProgram;
};

#endif // MYSHADERPROGRAM_H
