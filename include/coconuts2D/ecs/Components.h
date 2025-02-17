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

#ifndef COCONUTS2D_COMPONENTS_H
#define COCONUTS2D_COMPONENTS_H

#include <string>
#include <sol/sol.hpp>
#include <glm/glm.hpp>

namespace coconuts2D {

namespace Components
{

    struct TagComponent
    {
        std::string tag;

        TagComponent(void) : tag("untagged") {}
        TagComponent(const std::string& t) : tag(t) {}
    };

    struct TransformComponent
    {
        glm::vec2   position;
        glm::vec2   scale;
        float       rotationRadians;

        TransformComponent()
        : position(glm::vec2(0.0f)), scale(glm::vec2(1.0f)), rotationRadians(0.0f) {}

        TransformComponent(
            const glm::vec2& _position,
            const glm::vec2& _scale = glm::vec2(1.0f),
            float _rotationsRadians = 0.0f
        )
        : position(_position), scale(_scale), rotationRadians(_rotationsRadians) {}
    };

    struct ScriptComponent
    {
        sol::state_view lua;
        std::string file;
        sol::table self;

        struct
        {
            sol::function init;
            sol::function update;
            sol::function destroy;
        } lua_functions;

        ScriptComponent(lua_State* L, const std::string& f)
        : lua(L), file(f)
        {
            auto script = lua.load_file(file);
            assert(script.valid());
            self = script.call();
        }
    };

}

}

#endif  // COCONUTS2D_COMPONENTS_H

