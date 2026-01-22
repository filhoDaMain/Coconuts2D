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

#include "EditorScene.h"
#include <coconuts2D/Application.h>
#include <coconuts2D/SceneManager.h>
#include <coconuts2D/ResourceManager.h>
#include <string>
#include <cstdlib>
#include <memory>

 
int main(int argc, char* argv[])
{
    // Set LUA_PATH to include Coconuts2D Lua API scripts
    std::string lua_api_dir(COCONUTS2D_SCRIPTING_API_DIR);
    std::string env_lua_path = "$LUA_PATH;" + lua_api_dir + "/?.lua";
    (void) setenv("LUA_PATH", env_lua_path.c_str(), 1);

    coconuts2D::Application app( static_cast<std::string>(argv[0]));

    // Create the Game Editor specific scene
    auto editor = std::shared_ptr<coconuts2D::Scene>(
        new coconuts2D::EditorScene(0, "Game Editor Scene"))
    ;
    auto& sm = coconuts2D::SceneManager::GetInstance();
    sm.AddCustomScene(editor);

    coconuts2D::ResourceManager rm;
    rm.LoadVirtualFS();

    app.Play();
    return 0;
}
 