#include "cell.h"

void Cell::Colloc_point()
{
	for (int i = 0; i < 3; i++) {
		x[i] = (A[i] + B[i] + C[i] + D[i]) / 4;
	}
}

void Cell::Normal()
{
	Vector b, d;
	b(C[0] - A[0], C[1] - A[1], C[2] - A[2]);
	d(D[0] - B[0], D[1] - B[1], D[2] - B[2]);
	n1(b, d);
	n(n1[0] / abs_vec(n1), n1[1] / abs_vec(n1), n1[2] / abs_vec(n1));
}

void Cell::Basis()
{
	t1(C[0] - A[0], C[1] - A[1], C[2] - A[2]);
	t1(t1[0] / abs_vec(t1), t1[1] / abs_vec(t1), t1[2] / abs_vec(t1));
	t2(n, t1);
}

void Cell::Square()
{
	s = abs_vec(n1) / 2;
}

Cell::Cell(Dot x1, Dot x2, Dot x3, Dot x4)
{
	A = x1;;
	B = x2;
	C = x3;
	D = x4;

	Colloc_point();
	Normal();
	Basis();
	Square();
}

Cell Cell::operator()(Dot x1, Dot x2, Dot x3, Dot x4)
{
	A = x1;;
	B = x2;
	C = x3;
	D = x4;
	return *this;
}

double& Dot::operator[](int i)
{
	int k = i % 3;
	return coords[k];
}
Dot::Dot(double x, double y, double z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}
Dot::Dot()
{
}
Dot Dot::operator()(double x, double y, double z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
	return *this;
}

Dot Dot::operator=(Dot d)
{
	coords[0] = d[0];
	coords[1] = d[1];
	coords[2] = d[2];
	return *this;;
}

Dot Dot::operator+(Dot d)
{
	Dot res;
	res[0] = coords[0] + d[0];
	res[1] = coords[1] + d[1];
	res[2] = coords[2] + d[2];
	return res;
}

Dot Dot::operator-(Dot d)
{
	Dot res;
	res[0] = coords[0] - d[0];
	res[1] = coords[1] - d[1];
	res[2] = coords[2] - d[2];
	return res;
}

Dot Dot::operator*(double n)
{
	Dot res;
	res[0] = coords[0] * n;
	res[1] = coords[1] * n;
	res[2] = coords[2] * n;
	return res;
}

void Dot::show() const
{
	std::cout << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")\n";
}
