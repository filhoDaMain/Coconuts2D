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

#ifndef COCONUTS2D_SCENEMANAGER_H
#define COCONUTS2D_SCENEMANAGER_H

#include <coconuts2D/ecs/Scene.h>
#include <memory>
#include <vector>

namespace coconuts2D {

class SceneManager
{
public:
    SceneManager(const SceneManager&) = delete;
    void operator=(const SceneManager&) = delete;
    ~SceneManager();

    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }

    uint16_t NewScene(const std::string& name=std::string("Scene"));
    void RemoveScene(uint16_t id);
    std::shared_ptr<Scene> GetScene(uint16_t id);
    std::shared_ptr<Scene> GetActiveScene(void);
    uint16_t GetActiveSceneId(void) {  return m_ActiveSceneID; }
    uint16_t GetSceneCounter(void) { return m_ScenesPtrList.size(); }

    /**
     * Sets next active scene after current finishes running.
     * If no next active scene is specified, the next from the list
     * starts running.
     */
    void SetNextActiveScene(uint16_t id);

private:
    SceneManager();

private:
    std::vector< std::shared_ptr<Scene> > m_ScenesPtrList;
    uint16_t m_ActiveSceneID;
    bool m_QueueNextScene;
};

}

#endif  // COCONUTS2D_SCENEMANAGER_H