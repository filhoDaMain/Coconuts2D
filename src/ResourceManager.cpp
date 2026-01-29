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

#include <coconuts2D/ResourceManager.h>
#include <coconuts2D/SceneManager.h>
#include <coconuts2D/ecs/Entity.h>
#include <coconuts2D/ecs/Components.h>
#include <coconuts2D/Logger.h>
#include <cmrc/cmrc.hpp>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <stdexcept>

//#define DEBUG_CODE

CMRC_DECLARE(resources);
CMRC_DECLARE(scripting);

namespace coconuts2D {

//TODO delete
ResourceManager::ResourceManager()
: m_GameDescFile(), m_DescBuffer(), m_NrOfScenes(), m_LoadScene(),
m_Scripts(), m_SCRIPTING_API_PREFIX(SCRIPTING_API_PREFIX), m_IsVFSLoaded(false)
{
}

ResourceManager::ResourceManager(const std::string descPath, bool loadDefaultScene)
: m_GameDescFile(descPath), m_DescBuffer(), m_NrOfScenes(), m_LoadScene(),
m_Scripts(), m_SCRIPTING_API_PREFIX(SCRIPTING_API_PREFIX), m_IsVFSLoaded(false)
{
    std::ifstream istr(m_GameDescFile);
    m_DescBuffer << istr.rdbuf();

    if (m_DescBuffer.str().size() == 0)
    {
        throw std::invalid_argument("GameDescFile is empty");
    }
    
    YAML::Node desc = YAML::Load(m_DescBuffer.str());

    //TODO substitue the hardcoded strings by constants
    const auto& nrOfScenes = desc["NrOfScenes"];
    if ( nrOfScenes && nrOfScenes.IsScalar() )
    {
        m_NrOfScenes = nrOfScenes.as<uint16_t>();
    }

    const auto& loadScene = desc["LoadScene"];
    if ( loadScene && loadScene.IsScalar() )
    {
        m_LoadScene = loadScene.as<uint16_t>();
    }

    if (loadDefaultScene)
    {
        LoadScene(m_LoadScene);
    }
}

ResourceManager::~ResourceManager()
{

}


/**
 * Looks for a requested scene in game resources description file.
 * If Scene is found, creates the scene.
 */
bool ResourceManager::LoadScene(uint16_t id)
{    
    YAML::Node desc = YAML::Load(m_DescBuffer.str());

    //TODO substitue the hardcoded strings by constants
    const auto& scenesList = desc["Scenes"];

    if ( scenesList && scenesList.IsSequence() )
    {
        const auto& findScene = scenesList[ (id-1) ];
        if ( findScene && scenesList.IsSequence() )
        {
            const auto& sceneName = findScene["Name"];
            if (!sceneName)
            {
                return false;
            }
            LOG_INFO("Loading Scene: {}: {}", id, sceneName.as<std::string>());

            // Look for Entities:
            const auto& entitiesList = findScene["Entities"];
            if ( entitiesList && entitiesList.IsSequence() )
            {
                for (const auto& entity : entitiesList)
                {
                    LOG_DEBUG("New Entity");

                    // TagComponent
                    const auto& tagComponent = entity["TagComponent"];
                    if ( tagComponent && tagComponent.IsSequence() )
                    {
                        LOG_TRACE("TagComponent");
                        for (const auto& pair : tagComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();
                            const auto& value = pair.begin()->second.as<std::string>();
                            LOG_TRACE("   {}: {}", key, value);
                        }
                    }

                    // TransformComponent
                    const auto& transformComponent = entity["TransformComponent"];
                    if ( transformComponent && transformComponent.IsSequence() )
                    {
                        LOG_TRACE("TransformComponent");
                        for (const auto& pair : transformComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();
                            const auto& value = pair.begin()->second.as<std::string>();
                            LOG_TRACE("   {}: {}", key, value);

                            // if (key == <expected> ) do something
                        }
                    }

                    // ScriptComponent
                    const auto& scriptComponent = entity["ScriptComponent"];
                    if ( scriptComponent && scriptComponent.IsSequence() )
                    {
                        LOG_TRACE("ScriptComponent");
                        for (const auto& pair : scriptComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();
                            const auto& value = pair.begin()->second.as<std::string>();
                            LOG_TRACE("   {}: {}", key, value);

                            // if (key == <expected> ) do something
                        }
                    }
                }
            }
        }
    }

    return true;
}

void ResourceManager::LoadVirtualFS()
{
#ifdef DEBUG_CODE
    if (m_IsVFSLoaded) return;

    auto fs_resources = cmrc::resources::get_filesystem();
    auto fs_scripting = cmrc::scripting::get_filesystem();

    try
    {

        std::function<void(const std::string path)> iterate = [&](const std::string path)
        {
            for (auto&& entry : fs_scripting.iterate_directory(path))
            {
                std::string newPath = path + "/" + entry.filename();
                std::string newRelPath = newPath.substr(m_SCRIPTING_API_PREFIX.length()+1);

                // Use dot notation as a directory separator, so we can include these scripts
                // as Lua modules like ' require("coconuts2D.entity_api") '
                std::replace(newRelPath.begin(), newRelPath.end(), '/', '.');

                if (entry.is_directory())
                {
                    iterate(newPath);
                }
                else
                {
                    auto data = fs_scripting.open(newPath);

                    //TODO: avoid allocating new vectors and just point to data.beign()!
                    ScriptingAPI newScript = {
                        .bytes = std::vector<unsigned char>(data.begin(), data.end()),
                        .relPath = newRelPath,
                    };
                    m_Scripts.push_back(newScript);
                }
            }
        };

        LOG_INFO("Loading Scripting API...");
        iterate(m_SCRIPTING_API_PREFIX);

        // Debug
        LOG_TRACE("Current API Scripts:");
        for (auto& script : m_Scripts)
        {
            LOG_TRACE(" {}", script.relPath);
        }

        auto gameDesc = fs_resources.open("res/desc.txt");
        auto desc = std::string( gameDesc.begin(), gameDesc.end() );
        LOG_INFO("desc.txt: {}", desc);

        // Example of a script
        auto example1Script = fs_resources.open("res/scripts/example1.lua");
        auto script = std::string( example1Script.begin(), example1Script.end() );

        LOG_INFO("Creating scene things...");

        // Create dummy / example game scene
        // (this would be read from a descriptor file)
        auto& sm = SceneManager::GetInstance();
        auto sceneID = sm.NewScene("Game Scene");

        auto scene = sm.GetScene(sceneID);
        auto entity1 = scene->NewEntity();
        entity1.AddComponent<Components::TagComponent>("Ze Carlos");
        entity1.AddComponent<Components::TransformComponent>();
        entity1.AddComponent<Components::ScriptComponent>(
            scene->GetLua(),
            script.c_str()
        );
    }
    catch (const std::system_error e)
    {
        LOG_WARN("No res/desc.txt available");
    }
#endif
}

}
