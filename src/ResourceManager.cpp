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

CMRC_DECLARE(resources);

namespace coconuts2D {

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::LoadVirtualFS()
{
    auto fs = cmrc::resources::get_filesystem();

    try
    {
        auto gameDesc = fs.open("res/desc.txt");
        auto desc = std::string( gameDesc.begin(), gameDesc.end() );
        LOG_INFO("desc.txt: {}", desc);

        // Example of a script
        auto example1Script = fs.open("res/scripts/example1.lua");
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
}

}
