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

#ifndef COCONUTS2D_SCENE_H
#define COCONUTS2D_SCENE_H

#include <cstdint>
#include <string>
#include <entt/entt.hpp>

namespace coconuts2D {

// Forward declared
class Entity;
class ProjectManager;

class Scene
{
public:
    Scene(uint16_t id, const std::string& name)
    : m_ID(id), m_Name(name), m_IsActive(false), m_Registry()
    {}
    ~Scene() = default;
    virtual void Run(void);

    Entity NewEntity(void);

    std::string GetName(void) { return m_Name; }

protected:
    uint16_t m_ID;
    std::string m_Name;
    bool m_IsActive;
    entt::registry m_Registry;

private:
    friend class Entity;
    friend class ProjectManager;
};

}

#endif  // COCONUTS2D_SCENE_H
