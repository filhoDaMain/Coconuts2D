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
#include <cassert>

#ifdef COCONUTS2D_COMPILE_GAME_EDITOR
    #include <coconuts2D/editor/EditorScene.h>
#endif

namespace coconuts2D {

SceneManager::SceneManager()
: m_ScenesPtrList(), m_ActiveSceneID(0), m_QueueNextScene(false)
{
    assert(m_ScenesPtrList.empty());

    // Create "special" Game Editor scene if needed
#ifdef COCONUTS2D_COMPILE_GAME_EDITOR
    m_ScenesPtrList.push_back( std::shared_ptr<Scene>(new EditorScene(0, "Game Editor Scene")) );
#endif

    // Set first scene to run
    SetNextActiveScene(0);
}

SceneManager::~SceneManager()
{

}

uint16_t SceneManager::NewScene(const std::string& name)
{
    uint16_t tmpID = m_ScenesPtrList.size();
    m_ScenesPtrList.push_back( std::shared_ptr<Scene>(new Scene(tmpID, name)) ); 
    return tmpID;
}

void SceneManager::RemoveScene(uint16_t id)
{
    m_ScenesPtrList.erase( m_ScenesPtrList.begin() + id );
}

std::shared_ptr<Scene> SceneManager::GetScene(uint16_t id)
{
    assert(id < m_ScenesPtrList.size());
    return m_ScenesPtrList.at(id);
}

std::shared_ptr<Scene> SceneManager::GetActiveScene(void)
{
    // m_ActiveSceneID was already updated.
    if (m_QueueNextScene)
    {
        m_QueueNextScene = false;
        return m_ScenesPtrList.at(m_ActiveSceneID);
    }

    // Return next scene from the list
    return m_ScenesPtrList.at(++m_ActiveSceneID);
}

void SceneManager::SetNextActiveScene(uint16_t id)
{
    m_ActiveSceneID = id;
    m_QueueNextScene = true;
}

}
