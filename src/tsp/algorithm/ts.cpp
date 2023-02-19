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

#include "tsp/algorithm/ts.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace tsp::algorithm
{
TS::TS(math::Matrix<uint32_t> distances, size_t max_tabu, uint32_t max_iterations, std::chrono::milliseconds time_limit)
    : Algorithm{ distances }, kMaxTabuSize{ max_tabu }, kIterationsPerEpoch{ max_iterations }, kTimeLimit{ time_limit }
{
}

Algorithm::Solution TS::Solve()
{
    solution_.path = CalculateStartingPath();
    solution_.weight = CalculateWeight(solution_);

    const auto start_timestamp = std::chrono::high_resolution_clock::now();
    uint32_t iteration{};
    Solution current_solution = solution_;
    while ((std::chrono::high_resolution_clock::now() - start_timestamp) < kTimeLimit)
    {
        if (iteration > kIterationsPerEpoch)
        {
            current_solution.path = CalculateRandomPath();
            current_solution.weight = CalculateWeight(current_solution);

            iteration = 0;
        }

        iteration++;
        current_solution = CalculateNeighbour(current_solution);

        if (current_solution < solution_)
        {
            solution_ = current_solution;

            // Clear the iteration counter
            iteration = 0;
        }
    }

    return solution_;
}

Algorithm::Path TS::CalculateStartingPath()
{
    Path firstpath;
    std::vector<bool> visited;
    visited.resize(distances_.Columns());

    int minchoice, minnode, lastnode;

    firstpath.push_back(0);
    visited.at(0) = true;

    for (int i = 1; i < distances_.Columns(); i++)
    {
        minchoice = std::numeric_limits<int>::max();
        minnode = 0;
        lastnode = firstpath.back();

        for (int j = 0; j < distances_.Columns(); j++)
        {
            if (!visited.at(j) && distances_(lastnode, j) < minchoice)
            {
                minchoice = distances_(lastnode, j);
                minnode = j;
            }
        }
        firstpath.push_back(minnode);
        visited.at(minnode) = true;
    }
    return firstpath;
}

Algorithm::Path TS::CalculateRandomPath()
{
    Path randpath;
    std::vector<bool> visited;
    visited.resize(distances_.Columns());
    int x = 0;
    randpath.push_back(0);

    for (int i = 0; i < distances_.Columns() - 1; i++)
    {
        x = rand() % (distances_.Columns() - 1) + 1;
        if (visited[x] != true)
        {
            randpath.push_back(x);
            visited[x] = true;
        }
        else
        {
            i--;
        }
    }
    return randpath;
}

std::vector<TS::Tabu> TS::CalculateTabus(Solution solution) const
{
    std::vector<Tabu> result;
    result.push_back({ 0, 0 });

    for (int i = 1; i < distances_.Columns() - 1; i++)
    {
        for (int j = i; j < distances_.Columns() - 1; j++)
        {
            result.push_back({ solution.path.at(i), solution.path.at(j) });
        }
    }

    return result;
}

TS::Solution TS::CalculateNeighbour(Solution solution)
{
    Solution result = { solution.path, std::numeric_limits<uint32_t>::max() };
    Tabu new_tabu;

    for (const auto& tabu : CalculateTabus(solution))
    {
        Swap(solution, tabu.first, tabu.last);

        if (CalculateWeight(solution) < result.weight)
        {
            auto iterator = std::find(tabus_.begin(), tabus_.end(), tabu);
            if (iterator != tabus_.end())
            {
                tabus_.erase(iterator);
            }

            result = { solution.path, CalculateWeight(solution) };
            new_tabu = { tabu.last, tabu.first };
        }

        Swap(solution, tabu.last, tabu.first);
    }

    AddTabu(new_tabu);

    return result;
}

void TS::AddTabu(const Tabu& value)
{
    tabus_.push_back(value);
    if (tabus_.size() > kMaxTabuSize)
    {
        tabus_.erase(tabus_.begin());
    }
}

uint32_t TS::CalculateWeight(const Solution& solution)
{
    uint32_t result{};
    for (int i = 0; i < distances_.Columns() - 1; i++)
    {
        result += distances_(solution.path.at(i), solution.path.at(i + 1));
    }
    result += distances_(solution.path.at(distances_.Columns() - 1), solution.path.at(0));
    return result;
}

void TS::Swap(Solution& solution, uint32_t i, uint32_t j)
{
    int x = std::distance(solution.path.begin(), std::find(solution.path.begin(), solution.path.end(), i));
    int y = std::distance(solution.path.begin(), std::find(solution.path.begin(), solution.path.end(), j));

    solution.path.at(x) = j;
    solution.path.at(y) = i;
}

bool TS::Tabu::operator==(const Tabu& another) const
{
    return first == another.first && last == another.last;
}
} // namespace tsp::algorithm