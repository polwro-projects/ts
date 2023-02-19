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

#include <chrono>
#include <vector>

#include "math/matrix.hpp"
#include "tsp/algorithm/algorithm.hpp"

namespace tsp::algorithm
{
class TS : public Algorithm
{
    struct Tabu
    {
        size_t first, last;

        /**
         * @brief Overriding the comparing operator for the tabu entry
         *
         * @return true if the objects are equal
         * @return false otherwise
         */
        bool operator==(const Tabu&) const;
    };

public:
    /**
     * @brief Construct a new TS object
     *
     * @param distances the matrix of distances between cities
     * @param max_tabu the maximum size of the tabu list
     * @param max_iterations the maximum size of the iterations in each epoch
     * @param time_limit the limit of time
     */
    TS(math::Matrix<uint32_t> distances, size_t max_tabu, uint32_t max_iterations,
       std::chrono::milliseconds time_limit);

public:
    /**
     * @brief Solve the given problem
     *
     * @return Solution the solution of the given problem
     */
    Solution Solve() override;

protected:
    /**
     * @brief Calculate weight of the given solution
     *
     * @param solution the imput solution
     * @return uint32_t the weight of the solution
     */
    uint32_t CalculateWeight(const Solution& solution);

    /**
     * @brief Calculate new solution in the neighbourhood of the given one
     *
     * @param solution the solution from which a new one should be derived
     * @return Solution a new solution
     */
    Solution CalculateNeighbour(Solution solution);

    /**
     * @brief Add the tabu to the list
     *
     * @param value the tabu to add
     */
    void AddTabu(const Tabu& value);

    /**
     * @brief Calculate the starting path
     *
     * @return Path a new starting pat
     */
    Path CalculateStartingPath();

    /**
     * @brief Calculate a random path
     *
     * @return Path a random path
     */
    Path CalculateRandomPath();

    /**
     * @brief Create a list of tabus for the given solution
     *
     * @param solution the solution for which tabus should be calculated
     * @return std::vector<Tabu> the list of tabus
     */
    std::vector<Tabu> CalculateTabus(Solution solution) const;

    /**
     * @brief Swap two positions in the solution
     *
     * @param solution the solution to update
     * @param i the first position
     * @param j the second position
     */
    void Swap(Solution& solution, uint32_t i, uint32_t j);

private:
    Solution solution_;

    const uint32_t kIterationsPerEpoch;
    const std::chrono::milliseconds kTimeLimit;

    const size_t kMaxTabuSize;
    std::vector<Tabu> tabus_;
};
} // namespace tsp::algorithm