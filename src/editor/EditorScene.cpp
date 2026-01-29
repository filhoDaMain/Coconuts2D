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
#include <coconuts2D/Logger.h>

#define DEBUG_CODE

#ifdef DEBUG_CODE
// Testing
#include "ProjectManager.h"
#include <coconuts2D/SceneManager.h>
#include <coconuts2D/ResourceManager.h>
#endif

namespace coconuts2D {

void EditorScene::Run(void)
{
    LOG_INFO("Run Scene {}: {}", m_ID, m_Name);

    // This is just for fast prototyping
#ifdef DEBUG_CODE
    //auto& pm = ProjectManager::GetInstance();
    //pm.NewProject(ProjectManager::ProjectTemplate::NewProject, "/Users/temprilho/dev/Coconuts2D_Projects/Example1");

    //auto& sm = SceneManager::GetInstance();
    //auto sceneID = sm.NewScene("Game Scene");

    ResourceManager rm;
    rm.LoadScene(1);    // Load a Scene from a resource description file

#endif


	LOG_TRACE("Scene exited");
}

}
