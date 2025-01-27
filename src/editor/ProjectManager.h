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

#ifndef COCONUTS2D_PROJECTMANAGER_H
#define COCONUTS2D_PROJECTMANAGER_H

#include <string>
#include <yaml-cpp/yaml.h>

namespace coconuts2D {

class ProjectManager
{
public:
    ProjectManager(const ProjectManager&) = delete;
    void operator=(const ProjectManager&) = delete;
    ~ProjectManager() = default;

    static ProjectManager& GetInstance()
    {
        static ProjectManager instance;
        return instance;
    }

    bool SaveProject(const std::string projectFile);
    bool LoadProject(const std::string projectFile);

private:
    ProjectManager() = default;
    void SerializeEntities(YAML::Emitter& out, uint16_t sceneId);
    void SerializeScenes(YAML::Emitter& out);
};

}

#endif  // COCONUTS2D_PROJECTMANAGER_H