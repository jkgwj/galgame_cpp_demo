#pragma once
#include<math.h>

class Vector2D
{
public:
	double x, y;
public:
	Vector2D() = default;
	Vector2D(double x, double y) :x(x), y(y) {};
	~Vector2D() = default;

	double length() { return sqrt(x * x + y * y); }//获取向量长度
	Vector2D normaliza() {
		double len = length();
		if (len == 0)return Vector2D(0, 0);
		return Vector2D(x / len, y / len);
	}//获取向量的单位向量

	//运算符重载
	Vector2D operator+(const Vector2D& vec) { return Vector2D(x + vec.x, y + vec.y); }
	void operator+=(const Vector2D& vec) { x += vec.x; y += vec.y; }
	Vector2D operator-(const Vector2D& vec) { return Vector2D(x - vec.x, y - vec.y); }
	void operator-=(const Vector2D& vec) { x -= vec.x; y -= vec.y; }
	double operator*(const Vector2D& vec) { return (x * vec.x + y * vec.y); }
	Vector2D operator*(double val) { return Vector2D(x * val, y * val); }
	void operator*=(double val) { x *= val, y *= val; }

};