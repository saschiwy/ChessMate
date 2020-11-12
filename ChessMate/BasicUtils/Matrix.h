#pragma once
#include <vector>

namespace ChessNS
{
    template <class T>
    class Matrix
    {
    public:
        typedef typename std::vector<T>::size_type      size_type;
        typedef typename std::vector<T>::iterator       iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;

        void assign(size_type rows, size_type columns, const T& initializerValue = T{})
        {
            _rows = rows;
            _cols = columns;
            _mat.assign(_rows * _cols, initializerValue);
        }

        size_type rows() { return _rows; }

        size_type columns() { return _cols; }

        std::pair<size_type, size_type> size() { return {_rows, _cols}; }

        T& at(const size_type row, const size_type column) { return _mat.at(row * _cols + column); }

        iterator begin() { return _mat.begin(); }

        iterator end() { return _mat.end(); }

        iterator begin(size_type row) { return _mat.begin() + _cols * row; }

        iterator end(size_type row) { return _mat.begin() + _cols * row + _cols; }

        std::vector<const T*> row(size_type row)
        {
            std::vector<const T*> result(_cols);
            for (size_type i = 0; i < _cols; i++)
                result[i]    = &_mat[_cols * row + i];

            return result;
        }

        std::vector<const T*> column(size_type column)
        {
            std::vector<const T*> result(_rows);
            for (size_type i = 0; i < _rows; i++)
                result[i]    = &_mat[column * _rows + i];

            return result;
        }

    private:
        std::vector<T> _mat;
        size_type      _rows{};
        size_type      _cols{};
    };
}
