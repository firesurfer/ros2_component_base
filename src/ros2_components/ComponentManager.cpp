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

#include "ComponentManager.h"


namespace ros2_components
{
ComponentManager::ComponentManager(rclcpp::node::Node::SharedPtr _localNode)
{
    using namespace std::placeholders;
    //Variable Assignments
    this->RosNode = _localNode;

    //Start responder thread
    this->responder_thread = std::make_unique<std::thread>(std::bind(&ComponentManager::RespondingTask,this));
    //Qos Profile
    //rmw_qos_profile_services_default
    component_manager_profile = rmw_qos_profile_parameters;
    component_manager_profile.depth = 1000;
    //component_manager_profile.history = RMW_QOS_POLICY_KEEP_ALL_HISTORY;

    //Subscriptions
    this->ListComponentsResponseSubscription = RosNode->create_subscription<ros2_components_msg::msg::ListComponentsResponse>("ListComponentsResponse", std::bind(&ComponentManager::ListComponentsResponseCallback, this,_1), component_manager_profile);

    //Publishers
    this->ListComponentsRequestPublisher = RosNode->create_publisher<ros2_components_msg::msg::ListComponentsRequest>("ListComponentsRequest",component_manager_profile);
    std::srand(std::time(0)); // use current time as seed for random generator
    LOG(Info) << "Created new instance of a ComponentManager" << std::endl;
}

ComponentManager::ComponentManager(rclcpp::node::Node::SharedPtr _localNode, EntityBase::SharedPtr _baseEntity): ComponentManager(_localNode)
{

    //Variable Assignments
    this->BaseEntity = _baseEntity; //TODO register to components and add change callbacks
    for(EntityBase::SharedPtr & child:BaseEntity->getAllChilds())
    {
        connect(child.get(), &EntityBase::childAdded, this, &ComponentManager::OnChildAdded, Qt::DirectConnection);
        connect(child.get(), &EntityBase::childRemoved,this, &ComponentManager::OnChildRemoved, Qt::DirectConnection);
    }
    //Subscriptions
    using namespace std::placeholders;
    this->ListComponentsRequestSubscription = RosNode->create_subscription<ros2_components_msg::msg::ListComponentsRequest>("ListComponentsRequest", std::bind(&ComponentManager::ListComponentsRequestCallback, this,_1), component_manager_profile);

    //Publishers
    this->ListComponentsResponsePublisher = RosNode->create_publisher<ros2_components_msg::msg::ListComponentsResponse>("ListComponentsResponse",component_manager_profile);

}

ComponentManager::~ComponentManager()
{
    this->abort =true;
    responder_thread->join();
}

bool ComponentManager::IDAlreadyInUse(uint64_t id)
{
    for(auto & myInfo: Components)
    {
        if(myInfo.id == id)
            return true;
    }
    return false;
}

std::vector<ComponentInfo> ComponentManager::ListComponents()
{
    return Components;
}

int64_t ComponentManager::CountComponents()
{
    return Components.size();
}

std::vector<string> ComponentManager::ListNodes()
{
    return this->RosNode->get_node_graph_interface()->get_node_names();
}

std::vector<ComponentInfo> ComponentManager::ListComponentsBy(ComponentListFilter filter)
{
    return filter.Filter(this->Components);
}

ComponentInfo ComponentManager::GetInfoToId(uint64_t id, bool *success)
{
    if(success != NULL)
        *success = false;
    for(auto & comp  : Components)
    {
        if(id == comp.id)
        {
            if(success != NULL)
                *success= true;
            return comp;
        }
    }
    ComponentInfo dummy;
    return dummy;
}

void ComponentManager::UpdateComponentsList()
{
    ros2_components_msg::msg::ListComponentsRequest::SharedPtr request = std::make_shared<ros2_components_msg::msg::ListComponentsRequest>();
    request->nodename = RosNode->get_name();
    this->ListComponentsRequestPublisher->publish(request);
}

bool ComponentManager::CheckIfChildsAreAvailable(uint64_t id)
{
    std::function<bool(uint64_t)> checkChilds = [&](uint64_t id) ->bool
    {
        bool success = false;
        ComponentInfo info = this->GetInfoToId(id,&success);
        if(!success)
            return false;
        for(int64_t & childId : info.childIds )
        {
            bool result = checkChilds(childId);
            return result;
        }
        return true;
    };
    return checkChilds(id);
}

void ComponentManager::ListComponentsRequestCallback(ros2_components_msg::msg::ListComponentsRequest::SharedPtr msg)
{
    if(RosNode->get_name() != msg->nodename)
    {
        triggerResponseQueue.push(true);
    }
}

void ComponentManager::ListComponentsResponseCallback(ros2_components_msg::msg::ListComponentsResponse::SharedPtr msg)
{
    if(RosNode->get_name() != msg->nodename)
    {
        bool foundInList = false;
        ComponentInfo currentInfo = ComponentInfoFactory::FromListComponentsResponseMessage(msg);
        for(auto & myInfo: Components)
        {
            if(myInfo.name == msg->componentname)
            {
                foundInList = true;
                myInfo = currentInfo;
                //TODO emit changed event and implement equal method
            }
        }
        if(!foundInList)
        {
            Components.push_back(currentInfo);
            emit NewComponentFound(currentInfo);
        }
    }

}





void ComponentManager::RespondingTask()
{
    int count = 0;
    while(!abort)
    {
        auto responseFunc = [&]()
        {
            if(BaseEntity)
            {
                ros2_components_msg::msg::ListComponentsResponse::SharedPtr msg = ComponentInfoFactory::FromEntity(this->BaseEntity).toRosMessage();
                msg->nodename = RosNode->get_name();
                this->ListComponentsResponsePublisher->publish(msg);
                std::function<void(EntityBase::SharedPtr)> iteratingFunc = [&](EntityBase::SharedPtr ent)
                {
                    auto respMsg = ComponentInfoFactory::FromEntity(ent).toRosMessage();
                    respMsg->nodename = RosNode->get_name();
                    this->ListComponentsResponsePublisher->publish(respMsg);
                    ent->IterateThroughAllChilds(iteratingFunc);
                };

                this->BaseEntity->IterateThroughAllChilds(iteratingFunc);
            }
        };
        while(triggerResponseQueue.empty() && !abort)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            count ++;
            if(count > 100)
            {
                count = 0;
                responseFunc();
            }
        }
        while(!triggerResponseQueue.empty())
        {
            triggerResponseQueue.pop();
            responseFunc();
        }
    }
}

void ComponentManager::OnChildAdded(EntityBase::SharedPtr child, EntityBase::SharedPtr parent, bool remote)
{
    LOG(Info) << "Child added: " << child->getName() << std::endl;
    //Connect to add events
    connect(child.get(),&EntityBase::childAdded,this, &ComponentManager::OnChildAdded);
    connect(child.get(),&EntityBase::childRemoved,this, &ComponentManager::OnChildRemoved);

    //Publish component information
    ros2_components_msg::msg::ListComponentsResponse::SharedPtr msg = ComponentInfoFactory::FromEntity(child).toRosMessage();
    msg->nodename = RosNode->get_name();
    this->ListComponentsResponsePublisher->publish(msg);

}

void ComponentManager::OnChildRemoved(EntityBase::SharedPtr child, EntityBase::SharedPtr parent, bool remote)
{
    disconnect(child.get(), &EntityBase::childAdded, this, &ComponentManager::OnChildAdded);
    disconnect(child.get(), &EntityBase::childRemoved, this, &ComponentManager::OnChildRemoved);

    ros2_components_msg::msg::ListComponentsResponse::SharedPtr msg = ComponentInfoFactory::FromEntity(child).toRosMessage();
    msg->nodename = RosNode->get_name();
    msg->deleted = true;
    this->ListComponentsResponsePublisher->publish(msg);
}





}

