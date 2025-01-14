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

namespace coconuts2D {

Application::Application()
: m_IsPlaying(false)
{
    LOG_INIT();
    LOG_DEBUG("Initing application...");

    auto& sm = SceneManager::GetInstance();
    sm.SetActiveScene( sm.NewScene("Game Editor Scene") );
}

Application::~Application()
{

}

void Application::Play(void)
{
    auto& sm = SceneManager::GetInstance();
    Scene scene = sm.GetActiveScene();

    scene.Run();
}

void Application::Pause(void)
{

}

void Application::Stop(void)
{

}

}
