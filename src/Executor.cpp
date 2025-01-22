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

#include <coconuts2D/Executor.h>

namespace coconuts2D {

Executor::Executor()
: m_ExecBuffer(), m_Lua()
{
    m_Lua.open_libraries(sol::lib::base);
}

void Executor::ExecuteOneShot(const std::string& script)
{
    m_Lua.script_file(script);
}

bool Executor::CompileOneShot(const std::string& script)
{
    //TODO
    return true;
}

bool Executor::Compile(void)
{
    //TODO
    return true;
}


void Executor::Submit(const std::string& script)
{
    m_ExecBuffer.push_back(script);
}

void Executor::ExecuteBuffer(void)
{
    for (auto script : m_ExecBuffer)
    {
        m_Lua.script_file(script);
    }

    m_ExecBuffer.clear();
}

bool Executor::DoCompile()
{
    //TODO
    return true;
}

}