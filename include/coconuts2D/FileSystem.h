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

#ifndef COCONUTS2D_FILESYSTEM_H
#define COCONUTS2D_FILESYSTEM_H

#include <string>

namespace coconuts2D {

namespace {
namespace Parser
{
    namespace PATHS
    {
        /* Relative to binary's path */
        constexpr auto RUNTIME_CONFDIR_REL_PATH = "";   // empty == same as binary's current location
    }
}
} //namespace


class FileSystem
{
public:
    FileSystem(const FileSystem&) = delete;
    void operator=(const FileSystem&) = delete;
    ~FileSystem() = default;

    static FileSystem& GetInstance()
    {
        static FileSystem instance;
        return instance;
    }

    bool Init(const std::string& execPath);
    std::string GetRuntimeExecDirPathImpl();
    std::string GetProcWDirPathImpl();
    std::string GetRuntimeConfDirPathImpl();

private:
    FileSystem()
    :   m_IsInitialized(false),
        m_ExecPath(),
        m_ProcWorkingDirPath(),
        m_ConfDirPath()
    {}

    void GenProcWDirPath(void);
    void GenExecDirPath(const std::string& execPath);
    void GenConfDirPath(void);

private:
    bool m_IsInitialized;
    std::string m_ExecPath;             // abs path
    std::string m_ProcWorkingDirPath;   // abs path
    std::string m_ConfDirPath;          // abs path
};

}

#endif // COCONUTS2D_FILESYSTEM_H
