#pragma once

#include <utility>
#include "Matrix.h"

namespace MatrixFind {
    namespace not_use_ {
        struct Position
        {
            int row;
            int col;
        };

        Position operator-(const Position& rhs)
        {
            return { -rhs.row, -rhs.col };
        }
        Position operator+(const Position& lhs, const Position& rhs)
        {
           return { lhs.row + rhs.row, lhs.col + rhs.col };
        }
        Position operator-(const Position& lhs, const Position& rhs)
        {
            return lhs + -rhs;
        }
        Position operator/(const Position& lhs, int k)
        {
            return { lhs.row / k, lhs.col / k };
        }

        template<typename T>
        static typename Matrix<T>::const_iterator rowFindImpl(Matrix<T>& m, const T& value, int noRow, int noCol0, int noCol1)
        {
            if (noCol0 == noCol1 && m[noRow][noCol0] == value)
                return m.getCit(noRow, noCol0);

            if (noCol0 == noCol1)
                return m.cend();

            const int median = noCol0 + (noCol1 - noCol0) / 2;
            if (value > m[noRow][median])
                return rowFindImpl(m, value, noRow, median + 1, noCol1);

            return rowFindImpl(m, value, noRow, noCol0, median);
        }

        template<typename T>
        static typename Matrix<T>::const_iterator colFindImpl(Matrix<T>& m, const T& value, int noCol, int noRow0, int noRow1)
        {
            if (noRow0 == noRow1 && m[noRow0][noCol] == value)
                return m.getCit(noRow0, noCol);

            if (noRow0 == noRow1)
                return m.cend();

            const int median = noRow0 + (noRow1 - noRow0) / 2;
            if (value > m[median][noCol])
                return colFindImpl(m, value, noCol, median + 1, noRow1);

            return colFindImpl(m, value, noCol, noRow0, median);
        }

        template<typename T>
        static typename Matrix<T>::const_iterator findImpl(Matrix<T>& m, const T& value, Position pos0, Position pos1)
        {
            if (pos0.row == pos1.row)
                return rowFindImpl(m, value, pos0.row, pos0.col, pos1.col);
            if (pos0.col == pos1.col)
                return colFindImpl(m, value, pos0.col, pos0.row, pos1.row);

            const Position middleDiag = pos0 + (pos1 - pos0) / 2;

            if (m[middleDiag.row][middleDiag.col] == value)
                return m.getCit(middleDiag.row, middleDiag.col);

            auto it = m.cend();
            if (value > m[middleDiag.row][middleDiag.col]) {
                it = findImpl(m, value, { middleDiag.row + 1, pos0.col }, pos1);
                if (it == m.cend()) {
                    it = findImpl(m, value, { pos0.row, middleDiag.col + 1 }, { middleDiag.row, pos1.col });
                }
            }
            else {
                if (pos0.row == middleDiag.row)
                    it = rowFindImpl(m, value, pos0.row, pos0.col, middleDiag.col - 1);
                else
                    it = findImpl(m, value, pos0, { middleDiag.row - 1, pos1.col });
                if (it == m.cend()) {
                    if (pos0.col == middleDiag.col)
                        it = colFindImpl(m, value, pos0.col, pos0.row, middleDiag.row - 1);
                    else
                        it = findImpl(m, value, { middleDiag.row, pos0.col }, { pos1.row, middleDiag.col - 1 });
                }
            }

            return it;
        }

        template<typename T>
        static typename Matrix<T>::const_iterator findSquareImpl(Matrix<T>& m, const T& value, int pos0, int pos1)
        {
            const int middle = pos0 + (pos1 - pos0) / 2;

            if (value == m[middle][middle])
                return m.getCit(middle, middle);

            if (value > m[middle][middle]) {
                if (middle == pos0) {
                    auto it = findImpl(m, value, { 0, middle + 1 }, {middle + 1, m.columnSize() - 1 });
                    if (it == m.cend())
                        it = findImpl(m, value, { middle + 1, 0 }, { m.rowSize() - 1, middle + 1 });
                    return it;
                }
                return findSquareImpl(m, value, middle, pos1);
            }

            if (middle == pos0) {
                auto it = findImpl(m, value, { 0, middle }, { middle - 1, m.columnSize() - 1 });
                if (it == m.cend())
                    it = findImpl(m, value, { middle, 0 }, { m.rowSize() - 1, middle - 1 });
                return it;
            }

            return findSquareImpl(m, value, pos0, middle);
        }
    }

    template<typename T>
    typename Matrix<T>::const_iterator find(Matrix<T>& m, const T& value)
    {
        if (m.rowSize() != m.columnSize())
            return not_use_::findImpl(m, value, { 0, 0 }, { m.rowSize() - 1, m.columnSize() - 1 });

        return not_use_::findSquareImpl(m, value, 0, m.rowSize() - 1);
    }
}