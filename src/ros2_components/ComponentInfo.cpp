/*
 * Copyright 2018 <Lennart Nachtigall> <firesurfer127@gmail.com>
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

#include "ComponentInfo.h"
namespace ros2_components
{
ComponentInfo::ComponentInfo() : id(0), parentId(0), machineip(0), subscriber(false)
{

}

ComponentInfo::ComponentInfo(const ComponentInfo&  info)
{
    this->id = info.id;
    this->childIds = info.childIds;
    this->childTypes = info.childTypes;
    this->machineip = info.machineip;
    this->name = info.name;
    this->nodename = info.nodename;
    this->parentId = info.parentId;
    this->parentType = info.parentType;
    this->type = info.type;
    this->subscriber = info.subscriber;
}

}
