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

#include "NewProject.h"
#include <coconuts2D/Logger.h>
#include <coconuts2D/Logger.h>
#include <fstream>
#include <filesystem>
#include <functional>


CMRC_DECLARE(newproject);


namespace coconuts2D {

NewProject::NewProject()
: m_FileEntries(), m_DirEntries(), m_PREFIX(TEMPLATE_PREFIX)
{
    /**
     * Populate all entries of "NewProject" template in m_FileEntries[] and m_DirEntries[].
     * Upon CreateNewProject() call, these entries are written into filesystem.
     */
    auto fs = cmrc::newproject::get_filesystem();

    if (fs.is_directory(m_PREFIX))
    {
        std::function<void(const std::string path)> iterate = [&](const std::string path)
        {
            for (auto&& entry : fs.iterate_directory(path))
            {
                std::string newPath = path + "/" + entry.filename();
                std::string newRelPath = newPath.substr(m_PREFIX.length()+1);

                if (entry.is_directory())
                {
                    Entry newEntry = {
                        .relPath = newRelPath,
                        .isDir = true
                    };
                    m_DirEntries.push_back(newEntry);

                    iterate(newPath);
                }
                else
                {
                    auto data = fs.open(newPath);

                    //TODO: avoid allocating new vectors and just point to data.beign()!
                    Entry newEntry = {
                        .bytes = std::vector<unsigned char>(data.begin(), data.end()),
                        .relPath = newRelPath,
                        .isDir = false
                    };
                    m_FileEntries.push_back(newEntry);
                }
            }
        };

        iterate(m_PREFIX);
    }
}

NewProject::~NewProject()
{
}

bool NewProject::CreateNewProject(const std::string& destination)
{
    std::filesystem::create_directory(destination);

    for (auto& entry : m_DirEntries)
    {
        if (!entry.isDir)
        {
            return false;
        }

        std::string dirPath = destination + "/" + entry.relPath;
        std::filesystem::create_directory(dirPath);
    }

    for (auto& entry : m_FileEntries)
    {
        if (entry.isDir)
        {
            return false;
        }

        std::string filePath = destination + "/" + entry.relPath;

        std::ofstream file(filePath, std::ios::binary);
        if (!file)
        {
            return false;
        }

        file.write(
            reinterpret_cast<const char*>(
                entry.bytes.data()
            ),
            entry.bytes.size()
        );
        if (!file)
        {
            return false;
        }

        // Set +x permission for build.sh
        if ( entry.relPath.find("build.sh") != std::string::npos )
        {
            std::filesystem::permissions(
                filePath,
                std::filesystem::perms::owner_exec,
                std::filesystem::perm_options::add
            );
        }
    }

    return true;
}

}
