#include "integral.h"
/*
template <typename T>
T Int_segment(Dot A, Dot B, int N, T&& f) {
    Dot h[3];
    Vector BA(B[0] - A[0], B[1] - A[1], B[2] - A[2]);
    h = BA.module_vec() / 2;
    T res = 0;
    for (int i = 1; i <= N; ++i) {
        res += f(A + (B - A) * ((i - 0.5) / N)) * h;
    }
    return res;
}

template <typename T>
T Int_cell(Dot A, Dot B, Dot C, Dot D, T&& f) {
    Dot AB, BC, CD, DA, O;
    for (int i = 0; i < 3; ++i) {
        AB[i] = (A[i] + B[i]) / 2;
        BC[i] = (B[i] + C[i]) / 2;
        CD[i] = (C[i] + D[i]) / 2;
        DA[i] = (D[i] + A[i]) / 2;
    }
    for (int i = 0; i < 3; ++i) {
        O[i] = (DA[i] + BC[i] + CD[i] + AB[i]) / 2;
    }
    Cell c1(A, AB, O, DA), c2(AB, B, BC, O), c3(O, BC, C, CD), c4(DA, O, CD, D);
    return f(c1.x) * c1.s + f(c2.x) * c2.s + f(c3.x) * c3.s + f(c4.x) * c4.s;
}
*/