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
#include <sol/sol.hpp>
#include <glm/glm.hpp>


CMRC_DECLARE(resources);
CMRC_DECLARE(scripting);

namespace coconuts2D {

ResourceManager::ResourceManager(const std::string descPath, bool loadDefaultScene)
: m_GameDescFile(descPath),
  m_DescBuffer(),
  m_NrOfScenes(),
  m_LoadScene(),
  m_ScriptingAPIPrefix(SCRIPTING_API_PREFIX),
  m_Scripts(),
  m_IsScripingAPILoaded(false)
{
    std::ifstream istr(m_GameDescFile);
    m_DescBuffer << istr.rdbuf();

    if (m_DescBuffer.str().size() == 0)
    {
        throw std::invalid_argument("GameDescFile is empty");
    }
    
    YAML::Node desc = YAML::Load(m_DescBuffer.str());

    //TODO substitue the hardcoded strings by constants
    const auto& nrOfScenes = desc[PARSER_ROOT_NR_OF_SCENES_SCALAR];
    if ( nrOfScenes && nrOfScenes.IsScalar() )
    {
        m_NrOfScenes = nrOfScenes.as<uint16_t>();
    }

    const auto& loadScene = desc[PARSER_ROOT_LOAD_SCENE_SCALAR];
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
    if (id == 0)
    {
        LOG_CRITICAL("Scene id {} is reserved for Game Editor application!", id);
        return false;
    }

    if (id > m_NrOfScenes)
    {
        LOG_CRITICAL("Scene id {} exceeds the total number of Scenes in application!", id);
        return false;
    }

    if (!m_IsScripingAPILoaded)
    {
        LoadScriptingAPI();
    }

    YAML::Node desc = YAML::Load(m_DescBuffer.str());
    const auto& scenesList = desc[PARSER_ROOT_SCENES_SEQUENCE];

    if ( scenesList && scenesList.IsSequence() )
    {
        const auto& findScene = scenesList[ (id-1) ];
        if ( findScene && scenesList.IsSequence() )
        {
            const auto& sceneName = findScene[PARSER_SCENE_NAME_SCALAR];
            if (!sceneName)
            {
                return false;
            }
            LOG_DEBUG("Loading Scene: {}: {}", id, sceneName.as<std::string>());

            // Create New Scene
            auto scenePtr = std::shared_ptr<Scene>(
                new Scene(id, sceneName.as<std::string>())
            );

            // Look for Entities:
            const auto& entitiesList = findScene[PARSER_SCENE_ENTITIES_SEQUENCE];
            if ( entitiesList && entitiesList.IsSequence() )
            {
                for (const auto& entity : entitiesList)
                {
                    LOG_TRACE("New Entity");
                    auto sceneEntity = scenePtr->NewEntity();

                    // TagComponent
                    const auto& tagComponent = entity[PARSER_ENTITY_TAGCOMPONENT_SEQUENCE];
                    if ( tagComponent && tagComponent.IsSequence() )
                    {
                        LOG_TRACE("TagComponent");
                        std::string tag;

                        for (const auto& pair : tagComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();

                            if (key == PARSER_TAGCOMPONENT_TAG_KEY_STRING)
                            {
                                tag = pair.begin()->second.as<std::string>();
                                LOG_TRACE("   {}: {}", key, tag);
                            }
                        }

                        sceneEntity.AddComponent<Components::TagComponent>(tag);
                    }

                    // TransformComponent
                    const auto& transformComponent = entity[PARSER_ENTITY_TRANSFORMCOMPONENT_SEQUENCE];
                    if ( transformComponent && transformComponent.IsSequence() )
                    {
                        LOG_TRACE("TransformComponent");
                        glm::vec2 pos, scale;
                        float rot;

                        for (const auto& pair : transformComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();

                            // pos
                            if (key == PARSER_TRANSFORMOMPONENT_POS_KEY_FLOAT_SEQ)
                            {
                                const auto& posSequence = pair.begin()->second;
                                pos = {
                                    posSequence[0].as<float>(),
                                    posSequence[1].as<float>(),
                                };
                                LOG_TRACE("   {}: [{}, {}]", key, pos.x, pos.y);
                            }

                            // scale
                            else if (key == PARSER_TRANSFORMOMPONENT_SCALE_KEY_FLOAT_SEQ)
                            {
                                const auto& scaleSequence = pair.begin()->second;
                                scale = {
                                    scaleSequence[0].as<float>(),
                                    scaleSequence[1].as<float>(),
                                };
                                LOG_TRACE("   {}: [{}, {}]", key, scale.x, scale.y);
                            }

                            // rot
                            else if (key == PARSER_TRANSFORMOMPONENT_ROT_KEY_FLOAT)
                            {
                                rot = pair.begin()->second.as<float>();
                                LOG_TRACE("   {}: {}", key, rot);
                            }
                        }

                        sceneEntity.AddComponent<Components::TransformComponent>(
                            pos, scale, rot
                        );
                    }

                    // ScriptComponent
                    const auto& scriptComponent = entity[PARSER_ENTITY_SCRIPTCOMPONENT_SEQUENCE];
                    if ( scriptComponent && scriptComponent.IsSequence() )
                    {
                        LOG_TRACE("ScriptComponent");
                        lua_State* L = scenePtr->GetLua();
                        std::string script;
                        std::string_view code;

                        for (const auto& pair : scriptComponent)
                        {
                            const auto& key = pair.begin()->first.as<std::string>();

                            if (key == PARSER_SCRIPTCOMPONENT_FILE_KEY_STRING)
                            {
                                script = pair.begin()->second.as<std::string>();
                                LOG_TRACE("   {}: {}", key, script);
                            }
                        }

                        code = GetScriptCode(script);
                        sceneEntity.AddComponent<Components::ScriptComponent>(L, code.data());
                    }
                }
            } // entities per this Scene
            
        } // Scene
    }

    return true;
}

bool ResourceManager::LoadScriptingAPI(void)
{
    auto fs_scripting = cmrc::scripting::get_filesystem();

    try
    {
        std::function< void(const std::string& path) > iterate = [&](const std::string& path)
        {
            for (auto&& entity : fs_scripting.iterate_directory(path))
            {
                std::string newPath = path + "/" + entity.filename();
                std::string apiName = newPath.substr(m_ScriptingAPIPrefix.length()+1);

                // Use dot notation to name apis like "coconuts2D.enity_api"
                std::replace(apiName.begin(), apiName.end(), '/', '.');

                if (entity.is_directory())
                {
                    iterate(newPath);
                }
                else
                {
                    auto data = fs_scripting.open(newPath);
                    ScriptingAPI newScript {
                        .script = std::string_view(data.begin(), data.end() - data.begin()),
                        .apiName = apiName
                    };
                    m_Scripts.emplace_back(newScript);
                }
            }

        };

        iterate( std::string(m_ScriptingAPIPrefix) );
    }
    catch(const std::system_error& e)
    {
        LOG_CRITICAL("Scripting API not available!");
        return false;
    }
    
    m_IsScripingAPILoaded = true;
    return true;
}

std::string_view ResourceManager::GetScriptCode(const std::string& script)
{
    auto fs_resources = cmrc::resources::get_filesystem();
    std::string_view code;

    if ( fs_resources.is_file(script) )
    {
        LOG_TRACE("Reading script code from {}", script);
        auto data = fs_resources.open(script);
        code = std::string_view(data.begin(), data.end() - data.begin());
    }
    else
    {
        LOG_CRITICAL("Trying to load script code from non existing script: {}", script);
    }

    return code;
}

}
