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

#include "io/basereader.hpp"

#include <algorithm>

namespace io
{
BaseReader::BaseReader(const std::string& file)
{
    stream_.open(file);
    if (!stream_.is_open())
    {
        std::runtime_error("Input file " + file + " was not opened");
    }
}

BaseReader::~BaseReader()
{
    if (stream_.is_open())
    {
        stream_.close();
    }
}

BaseReader::Content BaseReader::ReadRaw() const
{
    Content content;
    std::string line;
    while (std::getline(stream_, line))
    {
        if (line.empty())
        {
            continue;
        }

        // Trim the line
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.cend());
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.cend());

        content.push_back(line);
    }

    return content;
}
} // namespace io