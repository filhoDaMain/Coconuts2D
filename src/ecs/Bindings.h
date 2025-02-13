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

#ifndef COCONUTS2D_BINDINGS_H
#define COCONUTS2D_BINDINGS_H

#include <coconuts2D/ecs/Components.h>
#include <coconuts2D/Logger.h>
#include <entt/entt.hpp>

namespace coconuts2D {

namespace Bindings {


namespace Meta
{

    template <typename C>
    auto meta_add_component(entt::registry* registry, entt::entity entity,
                            const sol::table& instance, sol::this_state s)
    {
        assert(registry);

        auto& comp = registry->emplace_or_replace<C>(
            entity,
            instance.valid() ? std::move(instance.as<C &&>()) : C{}
        );

        return sol::make_reference(s, std::ref(comp));
    }

    template <typename C>
    bool meta_has_component(entt::registry* registry, entt::entity entity)
    {
        assert(registry);
        return registry->any_of<C>(entity);
    }

    template <typename C>
    auto meta_remove_component(entt::registry* registry, entt::entity entity)
    {
        assert(registry);
        return registry->remove<C>(entity);
    }

    template <typename C>
    auto meta_get_component(entt::registry* registry, entt::entity entity,
                            sol::this_state s)
    {
        assert(registry);
        auto& comp = registry->get_or_emplace<C>(entity);
        return sol::make_reference(s, std::ref(comp));
    }

    template <typename C>
    void RegisterComponent(void)
    {
        using namespace entt::literals;

        entt::meta<C>()
            .type(entt::type_hash<C>::value())

            /* Reflected functions */
            .template func<&meta_add_component<C>>("meta_add"_hs)
            .template func<&meta_remove_component<C>>("meta_remove"_hs)
            .template func<&meta_get_component<C>>("meta_get"_hs)
            .template func<&meta_has_component<C>>("meta_has"_hs);
    }

    entt::id_type GetCompIdType(const sol::table& component)
    {
        if (!component.valid())
        {
            LOG_ERROR("Failed to get Component ID - Component was not exposed to Lua yet!");
            return -1;
        }

        const auto func = component["type_id"].get<sol::function>();
        assert(func.valid());

        if (func.valid())
        {
            return func().get<entt::id_type>();
        }
        else
        {
            LOG_ERROR("Component is missing mandatory function 'type_id()'. Must expose it to Lua!");
            return -1;
        }
    }

    // Passing meta_type
    template <typename... Args>
    inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type funcId, Args&&... args)
    {
        if (!meta)
        {
            LOG_ERROR("Invalid Meta Type!");
            return entt::meta_any{};
        }

        // If function was reflected, call it
        if (auto metafunc = meta.func(funcId); metafunc)
        {
            return metafunc.invoke({}, std::forward<Args>(args) ...);
        }

        return entt::meta_any{};
    }

    // Passing id_type
    template <typename... Args>
    inline auto InvokeMetaFunction(entt::id_type idtype, entt::id_type funcId, Args&&... args)
    {
        return InvokeMetaFunction(entt::resolve(idtype), funcId, std::forward<Args>(args) ...);
    }

}   // Meta


namespace Registry {

    void BindToLua(sol::this_state s)
    {
        using namespace entt::literals;
        sol::state_view lua{s};
        auto entt_module = lua["entt"].get_or_create<sol::table>();

        entt_module.new_usertype<entt::registry>(
            "registry",
            sol::meta_function::construct,
            sol::factories(
                []()
                {
                    return entt::registry{};
                }
            ),

            "size",
                [](const entt::registry &self)
                {
                    return self.storage<entt::entity>()->size();
                },

            "alive",
                [](const entt::registry &self)
                {
                    return self.storage<entt::entity>()->free_list();
                },

            "valid", &entt::registry::valid,

            "current", &entt::registry::current,

            "create",
                [](entt::registry &self)
                {
                    return self.create();
                },

            "destroy",
                [](entt::registry &self, entt::entity entity)
                {
                    return self.destroy(entity);
                },

            "emplace",
                [](entt::registry &self, entt::entity entity, const sol::table& comp,
                   sol::this_state s) -> sol::object
                {
                    if (!comp.valid())
                    {
                        return sol::lua_nil_t{};
                    }

                    const auto component = Meta::InvokeMetaFunction(
                        Meta::GetCompIdType(comp),
                        "meta_add"_hs,
                        &self,
                        entity,
                        comp,
                        s
                    );

                    return component ? component.cast<sol::reference>() : sol::lua_nil_t{} ;
                },
            
            "remove",
                [](entt::registry &self, entt::entity entity, const sol::table& comp)
                {

                    const auto component = Meta::InvokeMetaFunction(
                        Meta::GetCompIdType(comp),
                        "meta_remove"_hs,
                        &self,
                        entity
                    );

                    return component ? component.cast<std::size_t>() : 0 ;
                },

            "has",
                [](entt::registry &self, entt::entity entity, const sol::table& comp)
                {
                    const auto component = Meta::InvokeMetaFunction(
                        Meta::GetCompIdType(comp),
                        "meta_has"_hs,
                        &self,
                        entity
                    );

                    return component ? component.cast<bool>() : false ;
                },
            
            "get",
                [](entt::registry &self, entt::entity entity, const sol::table& comp,
                   sol::this_state s)
                {
                    const auto component = Meta::InvokeMetaFunction(
                        Meta::GetCompIdType(comp),
                        "meta_get"_hs,
                        &self,
                        entity,
                        s
                    );

                    return component ? component.cast<sol::reference>() : sol::lua_nil_t{} ;
                }
        );
    }

}   // Registry


namespace TagComponent {

    void BindToLua(lua_State* L)
    {
        sol::state_view lua(L);

        lua.new_usertype<Components::TagComponent>(
            "TagComponent", /* as it appears in Lua */

            /* Reflection */
            "type_id", &entt::type_hash<Components::TagComponent>::value,

            /* Bind Constructor   */
            sol::call_constructor,
            sol::factories(
                [](const std::string& t)
                {
                    return Components::TagComponent{t};
                 }
            ),

            /* Functions and params available in Lua */
            "tag", &Components::TagComponent::tag
         );
    }
}   // TagComponent


namespace ScriptComponent {

    void Init(entt::registry &registry, entt::entity entity)
    {
        auto &script = registry.get<Components::ScriptComponent>(entity);
        assert(script.self.valid());    // //assert script was read/called during its construction

        // Bind lua script functions
        script.lua_functions.update = script.self["update"];
        script.lua_functions.init = script.self["init"];
        script.lua_functions.destroy = script.self["destroy"];

        // Assign globals to Lua
        script.self["id"] = sol::readonly_property(
            [entity]()
            {
                return entity;
            }
        );
        script.self["registry"] = std::ref(registry);

        // If script "init" function exists (not mandatory) run it now
        if (script.lua_functions.init.valid())
        {
            script.lua_functions.init(script.self);
        }
    }

    void Release(entt::registry &registry, entt::entity entity)
    {
        auto &script = registry.get<Components::ScriptComponent>(entity);
        if (script.lua_functions.destroy.valid())
        {
            script.lua_functions.destroy(script.self);
        }

        script.self.abandon();
    }

}   // ScriptComponent

}   // Bindings

}   // coconuts2D

#endif  // COCONUTS2D_BINDINGS_H
