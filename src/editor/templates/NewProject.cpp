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

#include "NewProject.h"
#include <coconuts2D/Logger.h>
#include <fstream>
#include <filesystem>

#define INCBIN_SILENCE_BITCODE_WARNING
extern "C" {
#include <incbin/incbin.h>
}

// Embed files data at compile time
INCTXT(Template, COCONUTS2D_NEWPROJECT_TEMPLATE_FILE);
INCTXT(Build, COCONUTS2D_NEWPROJECT_TEMPLATE_BUILDSH);
INCTXT(CMake, COCONUTS2D_NEWPROJECT_TEMPLATE_CMAKELISTS);
INCTXT(Env, COCONUTS2D_NEWPROJECT_TEMPLATE_ENV);
INCTXT(Main, COCONUTS2D_NEWPROJECT_TEMPLATE_MAIN);

namespace coconuts2D {

#define TEMPLATE_DIR_TOKEN      "d: "
#define TEMPLATE_BUILD_TOKEN    "f_build: "
#define TEMPLATE_CMAKE_TOKEN    "f_cmake: "
#define TEMPLATE_ENV_TOKEN      "f_env: "
#define TEMPLATE_MAIN_TOKEN     "f_main: "

NewProject::NewProject()
:   m_TemplateFilePath(COCONUTS2D_NEWPROJECT_TEMPLATE_FILE),
    m_Template(gTemplateData),
    m_Mkdirs(),
    m_TouchFiles {
        EmbeddedTextFile(gBuildData, std::string(TEMPLATE_BUILD_TOKEN)),
        EmbeddedTextFile(gCMakeData, std::string(TEMPLATE_CMAKE_TOKEN)),
        EmbeddedTextFile(gEnvData, std::string(TEMPLATE_ENV_TOKEN)),
        EmbeddedTextFile(gMainData, std::string(TEMPLATE_MAIN_TOKEN))
    }
{
    ParseTemplateFile();
}

bool NewProject::CreateNewProject(const std::string& location)
{
    // Make directory at location
    //TODO sanitize
    std::filesystem::create_directory(location);

    // Make directories from template
    for (auto dir : m_Mkdirs)
    {
        std::string concat = location + "/" + dir;
        std::filesystem::create_directory(concat);
    }

    // Touch files
    for (auto embedfile : m_TouchFiles)
    {
        std::string concat = location + "/" + embedfile.path;
        std::ofstream data(concat);
        data << embedfile.data;
        data.close();

        // Set +x permission for ./build.sh
        if ( embedfile.token == std::string(TEMPLATE_BUILD_TOKEN) )
        {
            std::filesystem::permissions(
                concat,
                std::filesystem::perms::owner_exec,
                std::filesystem::perm_options::add
            );
        }
    }

    return true;
}

void NewProject::ParseTemplateFile(void)
{
    std::ifstream file(m_TemplateFilePath);
    std::string line;
    std::string tmpDirToken(TEMPLATE_DIR_TOKEN);

    // Parse template file looking for tokens.
    while (std::getline(file, line))
    {
        // It's a directory
        if (line.rfind(tmpDirToken, 0) == 0)
        {
            // Remove the token part
            m_Mkdirs.push_back(line.substr(tmpDirToken.size()));
        }
        // It's an EmbeddedTextFile
        else
        {
            //TODO Maybe a hash table could speed up...
            // For now I don't care as there's so few tokens to compare against.
            for (auto& embedfile : m_TouchFiles)
            {
                if (line.rfind(embedfile.token, 0) == 0)
                {
                    embedfile.path = line.substr(embedfile.token.size());
                    break;
                }
            }
        }
    }
}

}
