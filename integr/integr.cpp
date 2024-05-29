#include <iostream>
#include "integral.h"
#include "vec.h"
#include "cell.h"

double f(Dot x) {
    return x[0] + 2*x[1];
}

Vector r(Dot x) {
    Vector i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);
    return i * x[0] + j * (x[1] + x[2]) + k;
}

int main()
{
    std::cout << "enter coordinates of 4 dots\nDot 1\n";
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::cin >> x1 >> y1 >> z1;
    std::cout << "\nDot 2\n";
    std::cin >> x2 >> y2 >> z2;
    std::cout << "\nDot 3\n";
    std::cin >> x3 >> y3 >> z3;
    std::cout << "\nDot 4\n";
    std::cin >> x4 >> y4 >> z4;
    Dot a(x1, y1, z1), b(x2, y2, z2), c(x3, y3, z3), d(x4, y4, z4);
    Cell cell1(a, b, c, d);
    std::cout << "Normal\n";
    cell1.n.show();
    std::cout << "basis:\n";
    cell1.t1.show();
    cell1.t2.show();
    std::cout << "square:\n";
    std::cout << cell1.s;
    std::cout << "\ncell integral from vector function:\n";
    Int_cell(a, b, c, d, r).show();
    std::cout << "\nsegment integral from vector function:\n";
    Int_segment(a, b, r).show();
    std::cout << "cell integral from scalar function:\n";
    std::cout << Int_cell(a, b, c, d, f) << "\n";
    std::cout << "segment integral from scalar function:\n";
    std::cout << Int_segment(a, b, f);
    return 0;
}