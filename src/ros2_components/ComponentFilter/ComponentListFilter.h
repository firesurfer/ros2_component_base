/*
 * Copyright 2016 <Lennart Nachtigall> <firesurfer65@yahoo.de>
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
 *
 */

#pragma once

#include "ComponentInfo.h"
namespace ros2_components
{
/**
 * @brief The ComponentListFilter class
 * How to use this class:
 * Inherit from this class and override the Filter function. For example filter the components by their id, node or anything else.
 */
class ComponentListFilter
{
public:
    ComponentListFilter();
    virtual std::vector<ComponentInfo> Filter(std::vector<ComponentInfo> infos);
};
}
