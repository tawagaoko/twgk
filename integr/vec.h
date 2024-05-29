#pragma once
#include <iostream>
#include <cmath>
class Vector {
public:
	double& operator[](int i);
	Vector operator()(double x, double y, double z);
	Vector operator()(Vector a, Vector b);
	double operator*(Vector a);
	Vector operator*(double n);
	Vector operator+(Vector v);
	Vector(double x, double y, double z);
	void show() const;
	Vector();
private:
	double coords[3];
};

double abs_vec(Vector a);