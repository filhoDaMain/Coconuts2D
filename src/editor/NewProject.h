/*
 * Copyright 2026 Andre Temprilho
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
#include <cmrc/cmrc.hpp>

namespace coconuts2D {

#define TEMPLATE_PREFIX "templates/NewProject"

class NewProject
{
    struct Entry
    {
        std::vector<unsigned char> bytes;
        std::string relPath;
        bool isDir;
    };
    

public:
    NewProject();
    ~NewProject();

    bool CreateNewProject(const std::string& destination);

private:
    std::vector<Entry> m_FileEntries;
    std::vector<Entry> m_DirEntries;
    std::string m_PREFIX;
};

}

#endif  // COCONUTS2D_NEWPROJECT_H
