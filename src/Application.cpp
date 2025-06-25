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

#include <coconuts2D/Application.h>
#include <coconuts2D/Logger.h>
#include <coconuts2D/SceneManager.h>
#include <coconuts2D/FileSystem.h>

namespace coconuts2D {

Application::Application(const std::string& argv0)
: m_IsPlaying(false)
{
    LOG_INIT();
    LOG_DEBUG("Initing application...");

    auto& fs = FileSystem::GetInstance(); fs.Init(argv0);
    LOG_TRACE("ProcWDir = {}", fs.GetProcWDirPathImpl());
    LOG_TRACE("RuntimeConfDir = {}", fs.GetRuntimeConfDirPathImpl());
    LOG_TRACE("ExecDir = {}", fs.GetRuntimeExecDirPathImpl());
}

Application::~Application()
{

}

void Application::Play(void)
{
    auto& sm = SceneManager::GetInstance();

    // Create a Scene and watch it being executed
    sm.NewScene("Game Scene");

    m_IsPlaying.store(true);
    while (m_IsPlaying.load())
    {
        try
        {
            auto scenePtr = sm.GetActiveScene();
            scenePtr->Run();
        }
        catch(const std::exception& e)
        {
            LOG_CRITICAL("No active scene! Exiting...");
            exit(1);
        }
    }
}

void Application::Pause(void)
{

}

void Application::Stop(void)
{

}

}
