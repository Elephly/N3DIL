#ifndef MYGEOMETRY_H
#define MYGEOMETRY_H

#include "MyGraphics.h"

class MyPoint2D
{
public:
	MyPoint2D(float x = 0.0f, float y = 0.0f);
	~MyPoint2D();

	// Getters
	float GetX() const;
	float GetY() const;
	const float &GetXAddr() const;
	const float &GetYAddr() const;

	// Setters
	void SetX(float const & x);
	void SetY(float const & y);

protected:
	float x_;
	float y_;
};

class MyPoint3D
{
public:
	MyPoint3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	~MyPoint3D();

	// Getters
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	const float &GetXAddr() const;
	const float &GetYAddr() const;
	const float &GetZAddr() const;

	// Setters
	void SetX(float const & x);
	void SetY(float const & y);
	void SetZ(float const & z);

protected:
	float x_;
	float y_;
	float z_;
};

class MyPoint4D
{
public:
	MyPoint4D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	~MyPoint4D();

	// Getters
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;
	const float &GetXAddr() const;
	const float &GetYAddr() const;
	const float &GetZAddr() const;
	const float &GetWAddr() const;

	// Setters
	void SetX(float const & x);
	void SetY(float const & y);
	void SetZ(float const & z);
	void SetW(float const & w);

protected:
	float x_;
	float y_;
	float z_;
	float w_;
};

class MyRectangle
{
public:
	MyRectangle(int x = 0, int y = 0, int width = 0, int height = 0);
	~MyRectangle();
		
	bool IsZeroDimensional();

	int GetX1(), GetY1(), GetX2(), GetY2(), GetWidth(), GetHeight();
	void SetX1(int x), SetY1(int y), SetX2(int x), SetY2(int y), SetWidth(int width), SetHeight(int height);

	char *ToString();

private:
	int x_, y_, width_, height_;
	char stringRectangle[64];
};

class MyCircle
{
public:
	MyCircle(int x = 0, int y = 0, double radius = 0.0);
	~MyCircle();

	bool IsZeroDimensional();

	int GetCenterX(), GetCenterY();
	double GetRadius(), GetDiameter();
	void  SetCenterX(int x), SetCenterY(int y), SetRadius(double radius), SetDiameter(double diameter);

	char *ToString();

private:
	int x_, y_;
	double radius_;
	char stringCircle[64];
};

#endif // MYGEOMETRY_H