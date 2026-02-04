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

#define PARSER_ROOT_NR_OF_SCENES_SCALAR "NrOfScenes"
#define PARSER_ROOT_LOAD_SCENE_SCALAR "LoadScene"
#define PARSER_ROOT_SCENES_SEQUENCE "Scenes"

#define PARSER_SCENE_ID_SCALAR "ID"
#define PARSER_SCENE_NAME_SCALAR "Name"
#define PARSER_SCENE_ENTITIES_SEQUENCE "Entities"

// These maybe make sense to be defined somewhere else, like in Components.h
#define PARSER_ENTITY_TAGCOMPONENT_SEQUENCE "TagComponent"
#define PARSER_ENTITY_TRANSFORMCOMPONENT_SEQUENCE "TransformComponent"
#define PARSER_ENTITY_SCRIPTCOMPONENT_SEQUENCE "ScriptComponent"

#define PARSER_TAGCOMPONENT_TAG_KEY_STRING "tag"

#define PARSER_TRANSFORMOMPONENT_X_KEY_UINT16 "x"
#define PARSER_TRANSFORMOMPONENT_Y_KEY_UINT16 "y"
#define PARSER_TRANSFORMOMPONENT_Z_KEY_UINT16 "z"
#define PARSER_TRANSFORMOMPONENT_ROT_KEY_UINT16 "rot"

#define PARSER_SCRIPTCOMPONENT_FILE_KEY_STRING "file"


class ResourceManager
{

    struct ScriptingAPI
    {
        std::string_view script;    // ref to a script in memory
        std::string apiName;
    };

public:
    // Note: descPath must be later substituted by the runtime contents of the game descriptor file.
    // This current approach only serves for easy prototyping which depends on reading the file from
    // the filsesystem
    ResourceManager(const std::string descPath, bool loadDefaultScene = false);
    ~ResourceManager();

    bool LoadScene(uint16_t id);
    bool LoadScriptingAPI(void);

private:
    std::string m_GameDescFile;
    std::stringstream m_DescBuffer;
    uint16_t m_NrOfScenes;
    uint16_t m_LoadScene;
    std::string m_ScriptingAPIPrefix;
    std::vector<ScriptingAPI> m_Scripts;
    bool m_IsScripingAPILoaded;
};

}

#endif  // COCONUTS2D_RESOURCEMANAGER_H