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

#ifndef COCONUTS2D_APPLICATION_H
#define COCONUTS2D_APPLICATION_H

#include <atomic>
#include <string>

namespace coconuts2D {

class Application
{
public:
    Application(const std::string& argv0);
    Application() = delete;
    ~Application();

    void Play(void);
    void Pause(void);
    void Stop(void);

private:
    std::atomic<bool> m_IsPlaying;
};

}

#endif  // COCONUTS2D_APPLICATION_H
