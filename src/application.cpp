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

#include "application.hpp"

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include "utils/os/memory.hpp"
#endif

#include <chrono>

#include "tsp/algorithm/ts.hpp"

Application::Application(const std::string& config_file)
{
    // Read the parameters
    io::Reader<io::FileTypes::kIni> reader(config_file);
    parameters_ = reader.Read();

    // Open the output file
    auto iterator = std::find_if(parameters_.sections.cbegin(), parameters_.sections.cend(),
                                 [](const auto& section) { return section.name == "output"; });
    if (iterator == parameters_.sections.cend())
    {
        throw std::runtime_error("Output file was not specified");
    }
    output_file_.open(iterator->properties.at("filename"));
}

Application::~Application()
{
    output_file_.close();
}

void Application::Start()
{
    math::Matrix<uint32_t> positions;

    for (const auto& section : parameters_.sections)
    {
        // Skip the output section as it was checked already
        if (section.name == "output")
        {
            continue;
        }

        // Save the name of the section to the output
        output_file_ << section.name << std::endl;

        io::Reader<io::FileTypes::kAtsp> reader(section.properties.at("filename"));
        positions = reader.Read().positions;

        for (uint32_t index{ 1 }; index <= std::stoi(section.properties.at("count")); ++index)
        {
            tsp::algorithm::TS tsp{ positions, std::stoul(section.properties.at("max_tabu")),
                                    std::stoi(section.properties.at("max_iterations")),
                                    std::chrono::milliseconds(std::stoi(section.properties.at("time_limit"))) };

            const auto start_point = std::chrono::system_clock::now();
            const auto solution = tsp.Solve();
            const auto end_point = std::chrono::system_clock::now();

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
            // Write out the results of the calculation
            output_file_ << std::chrono::duration_cast<std::chrono::microseconds>(end_point - start_point).count()
                         << "," << utils::os::getProcessVirtualMemorySize() << ", ";
#else
            // Store the duration of the operation
            output_file_ << std::chrono::duration_cast<std::chrono::microseconds>(end_point - start_point).count()
                         << ", ";
#endif

            // Write the result of the calculation into the file
            for (const auto point : solution.path)
            {
                output_file_ << point << " -> ";
            }

            output_file_ << solution.path.at(0) << ", " << solution.weight << std::endl;
        }

        // Visually separate the sections
        output_file_ << std::endl;
    }
}