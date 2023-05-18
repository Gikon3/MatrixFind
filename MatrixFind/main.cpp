#include <iostream>
#include <utility>
#include <chrono>
#include <random>
#include <set>
#include "Matrix.h"
#include "matrixFind.h"

#define SEED        0
#define N_TESTS     100
#define PRINT_ALL   0

class Pool final
{
public:
    int operator()(int val)
    {
        if (data_.count(val) == 0) {
            data_.insert(val);
            return val;
        }

        do {
            ++val;
        } while (data_.count(val) != 0);
        data_.insert(val);
        return val;
    }

private:
    std::set<int> data_;
};

void print(Matrix<int>& m)
{
    for (int j = 0; j < m.rowSize(); ++j) {
        for (int i = 0; i < m.columnSize(); ++i)
            std::cout << m[j][i] << " ";
        std::cout << std::endl;
    }
}

Matrix<int> genMatrix(std::mt19937& engine)
{
    const int nColumns = 1 + engine() % 9;
    const int nRows = 1 + engine() % 9;
    Matrix<int> m(nColumns, nRows);
    Pool pool;

    m[0][0] = engine() % 30;
    pool(m[0][0]);

    for (int i = 1; i < nColumns; ++i)
        m[0][i] = pool(m[0][i - 1] + 1 + engine() % 29);
    for (int j = 1; j < nRows; ++j) 
        m[j][0] = pool(m[j - 1][0] + 1 + engine() % 29);

    for (int j = 1; j < nRows; ++j) {
        for (int i = 1; i < nColumns; ++i) {
            const int max = std::max(m[j - 1][i], m[j][i - 1]);
            m[j][i] = pool(max + 1 + engine() % 29);
        }
    }

    return m;
}

std::pair<int, int> test(Matrix<int>& m)
{
    std::pair info = { 0, 0 };
    for (int j = 0; j < m.rowSize(); ++j) {
        for (int i = 0; i < m.columnSize(); ++i) {
            info.first += MatrixFind::find(m, m[j][i]) == std::pair(j, i) ? 1 : 0;
            ++info.second;
        }
    }
   
    return info;
}

int main()
{
#if SEED == 0
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
#else
    auto seed = SEED;
#endif
    std::mt19937 engine(seed);

    std::pair info = { 0, 0 };
    for (int i = 0; i < N_TESTS; ++i) {
        Matrix m = genMatrix(engine);
        const std::pair temp = test(m);
#if PRINT_ALL == 1
        print(m);
        std::cout << temp.first << " / " << temp.second << std::endl << std::endl;
#endif
        info.first += temp.first;
        info.second += temp.second;
    }

    std::cout << std::endl <<
        (info.first == info.second ? "OK" : "FAIL") << std::endl <<
        "Seed: " << seed << std::endl <<
        "Conclusions: " << info.first << " / " << info.second << std::endl;
}
