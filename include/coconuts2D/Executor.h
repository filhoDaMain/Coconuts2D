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

#ifndef COCONUTS2D_EXECUTOR_H
#define COCONUTS2D_EXECUTOR_H

#include <string>
#include <vector>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace coconuts2D {

// Singleton (maybe multithreaded)
class Executor
{
public:
    Executor(const Executor&) = delete;
    void operator=(const Executor&) = delete;
    ~Executor() = default;

    static Executor& GetInstance()
    {
        static Executor instance;
        return instance;
    }

    void ExecuteOneShot(const std::string& script);
    bool CompileOneShot(const std::string& script);
    bool Compile(void);

    // Submit to execution buffer
    void Submit(const std::string& script);
    void ExecuteBuffer(void);

private:
    Executor();
    bool DoCompile();

private:
    std::vector<std::string> m_ExecBuffer;
    sol::state m_Lua;
};

}

#endif  // COCONUTS2D_EXECUTOR_H