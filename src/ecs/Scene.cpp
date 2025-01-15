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
#include <coconuts2D/Logger.h>


// Test Lua scripting:
// -------------------------------
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
// -------------------------------

namespace coconuts2D {

void Scene::Run(void)
{
    LOG_INFO("Run Scene {}: {}", m_ID, m_Name);

	sol::state lua;
	lua.open_libraries(sol::lib::base);

    // load and execute example file
    // Execute from build dir:
    // $ ./src/Coconuts2D
	lua.script_file("../src/ecs/scripts/example.lua");

    LOG_TRACE("Scene exited");
}

}
