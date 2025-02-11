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

#include "ProjectManager.h"
#include <coconuts2D/Logger.h>
#include <coconuts2D/SceneManager.h>
#include <fstream>
#include <cassert>

namespace coconuts2D {

namespace Parser {

    namespace ROOT_NODE {

    namespace SCENEMANAGER_ROOT {
    constexpr auto node = "<SceneManager>";

        namespace SCENE_NODE {
        constexpr auto node = "<Scene>";
        constexpr auto u16_id = "id";
        constexpr auto str_name = "name";
        constexpr auto bool_isactive = "isactive";
        constexpr auto vec_entities_node = "entities";

            namespace ENTITY_NODE {
            constexpr auto node = "<Entity>";

            namespace COMPONENTS {

                namespace OPTIONAL_TAG_COMPONENT_NODE {
                    constexpr auto node = "TagComponent";
                    constexpr auto str_tag = "tag";
                }

                namespace OPTIONAL_SCRIPT_COMPONENT_NODE {
                    constexpr auto node = "ScriptComponent";
                    constexpr auto file = "file";
                }

            }   // COMPONENTS

            }   // ENTITY_NODE

        }   // SCENE_NODE
    }   // SCENEMANAGER_ROOT

    }   // ROOT_NODE
}   // Parser


void ProjectManager::SerializeEntity(YAML::Emitter& out, const Entity& entity)
{
    using namespace Parser::ROOT_NODE::SCENEMANAGER_ROOT::SCENE_NODE;

    // MAP <Entity>
    out << YAML::BeginMap << YAML::Key << ENTITY_NODE::node << YAML::BeginMap;
    {
        // Poll registry for each component this Entity might have
        using namespace ENTITY_NODE::COMPONENTS;

        // TagComponent
        if ( entity.HasComponent<Components::TagComponent>() )
        {
            out << YAML::Key << OPTIONAL_TAG_COMPONENT_NODE::node << \
            YAML::BeginMap;
            {
                out << YAML::Key << OPTIONAL_TAG_COMPONENT_NODE::str_tag << \
                    entity.GetComponent<Components::TagComponent>().tag;
            }
            out << YAML::EndMap;    // TagComponent MAP
        }

        // ScriptComponent
        if ( entity.HasComponent<Components::ScriptComponent>() )
        {
            out << YAML::Key << OPTIONAL_SCRIPT_COMPONENT_NODE::node << \
            YAML::BeginMap;
            {
                out << YAML::Key << OPTIONAL_SCRIPT_COMPONENT_NODE::file << \
                    entity.GetComponent<Components::ScriptComponent>().file;
            }
            out << YAML::EndMap;    // ScriptComponent MAP
        }
    }
    out << YAML::EndMap << YAML::EndMap;
}

void ProjectManager::SerializeScene(YAML::Emitter& out, uint16_t id)
{
    using namespace Parser::ROOT_NODE::SCENEMANAGER_ROOT;
    auto& sm = SceneManager::GetInstance();
    auto scene = sm.GetScene(id);

    if (scene)
    {
        // MAP <Scene>
        out << YAML::BeginMap << YAML::Key << SCENE_NODE::node << YAML::BeginMap;
        {
            out << YAML::Key << SCENE_NODE::u16_id << YAML::Hex << id;
            out << YAML::Key << SCENE_NODE::str_name << YAML::Value << scene->GetName();
            out << YAML::Key << SCENE_NODE::bool_isactive << \
                YAML::LongBool << ( id == sm.GetActiveSceneId() );

            // SEQUENCE of "mapped" <Entity>
            out << YAML::Key << SCENE_NODE::vec_entities_node << YAML::BeginSeq;
            for(auto entity: scene->m_Registry.view<entt::entity>()) 
            {
                Entity tmp = {entity, scene.get()};
                SerializeEntity(out, tmp);
            }
            out << YAML::EndSeq;    // SEQUENCE of <Entity>
        }
        out << YAML::EndMap << YAML::EndMap;
    }

}

bool ProjectManager::SaveProject(const std::string& projectFile)
{
    YAML::Emitter out;

    // Root
    out << YAML::BeginMap;
    {
        // <SceneManager> SEQUENCE of "mapped" <Scene>
        out << YAML::Key << Parser::ROOT_NODE::SCENEMANAGER_ROOT::node << YAML::BeginSeq;
        auto& sm = SceneManager::GetInstance();
        for (uint16_t id = 0; id < sm.GetSceneCounter(); ++id)
        {
            SerializeScene(out, id);
        }
        out << YAML::EndSeq;    // SEQUENCE of <Scene>

    }
    out << YAML::EndMap;    // Root

    // Write to project file
    std::ofstream outfile(projectFile);
    outfile << out.c_str() << std::endl;
    outfile.close();

    return true;
}

void ProjectManager::DeserializeEntity(YAML::Node& node, uint16_t sceneID)
{
    using namespace Parser::ROOT_NODE::SCENEMANAGER_ROOT::SCENE_NODE;
    LOG_TRACE("    Parsing node: {}", ENTITY_NODE::node);

    // Create empty Entity on Scene, then start adding components to it
    auto& sm = SceneManager::GetInstance();
    auto scene = sm.GetScene(sceneID);
    Entity entity = scene->NewEntity();

    // Query YAML for each possible component
    using namespace ENTITY_NODE::COMPONENTS;
    auto component_node = node[OPTIONAL_TAG_COMPONENT_NODE::node];
    if (component_node)
    {
        std::string tag = component_node[OPTIONAL_TAG_COMPONENT_NODE::str_tag].as<std::string>();
        entity.AddComponent<Components::TagComponent>(tag);
        LOG_TRACE("      Parsing node: {}", OPTIONAL_TAG_COMPONENT_NODE::node);
        LOG_TRACE("        tag: {}", tag);
    }

    component_node = node[OPTIONAL_SCRIPT_COMPONENT_NODE::node];
    if (component_node)
    {
        std::string file = component_node[OPTIONAL_SCRIPT_COMPONENT_NODE::file].as<std::string>();
        entity.AddComponent<Components::ScriptComponent>(scene->m_Lua, file);
        LOG_TRACE("      Parsing node: {}", OPTIONAL_SCRIPT_COMPONENT_NODE::node);
        LOG_TRACE("        file: {}", file);
    }

    //TODO all components from include/coconuts2D/ecs/Components.h ...
}

void ProjectManager::DeserializeScene(YAML::Node& node)
{
    using namespace Parser::ROOT_NODE::SCENEMANAGER_ROOT;
    uint16_t id = node[SCENE_NODE::u16_id].as<uint16_t>();
    std::string name = node[SCENE_NODE::str_name].as<std::string>();
    bool isactive = node[SCENE_NODE::bool_isactive].as<bool>();

    // Create Scene!
    LOG_TRACE("  Parsing node: {}", SCENE_NODE::node);
    LOG_TRACE("    id: {}", id);
    LOG_TRACE("    name: {}", name);
    LOG_TRACE("    isactive: {}", isactive);
    auto& sm = SceneManager::GetInstance();

    // Currently, Scene ID's are attributed by order of creation.
    // We just perform a simple runtime sanity check.
    assert(id == sm.NewScene(name));

    // Loop through (parse list) each sub-node of vec_entities_node
    auto vec_entities_list = node[SCENE_NODE::vec_entities_node];
    for (auto iterate : vec_entities_list)
    {
        auto entity_node = iterate[SCENE_NODE::ENTITY_NODE::node];
        if (entity_node)
        {
            DeserializeEntity(entity_node, id);
        }
    }
}

bool ProjectManager::LoadProject(const std::string& projectFile)
{
    LOG_DEBUG("LOADING PROJECT...");
    LOG_DEBUG("File: {}", projectFile);

    YAML::Node root = YAML::LoadFile(projectFile);
    auto scenemanager_node = root[Parser::ROOT_NODE::SCENEMANAGER_ROOT::node];
    if (scenemanager_node)
    {
        LOG_TRACE("Parsing node: {}", Parser::ROOT_NODE::SCENEMANAGER_ROOT::node);
        const auto& scenes_list = scenemanager_node;

        // Loop through (parse list) each sub-node which is a SCENE_NODE
        using namespace Parser::ROOT_NODE::SCENEMANAGER_ROOT;
        for (auto iterate : scenes_list)
        {
            auto scene_node = iterate[SCENE_NODE::node];
            if (scene_node)
            {
                DeserializeScene(scene_node);
            }
        }
    }

    return true;
}

}