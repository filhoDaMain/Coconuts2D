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

// Testing
#include "ProjectManager.h"

namespace coconuts2D {

void EditorScene::Run(void)
{
    LOG_INFO("Run Scene {}: {}", m_ID, m_Name);

    auto& pm = ProjectManager::GetInstance();
    pm.NewProject(ProjectManager::ProjectTemplate::NewProject, "/Users/temprilho/dev/Coconuts2D_Projects/Example1");


	LOG_TRACE("Scene exited");
}

}
