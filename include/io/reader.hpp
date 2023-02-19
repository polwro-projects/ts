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

#include <list>
#include <map>
#include <regex>
#include <string>

#include "io/basereader.hpp"
#include "math/matrix.hpp"
#include "utils/tokenizer.hpp"

namespace io
{
enum class FileTypes
{
    kTxt,
    kIni,
    kAtsp
};

template <FileTypes type> class Reader
{
};

template <> class Reader<FileTypes::kTxt> : public BaseReader
{
public:
    Reader(const std::string& file) : BaseReader{ file }
    {
    }
};

template <> class Reader<FileTypes::kIni> : public BaseReader
{
public:
    struct Parameters
    {
        struct Section
        {
            using Property = std::pair<std::string, std::string>;

            std::string name;
            std::map<std::string, std::string> properties;

            void clear()
            {
                name.clear();
                properties.clear();
            }
        };

        std::list<Section> sections;
    };

public:
    Reader(const std::string& file) : BaseReader{ file }
    {
    }

public:
    Parameters Read() const
    {
        Parameters parameters;
        Parameters::Section section;
        Content content{ ReadRaw() };

        for (auto& line : content)
        {
            if (IsSectionHeader(line))
            {
                if (!section.name.empty())
                {
                    parameters.sections.push_back(section);
                }

                // Update the section
                section.clear();
                section.name = ProcessSectionHeader(line);

                continue;
            }

            auto property = ProcessProperty(line);
            section.properties.insert(property);
        }

        // Save the latest section
        parameters.sections.push_back(section);

        return parameters;
    }

protected:
    std::pair<std::string, std::string> ProcessProperty(const std::string& value) const
    {
        auto parsed_line = utils::Tokenizer::tokenize(value, '=');
        if (parsed_line.size() != 2)
        {
            throw std::runtime_error("Property is not defined properly.");
        }

        return { parsed_line.at(0), parsed_line.at(1) };
    }

    bool IsSectionHeader(const std::string& value) const
    {
        return std::regex_search(value, std::regex{ "^\[[a-zA-Z0-9]*\\]" });
    }

    std::string ProcessSectionHeader(const std::string& value) const
    {
        return value.substr(1, value.size() - 2);
    }
};

template <> class Reader<FileTypes::kAtsp> : public BaseReader
{
public:
    struct Parameters
    {
        // TODO

        math::Matrix<uint32_t> positions;
    };

public:
    Reader(const std::string& file) : BaseReader{ file }
    {
    }

public:
    Parameters Read() const
    {
        Parameters parameters;
        Content content{ ReadRaw() };
        uint32_t dimension;

        for (const auto& line : content)
        {
            if (IsDimensionParameter(line))
            {
                dimension = GetDimensionParameter(line);
            }
        }

        parameters.positions = GetPositionsParameter(content, dimension);
        if (parameters.positions.Columns() != dimension)
        {
            std::cout << parameters.positions.Columns() << " " << dimension << std::endl;
            throw std::runtime_error("Matrix from the given file is broken");
        }

        return parameters;
    }

protected:
    inline bool IsDimensionParameter(const std::string& value) const
    {
        return value.rfind("DIMENSION: ", 0) == 0;
    }

    inline bool IsEndOfFile(const std::string& value) const
    {
        return value.rfind("EOF", 0) == 0;
    }

    uint32_t GetDimensionParameter(const std::string& value) const
    {
        if (!IsDimensionParameter(value))
        {
            throw std::runtime_error("Passed value is not a dimension parameter");
        }

        const auto data = utils::Tokenizer::tokenize(value, ' ');
        if (data.size() != 2)
        {
            throw std::runtime_error("Passed value is not a valid dimension parameter");
        }

        return std::stoi(data.at(1));
    }

    math::Matrix<uint32_t> GetPositionsParameter(Content& content, uint32_t dimensions) const
    {
        math::Matrix<uint32_t> positions;

        if (content.empty())
        {
            throw std::runtime_error("Content is empty. Was the file opened?");
        }

        auto iterator = std::find(content.begin(), content.end(), "EDGE_WEIGHT_SECTION");
        if (iterator == content.end())
        {
            throw std::runtime_error("Positions matrix was not found");
        }

        ++iterator;
        std::deque<uint32_t> weights;
        std::deque<uint32_t> buffer;
        for (; iterator != content.end(); ++iterator)
        {
            const auto line = *iterator;
            if (IsEndOfFile(line))
            {
                break;
            }

            // Convert the parsed string to a row of matrix
            const auto data = utils::Tokenizer::tokenize(line, ' ');
            std::for_each(data.cbegin(), data.cend(), [&buffer, &weights, dimensions](const std::string& value) {
                if (weights.size() < dimensions)
                {
                    weights.push_back(std::stoi(value));
                }
                else
                {
                    buffer.push_back(std::stoi(value));
                }
            });

            if (weights.size() == dimensions)
            {
                positions.insert(weights);
                weights = buffer;
                weights.clear();
            }
        }

        return positions;
    }
};
} // namespace io