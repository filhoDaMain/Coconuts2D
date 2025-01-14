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

#ifndef COCONUTS2D_LOGGER_H
#define COCONUTS2D_LOGGER_H

#include <ctime>

// Set Logger level based on build type
#ifdef COCONUTS2D_BUILDTYPE_DEBUG
    #define SPDLOG_ACTIVE_LEVEL 0
#else
    #define SPDLOG_ACTIVE_LEVEL 2
#endif
#include <spdlog/spdlog.h>

namespace coconuts2D {

#define LOG_INIT() {\
    spdlog::set_level(spdlog::level::trace);\
    spdlog::set_pattern("%^[+ %3ims] [%L] [%@]: %v%$");\
    time_t timestamp;\
    time(&timestamp);\
    SPDLOG_INFO("Init Time: {}", ctime(&timestamp));\
}

#define LOG_TRACE(...)      SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)   SPDLOG_CRITICAL(__VA_ARGS__)

}

#endif  // COCONUTS2D_LOGGER_H