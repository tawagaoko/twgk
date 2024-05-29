#include "vec.h"

double& Vector::operator[](int i)
{
	int k = i % 3;
	return coords[k];
}
Vector Vector::operator()(double x, double y, double z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	return *this;
}
Vector Vector::operator()(Vector a, Vector b) {
	coords[0] = a[1] * b[2] - a[2] * b[1];
	coords[1] = -a[0] * b[2] + a[2] * b[0];
	coords[2] = a[0] * b[1] - a[1] * b[0];
	return *this;
}

double Vector::operator*(Vector a) {
	double res = 0;
	for (int i = 0; i < 3; i++) {
		res += coords[i] * a[i];
	}
	return res;
}
Vector Vector::operator*(double n)
{
	coords[0] *= n;
	coords[1] *= n;
	coords[2] *= n;
	return *this;
}
Vector Vector::operator+(Vector v)
{
	coords[0] += v[0];
	coords[1] += v[1];
	coords[2] += v[2];
	return *this;
}
Vector::Vector(double x, double y, double z) {
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}
void Vector::show () const {
	std::cout << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")\n";
}
Vector::Vector() {
	for (int i = 0; i < 3; i++) {
		coords[i] = 0;
	}
}

double abs_vec(Vector a)
{
	return sqrt(a*a);
}
