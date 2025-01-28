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

#ifndef COCONUTS2D_ENTITY_H
#define COCONUTS2D_ENTITY_H

#include <entt/entt.hpp>
#include <coconuts2D/ecs/Scene.h>

namespace coconuts2D {

// Forward declared
class Entity;

class Entity
{
public:
    Entity(entt::entity backend, Scene* scene)
    : m_EntityBackend(backend), m_Scene(scene) {}

    template <typename C, typename ... Args>
    C& AddComponent(Args && ... args)
    {
        return m_Scene->m_Registry.emplace<C>(m_EntityBackend, std::forward<Args>(args)...);
    }

    template <typename C>
    std::size_t RemoveComponent()
    {
        return m_Scene->m_Registry.remove<C>(m_EntityBackend);
    }

    template <typename C>
    C& GetComponent() const
    {
        return m_Scene->m_Registry.get<C>(m_EntityBackend);
    }

    template <typename C>
    bool HasComponent() const
    {
        return m_Scene->m_Registry.all_of<C>(m_EntityBackend);
    }

private:
    entt::entity m_EntityBackend;
    Scene* m_Scene;
};

}

#endif  // COCONUTS2D_ENTITY_H
