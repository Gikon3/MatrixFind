#pragma once

#include <vector>
#include <initializer_list>
#include <utility>

template<typename T = int>
class MatrixRow final
{
public:
    MatrixRow(std::vector<T>& data, int index);
    T& operator[](int noColumn);
    const T& operator[](int noColumn) const;

private:
    std::vector<T>& data_;
    int index_;
};


template<typename T = int>
class Matrix final
{
public:
    Matrix(int nColumns, int nRows);
    Matrix(std::initializer_list<std::initializer_list<T>> init);
    MatrixRow<T> operator[](int noRow);
    int columnSize() const;
    int rowSize() const;
    std::pair<int, int> find(const T& value);

private:
    struct Position
    {
        int row;
        int col;
    };

private:
    bool rowFindImpl(const T& value, Position& findPos, int noRow, int noCol0, int noCol1);
    bool colFindImpl(const T& value, Position& findPos, int moCol, int noRow0, int noRow1);
    bool findImpl(const T& value, Position& findPos, Position pos0, Position pos1);

private:
    std::vector<T> data_;
    int nColumns_;
    int nRows_;
};


template<typename T>
MatrixRow<T>::MatrixRow(std::vector<T>& data, int index) :
    data_(data), index_(index)
{
}

template<typename T>
T& MatrixRow<T>::operator[](int noColumn)
{
    return data_[index_ + noColumn];
}

template<typename T>
const T& MatrixRow<T>::operator[](int noColumn) const
{
    return data_[index_ + noColumn];
}


template<typename T>
Matrix<T>::Matrix(int nColumns, int nRows) :
    data_(nColumns * nRows), nColumns_(nColumns), nRows_(nRows)
{
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init) :
    data_(init.size() * (*init.begin()).size()), nRows_(init.size()), nColumns_((*init.begin()).size())
{
    auto it = data_.begin();
    for (auto& row : init)
        for (auto& val : row)
            *it++ = val;
}

template<typename T>
MatrixRow<T> Matrix<T>::operator[](int noRow)
{
    return MatrixRow<T>(data_, nColumns_ * noRow);
}

template<typename T>
int Matrix<T>::columnSize() const
{
    return nColumns_;
}

template<typename T>
int Matrix<T>::rowSize() const
{
    return nRows_;
}

template<typename T>
std::pair<int, int> Matrix<T>::find(const T& value)
{
    Position position = { -1, -1 };
    findImpl(value, position, { 0, 0 }, { nRows_ - 1, nColumns_ - 1 });
    return { position.row, position.col };
}

template<typename T>
bool Matrix<T>::rowFindImpl(const T& value, Position& findPos, int noRow, int noCol0, int noCol1)
{
    Matrix<T>& m = *this;

    if (noCol0 == noCol1 && m[noRow][noCol0] == value) {
        findPos = { noRow, noCol0 };
        return true;
    }

    if (noCol0 == noCol1)
        return false;

    const int median = noCol0 + (noCol1 - noCol0) / 2;
    if (value > m[noRow][median])
        return rowFindImpl(value, findPos, noRow, median + 1, noCol1);

    return rowFindImpl(value, findPos, noRow, noCol0, median);
}

template<typename T>
bool Matrix<T>::colFindImpl(const T& value, Position& findPos, int noCol, int noRow0, int noRow1)
{
    Matrix<T>& m = *this;

    if (noRow0 == noRow1 && m[noRow0][noCol] == value) {
        findPos = { noRow0, noCol };
        return true;
    }

    if (noRow0 == noRow1)
        return false;

    const int median = noRow0 + (noRow1 - noRow0) / 2;
    if (value > m[median][noCol])
        return colFindImpl(value, findPos, noCol, median + 1, noRow1);

    return colFindImpl(value, findPos, noCol, noRow0, median);
}

template<typename T>
bool Matrix<T>::findImpl(const T& value, Position& findPos, Position pos0, Position pos1)
{
    Matrix<T>& m = *this;

    if (pos0.row == pos1.row)
        return rowFindImpl(value, findPos, pos0.row, pos0.col, pos1.col);
    if (pos0.col == pos1.col)
        return colFindImpl(value, findPos, pos0.col, pos0.row, pos1.row);

    const Position middleDiag = { pos0.row + (pos1.row - pos0.row) / 2, pos0.col + (pos1.col - pos0.col) / 2 };

    if (m[middleDiag.row][middleDiag.col] == value) {
        findPos = middleDiag;
        return true;
    }
    
    bool status = false;
    if (value > m[middleDiag.row][middleDiag.col]) {
        status = findImpl(value, findPos, { middleDiag.row + 1, pos0.col }, pos1);
        if (!status)
            status = rowFindImpl(value, findPos, middleDiag.row, middleDiag.col + 1, pos1.col);
    }

    if (!status) {
        if (pos0.row == middleDiag.row)
            status = rowFindImpl(value, findPos, pos0.row, pos0.col, pos1.col);
        else
            status = findImpl(value, findPos, pos0, { middleDiag.row - 1, pos1.col });
    }
    if (!status) {
        if (!status && pos0.col == middleDiag.col)
            status = colFindImpl(value, findPos, pos0.col, middleDiag.row, pos1.row);
        else
            status = findImpl(value, findPos, { middleDiag.row, pos0.col }, { pos1.row, middleDiag.col - 1 });
    }

    return status;
}
