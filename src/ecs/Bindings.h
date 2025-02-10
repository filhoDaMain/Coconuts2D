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

#ifndef COCONUTS2D_BINDINGS_H
#define COCONUTS2D_BINDINGS_H

#include <coconuts2D/ecs/Components.h>
#include <coconuts2D/Logger.h>
#include <entt/entt.hpp>

namespace coconuts2D {

namespace Bindings {


namespace ScriptComponent {

    void Init(entt::registry &registry, entt::entity entity)
    {
        auto &script = registry.get<Components::ScriptComponent>(entity);
        assert(script.self.valid());    // //assert script was read/called during its construction

        // Bind lua script functions
        script.lua_functions.update = script.self["update"];
        script.lua_functions.init = script.self["init"];
        script.lua_functions.destroy = script.self["destroy"];

        // Assign globals to Lua
        script.self["id"] = sol::readonly_property(
            [entity]()
            {
                return entity;
            }
        );
        script.self["registry"] = std::ref(registry);

        // If script "init" function exists (not mandatory) run it now
        if (script.lua_functions.init.valid())
        {
            script.lua_functions.init(script.self);
        }
    }

    void Release(entt::registry &registry, entt::entity entity)
    {
        auto &script = registry.get<Components::ScriptComponent>(entity);
        if (script.lua_functions.destroy.valid())
        {
            script.lua_functions.destroy(script.self);
        }
    
        script.self.abandon();
    }

}   // ScriptComponent

}   // Bindings

}   // coconuts2D

#endif  // COCONUTS2D_BINDINGS_H
