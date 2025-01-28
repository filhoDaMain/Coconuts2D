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

#ifndef COCONUTS2D_COMPONENTS_H
#define COCONUTS2D_COMPONENTS_H

#include <string>
#include <coconuts2D/Executor.h>

namespace coconuts2D {

namespace Components
{

    struct TagComponent
    {
        std::string tag;
        TagComponent(const std::string& t) : tag(t) {}
    };

    struct ScriptComponent
    {
        std::string script;

        ScriptComponent(const std::string& scriptFile) : script(scriptFile) {}
        ~ScriptComponent() = default;

        void ExecuteOneShot(void)
        {
            auto& ex = Executor::GetInstance();
            ex.ExecuteOneShot(script);
        }

        void Submit(void)
        {
            auto& ex = Executor::GetInstance();
            ex.Submit(script);
        }
    };

}

}

#endif  // COCONUTS2D_COMPONENTS_H

