#include <iostream>
#include <utility>
#include "Matrix.h"

void print(Matrix<int>& m)
{
    for (int j = 0; j < m.rowSize(); ++j) {
        for (int i = 0; i < m.columnSize(); ++i)
            std::cout << m[j][i] << " ";
        std::cout << std::endl;
    }
}

std::pair<int, int> test(Matrix<int>& m)
{
    std::pair info = { 0, 0 };
    print(m);
    for (int j = 0; j < m.rowSize(); ++j) {
        for (int i = 0; i < m.columnSize(); ++i) {
            info.first += m.find(m[j][i]) == std::pair(j, i) ? 1 : 0;
            ++info.second;
        }
    }
    std::cout << info.first << " / " << info.second << std::endl << std::endl;
    return info;
}

int main()
{
    std::pair info = { 0, 0 };
    std::pair temp = { 0, 0 };

    Matrix<int> m0 = { {0} };
    temp = test(m0);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m1 = { {0, 1} };
    temp = test(m1);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m2 = { {0}, {1} };
    temp = test(m2);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m3 = { {0, 1, 2} };
    temp = test(m3);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m4 = { {0}, {1}, {2} };
    temp = test(m4);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m5 = { {0, 3}, {1, 4}, {2, 5} };
    temp = test(m5);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m6 = { {0, 1, 2}, {3, 4, 5} };
    temp = test(m6);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m7 = { {0, 1, 2, 3, 4, 5} };
    temp = test(m7);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m8 = { {0}, {1}, {2}, {3}, {4}, {5} };
    temp = test(m8);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m9 = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };
    temp = test(m9);
    info.first += temp.first;
    info.second += temp.second;

    Matrix<int> m10 = { 
        {10, 20, 30, 40, 60}, 
        {11, 71, 72, 73, 77},
        {12, 80, 81, 82, 91},
        {13, 85, 90, 95, 100},
        {96, 97, 98, 99, 101} };
    temp = test(m10);
    info.first += temp.first;
    info.second += temp.second;
}
