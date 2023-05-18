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
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

public:
    Matrix(int nColumns, int nRows);
    Matrix(std::initializer_list<std::initializer_list<T>> init);
    MatrixRow<T> operator[](int noRow);
    iterator begin();
    iterator end();
    const_iterator cbegin();
    const_iterator cend();
    int columnSize() const;
    int rowSize() const;
    const_iterator getCit(int noRow, int noCol) const;

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
typename Matrix<T>::iterator Matrix<T>::begin()
{
    return data_.begin();
}

template<typename T>
typename Matrix<T>::iterator Matrix<T>::end()
{
    return data_.end();
}

template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::cbegin()
{
    return data_.cbegin();
}

template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::cend()
{
    return data_.cend();
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
typename Matrix<T>::const_iterator Matrix<T>::getCit(int noRow, int noCol) const
{
    return data_.begin() + noRow * nColumns_ + noCol;
}
