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

#include <coconuts2D/SceneManager.h>
#include <coconuts2D/ecs/Scene.h>

namespace coconuts2D {

SceneManager::SceneManager()
: m_ScenesList(), m_ActiveSceneID(0)
{
}

SceneManager::~SceneManager()
{

}

uint16_t SceneManager::NewScene(const std::string& name)
{
    uint16_t tmpID = m_ScenesList.size();
    Scene scene(tmpID, name);
    m_ScenesList.push_back(scene);

    return tmpID;
}

void SceneManager::RemoveScene(uint16_t id)
{
    m_ScenesList.erase( m_ScenesList.begin() + id );
}

const Scene& SceneManager::GetActiveScene(void)
{
    return m_ScenesList.at(m_ActiveSceneID);
}

void SceneManager::SetActiveScene(uint16_t id)
{
    m_ActiveSceneID = id;
}

}
