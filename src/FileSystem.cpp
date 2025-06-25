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

#include <coconuts2D/FileSystem.h>
#include <coconuts2D/Logger.h>

namespace coconuts2D {

bool FileSystem::Init(const std::string& execPath)
{
    if (m_IsInitialized)
    {
        LOG_ERROR("FileSystem Tools can only be initialized once!");
        return false;
    }

    // Generate paths strings
    this->GenProcWDirPath();
    this->GenExecDirPath(execPath);
    this->GenConfDirPath();
    m_IsInitialized = true;

    LOG_TRACE("FileSystem paths initialized");
    return true;
}

std::string FileSystem::GetRuntimeExecDirPathImpl()
{
    if (!m_IsInitialized)
    {
        LOG_ERROR("FileSystem paths were not initialized yet!");
    }

    return m_ExecPath;
}

std::string FileSystem::GetProcWDirPathImpl()
{
    if (!m_IsInitialized)
    {
        LOG_ERROR("FileSystem paths were not initialized yet!");
    }

    return m_ProcWorkingDirPath;
}

std::string FileSystem::GetRuntimeConfDirPathImpl()
{
    if (!m_IsInitialized)
    {
        LOG_ERROR("FileSystem paths were not initialized yet!");
    }

    return m_ConfDirPath;
}

//private
void FileSystem::GenProcWDirPath(void)
{
    char buff[128];
    m_ProcWorkingDirPath = ( getcwd(buff, sizeof(buff)) ? std::string(buff) : std::string("nullptr") );
    
    /* Append '/' if missing */
    if (m_ProcWorkingDirPath.back() != '/')
    {
        m_ProcWorkingDirPath += '/';
    }
}

//private
void FileSystem::GenExecDirPath(const std::string& execPath)
{
    std::string tmp;

    /* Cut executable name from path string */
    tmp = execPath.substr(0, execPath.find_last_of("/")) + "/";


    /* (1) Executable path is the same as process working directory */
    if (tmp.compare("./") == 0)
    {
        m_ExecPath = m_ProcWorkingDirPath;
    }

    /* (2) Executable path is already an absolute path */
    else if (tmp.at(0) == '/')
    {
        m_ExecPath = tmp;
    }

    /* (3) Executable path is reletive to process working directory path */
    else
    {
        m_ExecPath = m_ProcWorkingDirPath + tmp;
    }
}

//private
void FileSystem::GenConfDirPath(void)
{
    m_ConfDirPath = m_ExecPath + Parser::PATHS::RUNTIME_CONFDIR_REL_PATH;
}

}
