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

#include "utils/tokenizer.hpp"

#include <iostream>
#include <regex>

namespace utils
{
Tokenizer::Tokens Tokenizer::tokenize(std::string line, char delimiter)
{
    Tokens tokens;
    size_t position;

    std::regex space_re(R"(\s+)");
    line = std::regex_replace(line, space_re, " ");

    while ((position = line.find(delimiter)) != std::string::npos)
    {
        auto token = line.substr(0, position);
        if (!token.empty())
            tokens.push_back(token);

        line.erase(0, position + 1);
    }
    tokens.push_back(line);

    return tokens;
}
} // namespace utils