/*
 * Copyright 2025 Andre Temprilho
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COCONUTS2D_NEWPROJECT_H
#define COCONUTS2D_NEWPROJECT_H

#include <string>
#include <vector>
#include <array>

namespace coconuts2D {

struct EmbeddedTextFile
{
    std::string data;
    std::string path;
    std::string token;
    EmbeddedTextFile(const char* embed, const std::string& token_val)
    : data(embed), token(token_val) {}
};

class NewProject
{
public:
    NewProject();
    ~NewProject() = default;

    bool CreateNewProject(const std::string& location);

private:
    void ParseTemplateFile(void);

private:
    std::string m_TemplateFilePath;
    std::string m_Template;
    std::vector< std::string > m_Mkdirs;
    std::array<EmbeddedTextFile, 4> m_TouchFiles;
};

}

#endif  // COCONUTS2D_NEWPROJECT_H
