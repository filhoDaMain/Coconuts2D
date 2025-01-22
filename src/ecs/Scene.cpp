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

#include <coconuts2D/ecs/Scene.h>
#include <coconuts2D/ecs/Entity.h>
#include <coconuts2D/ecs/Components.h>
#include <coconuts2D/Logger.h>

namespace coconuts2D {

void Scene::Run(void)
{
    LOG_INFO("Run Scene {}: {}", m_ID, m_Name);

    // Create entity
    std::string scriptPath("../src/ecs/scripts/example.lua");
    Entity entity1 = this->NewEntity();
    entity1.AddComponent< Components::ScriptComponent >(scriptPath);

    m_Registry.view<Components::ScriptComponent>().each([] (auto entity, auto& script)
    {
        LOG_TRACE("Dev Script found");
        script.ExecuteOneShot();
        //script.Submit();
    });

    LOG_TRACE("Scene exited");
}

Entity Scene::NewEntity(void)
{
    return {
        m_Registry.create(),
        this
    };
}

}
