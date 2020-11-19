/*!
* \brief:  Declares the matrix class
*
* The MIT License (MIT)
*
* Copyright (c) 2020 Sascha Schiwy. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#pragma once
#include <vector>

namespace ChessNS
{
    /*!
     * \class   Matrix
     *
     * \brief   A matrix for all types. There is no math like Matrix multiplication given.
     *
     * \tparam  T   Generic type parameter.
     */
    template <class T> class Matrix
    {
    public:

        /*!
         * \typedef typename std::vector<T>::size_type size_type
         *
         * \brief   Defines an alias representing type of the size
         */
        typedef typename std::vector<T>::size_type size_type;

        /*!
         * \typedef typename std::vector<T>::iterator iterator
         *
         * \brief   Defines an alias representing the iterator
         */
        typedef typename std::vector<T>::iterator iterator;

        /*!
         * \typedef typename std::vector<T>::const_iterator const_iterator
         *
         * \brief   Defines an alias representing the constant iterator
         */
        typedef typename std::vector<T>::const_iterator const_iterator;

        /*!
         * \fn  void Matrix::assign(size_type rows, size_type columns, const T& initializerValue = T{})
         *
         * \brief   Assigns a new Matrix with an initializer value
         *
         * \param   rows                The rows.
         * \param   columns             The columns.
         * \param   initializerValue    (Optional) The initializer value.
         */
        void assign(size_type rows, size_type columns, const T& initializerValue = T{})
        {
            _rows = rows;
            _cols = columns;
            _mat.assign(_rows * _cols, initializerValue);
        }

        /*!
         * \fn  size_type Matrix::rows()
         *
         * \brief   Gets the number of rows
         *
         * \returns The number of rows.
         */
        size_type rows() { return _rows; }

        /*!
         * \fn  size_type Matrix::columns()
         *
         * \brief   Gets the number of columns
         *
         * \returns the number of columns;
         */
        size_type columns() { return _cols; }

        /*!
         * \fn  std::pair<size_type, size_type> Matrix::size()
         *
         * \brief   Gets the size of the matrix, <rows, columns>
         *
         * \returns <rows, columns>;
         */
        std::pair<size_type, size_type> size() { return {_rows, _cols}; }

        /*!
         * \fn  T& Matrix::at(const size_type row, const size_type column)
         *
         * \brief   Access the matrix at row, column
         *
         * \param   row     The row.
         * \param   column  The column.
         *
         * \returns A reference to the Element.
         */
        T& at(const size_type row, const size_type column) { return _mat.at(row * _cols + column); }

        /*!
         * \fn  iterator Matrix::begin()
         *
         * \brief   Gets the begin of the matrix in row based way 
         *          (i.e. [0,0]; [0,1]; ...; [1, 0]; [1,1]; ... [rows-1,columns-1])
         *
         * \returns An iterator.
         */
        iterator begin() { return _mat.begin(); }

        /*!
         * \fn  iterator Matrix::end()
         *
         * \brief   Gets the end of the matrix
         *
         * \returns An iterator.
         */
        iterator end() { return _mat.end(); }

        /*!
         * \fn  iterator Matrix::begin(size_type row)
         *
         * \brief   Begins at the given row
         *
         * \param   row The row.
         *
         * \returns An iterator.
         */
        iterator begin(size_type row) { return _mat.begin() + _cols * row; }

        /*!
         * \fn  iterator Matrix::end(size_type row)
         *
         * \brief   The end of a given row
         *
         * \param   row The row.
         *
         * \returns An iterator.
         */
        iterator end(size_type row) { return _mat.begin() + _cols * row + _cols; }

        /*!
         * \fn  std::vector<const T*> Matrix::row(size_type row)
         *
         * \brief   Gets all the elements of a given row
         *
         * \param   row The row.
         *
         * \returns all the elements of a given row;
         */
        std::vector<const T*> row(size_type row)
        {
            std::vector<const T*> result(_cols);
            for (size_type i = 0; i < _cols; i++)
                result[i]    = &_mat[_cols * row + i];

            return result;
        }

        /*!
         * \fn  std::vector<const T*> Matrix::column(size_type column)
         *
         * \brief   Gets all the elements of a given column
         *
         * \param   column  The column.
         *
         * \returns all the elements of a given column;
         */
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
