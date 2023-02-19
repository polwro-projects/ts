/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>

namespace math
{
template <class T> class Matrix
{
private:
    using Values = std::deque<std::deque<T>>;

public:
    Matrix() = default;

    Matrix(uint32_t columns, uint32_t rows)
    {
        resize(columns, rows);
    }

    Matrix(const Matrix<T>& rhs) : values_{ rhs.values_ }
    {
    }

public:
    Matrix& operator=(const Matrix<T>& rhs)
    {
        values_ = rhs.values_;
        return *this;
    }

    T& operator()(uint32_t row, uint32_t column)
    {
        return const_cast<T&>(const_cast<const Matrix*>(this)->operator()(column, row));
    }

    const T& operator()(uint32_t row, uint32_t column) const
    {
        if (column >= Columns() || row >= Rows())
        {
            throw std::runtime_error("Size of the matrix is smaller than the provided position");
        }

        const auto& matrix_row = values_.at(row);
        return matrix_row.at(column);
    }

    size_t Rows() const
    {
        return values_.size();
    }

    size_t Columns() const
    {
        return Rows() == 0 ? 0 : values_.at(0).size();
    }

    void resize(uint32_t columns, uint32_t rows)
    {
        // TODO : add checks

        // Construct rows of the matrix
        for (uint32_t row{}; row < rows; ++row)
        {
            values_.emplace_back(columns);
        }
    }

    void insert(const std::deque<T>& value)
    {
        // TODO : add a check

        values_.push_back(value);
    }

    const std::deque<T>& GetRow(uint32_t index) const
    {
        if (index >= Rows())
        {
            throw std::runtime_error("Size of the matrix is smaller than the provided index of the row");
        }

        return values_.at(index);
    }

private:
    Values values_;
};
} // namespace math

template <class T> std::ostream& operator<<(std::ostream& stream, const math::Matrix<T>& matrix)
{
    for (uint32_t row{}; row < matrix.Rows(); ++row)
    {
        for (uint32_t column{}; column < matrix.Columns(); ++column)
        {
            std::cout << matrix(column, row);
        }
        std::cout << std::endl;
    }

    return stream;
}