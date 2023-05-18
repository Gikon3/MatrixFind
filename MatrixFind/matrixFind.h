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
        static bool rowFindImpl(Matrix<T>& m, const T& value, Position& findPos, int noRow, int noCol0, int noCol1)
        {
            if (noCol0 == noCol1 && m[noRow][noCol0] == value) {
                findPos = { noRow, noCol0 };
                return true;
            }

            if (noCol0 == noCol1)
                return false;

            const int median = noCol0 + (noCol1 - noCol0) / 2;
            if (value > m[noRow][median])
                return rowFindImpl(m, value, findPos, noRow, median + 1, noCol1);

            return rowFindImpl(m, value, findPos, noRow, noCol0, median);
        }

        template<typename T>
        static bool colFindImpl(Matrix<T>& m, const T& value, Position& findPos, int noCol, int noRow0, int noRow1)
        {
            if (noRow0 == noRow1 && m[noRow0][noCol] == value) {
                findPos = { noRow0, noCol };
                return true;
            }

            if (noRow0 == noRow1)
                return false;

            const int median = noRow0 + (noRow1 - noRow0) / 2;
            if (value > m[median][noCol])
                return colFindImpl(m, value, findPos, noCol, median + 1, noRow1);

            return colFindImpl(m, value, findPos, noCol, noRow0, median);
        }

        template<typename T>
        static bool findImpl(Matrix<T>& m, const T& value, Position& findPos, Position pos0, Position pos1)
        {
            if (pos0.row == pos1.row)
                return rowFindImpl(m, value, findPos, pos0.row, pos0.col, pos1.col);
            if (pos0.col == pos1.col)
                return colFindImpl(m, value, findPos, pos0.col, pos0.row, pos1.row);

            const Position middleDiag = pos0 + (pos1 - pos0) / 2;

            if (m[middleDiag.row][middleDiag.col] == value) {
                findPos = middleDiag;
                return true;
            }

            bool status = false;
            if (value > m[middleDiag.row][middleDiag.col]) {
                status = findImpl(m, value, findPos, { middleDiag.row + 1, pos0.col }, pos1);
                if (!status) {
                    status = findImpl(m, value, findPos, { pos0.row, middleDiag.col + 1 }, { middleDiag.row, pos1.col });
                }
            }
            else {
                if (pos0.row == middleDiag.row)
                    status = rowFindImpl(m, value, findPos, pos0.row, pos0.col, middleDiag.col - 1);
                else
                    status = findImpl(m, value, findPos, pos0, { middleDiag.row - 1, pos1.col });
                if (!status) {
                    if (pos0.col == middleDiag.col)
                        status = colFindImpl(m, value, findPos, pos0.col, pos0.row, middleDiag.row - 1);
                    else
                        status = findImpl(m, value, findPos, { middleDiag.row, pos0.col }, { pos1.row, middleDiag.col - 1 });
                }
            }

            return status;
        }

        template<typename T>
        static bool findSquareImpl(Matrix<T>& m, const T& value, Position& findPos, int pos0, int pos1)
        {
            const int middle = pos0 + (pos1 - pos0) / 2;

            if (value == m[middle][middle]) {
                findPos = { middle, middle };
                return true;
            }

            if (value > m[middle][middle]) {
                if (middle == pos0) {
                    bool status = findImpl(m, value, findPos, { 0, middle + 1 }, {middle + 1, m.columnSize() - 1 });
                    if (!status)
                        status = findImpl(m, value, findPos, { middle + 1, 0 }, { m.rowSize() - 1, middle + 1 });
                    return status;
                }
                return findSquareImpl(m, value, findPos, middle, pos1);
            }

            if (middle == pos0) {
                bool status = findImpl(m, value, findPos, { 0, middle }, { middle - 1, m.columnSize() - 1 });
                if (!status)
                    status = findImpl(m, value, findPos, { middle, 0 }, { m.rowSize() - 1, middle - 1 });
                return status;
            }

            return findSquareImpl(m, value, findPos, pos0, middle);
        }
    }

    template<typename T>
    std::pair<int, int> find(Matrix<T>& m, const T& value)
    {
        not_use_::Position position = { -1, -1 };
        if (m.rowSize() != m.columnSize())
            not_use_::findImpl(m, value, position, { 0, 0 }, { m.rowSize() - 1, m.columnSize() - 1 });
        else
            not_use_::findSquareImpl(m, value, position, 0, m.rowSize() - 1);
        return { position.row, position.col };
    }
}