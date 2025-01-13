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

namespace coconuts2D {

Application::Application()
: m_IsRunning(false)
{
    // Initializations
}

Application::~Application()
{

}

void Application::Run(void)
{
    m_IsRunning.store(true);

    while (m_IsRunning.load())
    {
        // Get Active Scene
        // Run Scene
    }
}

}
