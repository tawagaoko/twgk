#pragma once
#include <iostream>
#include <vector>
#include "vec.h"
class Dot {
public:
	double& operator[](int i);
	Dot (double x, double y, double z);
	Dot();
	Dot operator ()(double x, double y, double z);
	Dot operator =(Dot d);
	Dot operator +(Dot d);
	Dot operator -(Dot d);
	Dot operator *(double n);
	void show() const;
private:
	double coords[3];
};
class Cell {
public:
	Cell(Dot x1, Dot x2, Dot x3, Dot x4);
	Cell operator ()(Dot x1, Dot x2, Dot x3, Dot x4);
	Vector n;
	Vector n1;
	Vector t1;
	Vector t2;
	double s;
	Dot x;
	void Colloc_point();
	void Normal();
	void Basis();
	void Square();
private:
	Dot A;
	Dot B;
	Dot C;
	Dot D;
};
