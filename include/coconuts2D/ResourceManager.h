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

#ifndef COCONUTS2D_RESOURCEMANAGER_H
#define COCONUTS2D_RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <sstream>

namespace coconuts2D {

#define SCRIPTING_API_PREFIX "scripting"

class ResourceManager
{
    struct ScriptingAPI
    {
        // First PoC just to prove it works
        //TODO we don't want to allocate memory but just point to original data structure!
        std::vector<unsigned char> bytes;
        std::string relPath;
    };

public:
    //TODO to be deleted
    ResourceManager();

    // Note: descPath must be later substituted by the runtime contents of the game descriptor file.
    // This current approach only serves for easy prototyping which depends on reading the file from
    // the filsesystem
    ResourceManager(const std::string descPath, bool loadDefaultScene = false);
    ~ResourceManager();

    bool LoadScene(uint16_t id);


    void LoadVirtualFS();

    const std::vector<ScriptingAPI>& GetScriptingAPI() { return m_Scripts; }

private:
    std::string m_GameDescFile;
    std::stringstream m_DescBuffer;
    uint16_t m_NrOfScenes;
    uint16_t m_LoadScene;

    std::vector<ScriptingAPI> m_Scripts;    //TODO delete
    std::string m_SCRIPTING_API_PREFIX;     //TODO delete
    bool m_IsVFSLoaded;                     //TODO delete
};

}

#endif  // COCONUTS2D_RESOURCEMANAGER_H