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
#include "Bindings.h"

#define AUTO_ARG(x) decltype(x), x

namespace coconuts2D {

Scene::Scene(uint16_t id, const std::string& name)
: m_ID(id), m_Name(name), m_IsActive(false), m_Registry(), m_Lua()
{
    m_Lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

    // Bind entt::registry - Make its functions available in Lua
    m_Lua.require("registry", sol::c_call<AUTO_ARG(&Bindings::Registry::BindToLua)>, false);

    // Bind "special" ScriptComponent - Bind script functions and tables
    m_Registry.on_construct<Components::ScriptComponent>().connect<&Bindings::ScriptComponent::Init>();
    m_Registry.on_destroy<Components::ScriptComponent>().connect<&Bindings::ScriptComponent::Release>();

    // Bind Components - Make their properties available in Lua
    Bindings::TagComponent::BindToLua(m_Lua);
    Bindings::TransformComponent::BindToLua(m_Lua);


}

Scene::~Scene()
{
    LOG_WARN("Scene {} destroyed", m_Name);
    m_Registry.clear();
}

void Scene::Run(void)
{
    LOG_INFO("Run Scene {}: {}", m_ID, m_Name);

    // Call update() from each ScriptComponent
    m_Registry.view<Components::ScriptComponent>().each(
        [&] (auto entity, auto& script)
        {
            script.lua_functions.update(script.self, 7);
        }
    );

    LOG_TRACE("Scene exited");
}

Entity Scene::NewEntity(void)
{
    return {
        m_Registry.create(),
        this
    };
}

void Scene::LoadScriptingAPI(std::vector<ScriptingAPI>& apiScripts)
{
    for (const auto& api : apiScripts)
    {
        m_Lua.require_script(api.apiName, api.script);
    }
}

}
