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

#include "Entity.h"
namespace ros2_components {

EntityBase::EntityBase(int64_t _id, bool _subscribe, std::shared_ptr<rclcpp::node::Node> _parentNode, string _className)
{
    this->id = _id;
    this->subscriber = _subscribe;
    this->parentNode = _parentNode;
    this->className = _className;
    this->active = true;
    REFLECT(id);
    REFLECT(virtualEntity);
    REFLECT(className);
    REFLECT(active);
    qRegisterMetaType<int64_t>("int64_t");
    qRegisterMetaType<std::string>("std::string");

}

EntityBase::~EntityBase()
{
    std::cout << "Destroying: " << getName() << std::endl;
}

int64_t EntityBase::getId()
{
    return id;
}

string EntityBase::getName()
{
    return getClassName()  + std::to_string(id);
}

string EntityBase::getClassName()
{
    /* const QMetaObject* metaObject = this->metaObject();

    std::string localClassName = metaObject->className();
    localClassName.erase(0, localClassName.find_last_of(":")+1);*/
    return className;
}

void EntityBase::addChild(std::shared_ptr<EntityBase> child, bool remote)
{
    LOG(LogLevel::Debug) << "addChild called with: " << child->getName() << "From: " << getName()<< std::endl;
    childs.push_back(child);
    child->setParent(shared_from_this());
    connect(child.get(), &EntityBase::childAdded,this, &EntityBase::on_child_added,Qt::DirectConnection);
    connect(child.get(), &EntityBase::childRemoved,this, &EntityBase::on_child_removed,Qt::DirectConnection);
    emit childAdded(child,child->getParent(),0,remote);
}

void EntityBase::removeChild(std::shared_ptr<EntityBase> child, bool remote)
{
    auto iteratorPos = std::find(childs.begin(), childs.end(), child) ;
    if(iteratorPos != childs.end())
    {
        childs.erase(iteratorPos);
    }
    else
        throw std::runtime_error("Can't remove given child - child not found!");
    disconnect(child.get(), &EntityBase::childAdded,this, &EntityBase::on_child_added);
    emit childRemoved(child,child->getParent(),0, remote);
}

std::shared_ptr<EntityBase> EntityBase::getChildById(int64_t id)
{
    for(auto & child: childs)
    {
        if(child->getId() == id)
        {
            //Do to bug in ROS 2 this might throw an exception
            //if(!child->WasMetaInformationUpdated())
            //child->updateParameters();
            return child;
        }
    }
    throw std::runtime_error("Child with id: " + std::to_string(id) + " not found");
}

uint64_t EntityBase::countChilds()
{

    return childs.size();
}

std::shared_ptr<EntityBase> EntityBase::getParent()
{
    return this->parent;
}

void EntityBase::setParent(std::shared_ptr<EntityBase> par)
{
    this->parent = par;
}

string EntityBase::getTopicName()
{
    if(!isSubscriber())
    {
        return pubBase->get_topic_name();
    }
    else
        return subBase->get_topic_name();
}

void EntityBase::updateParameters()
{

    LOG(LogLevel::Info) << "Updating Parameters" << std::endl;
    updated = true;
    std::vector<std::string> myParameters;
    for(auto & par: internalmap)
    {
        myParameters.push_back(getName()+ "."+par->key);
    }

    auto parameters = parameterClient->get_parameters({myParameters});

    if (parameters.wait_for(std::chrono::seconds(10)) != std::future_status::ready)
    {
        LOG(LogLevel::Fatal) <<"get parameters service call failed"<< std::endl;
        return;

    }
    for (auto & parameter : parameters.get())
    {

        std::string reducedParameter = parameter.get_name();
        reducedParameter.erase(0,reducedParameter.find_last_of(".")+1);
        for (auto & internal_val : internalmap)
        {
            if(internal_val->key == reducedParameter)
            {


                /*
                         * uint8 PARAMETER_NOT_SET=0
                         * uint8 PARAMETER_BOOL=1
                         * uint8 PARAMETER_INTEGER=2
                         * uint8 PARAMETER_DOUBLE=3
                         * uint8 PARAMETER_STRING=4
                         * uint8 PARAMETER_BYTES=5
                         */

                switch(parameter.get_type())
                {
                case rcl_interfaces::msg::ParameterType::PARAMETER_BOOL:
                {
                    SpecificElement<bool>* elem = static_cast<SpecificElement<bool>*>(internal_val);
                    elem->setValue(parameter.as_bool());
                    break;
                }
                case rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER:
                {
                    SpecificElement<int64_t>* elem = static_cast<SpecificElement<int64_t>*>(internal_val);
                    elem->setValue(parameter.as_int());
                    break;
                }
                case rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE:
                {
                    SpecificElement<double>* elem = static_cast<SpecificElement<double>*>(internal_val);
                    elem->setValue(parameter.as_double());
                    break;
                }
                case rcl_interfaces::msg::ParameterType::PARAMETER_STRING:
                {
                    SpecificElement<std::string>* elem = static_cast<SpecificElement<std::string>*>(internal_val);
                    elem->setValue(parameter.as_string());
                    break;
                }
                case rcl_interfaces::msg::ParameterType::PARAMETER_BYTES:
                {
                    SpecificElement<std::vector<uint8_t>>* elem = static_cast<SpecificElement<std::vector<uint8_t>>*>(internal_val);
                    elem->setValue(parameter.as_bytes());
                    break;
                }
                case rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET:
                {
                    break;
                }

                }

            }
        }
    }


    emit parametersUpdated();
}

void EntityBase::publishMetaInformation()
{
    LOG(LogLevel::Info) << "Publish meta information in: " <<  getName() << std::endl;
    std::vector< rclcpp::parameter::ParameterVariant> params;

    for(auto it = internalmap.begin(); it != internalmap.end(); it++)
    {
        params.push_back((*it)->getParameterVariant(getName()));
        //(*it)->print();
    }
    auto set_parameters_results = this->parameterClient->set_parameters(params);
    if (set_parameters_results.wait_for(std::chrono::seconds(20)) != std::future_status::ready)
    {
        throw std::runtime_error("Couldn't access parameter server!");

    }

}

string EntityBase::getAutogeneratedClassName()
{
    int status;
    char * demangled = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
    std::string tempname =  std::string(demangled);
    if(tempname.find_last_of(":") != std::string::npos)
        tempname.erase(0, tempname.find_last_of(":")+1);

    return tempname;
}

void EntityBase::on_child_added(std::shared_ptr<EntityBase> child,std::shared_ptr<EntityBase> parent,int depth, bool remote)
{
    if(parent != NULL && child != NULL)
        LOG(LogLevel::Info) << "child"<<child->getName()<< " added to: " <<parent->getName() << " depth: " << depth << std::endl;


    if(!isSubscriber() && advertised)
    {

        Advertise(AdvertisementType::Enum::Change);
    }
    emit childAdded(child,parent, depth+1,remote);
}

void EntityBase::on_child_removed(std::shared_ptr<EntityBase> child, std::shared_ptr<EntityBase> parent, int depth, bool remote)
{
    if(!isSubscriber() && advertised)
        Advertise(AdvertisementType::Enum::Change);
    LOG(LogLevel::Info) << "child"<<child->getName()<< " removed from: " <<parent->getName() << " depth: " << depth << std::endl;
    emit childRemoved(child,parent, depth+1,remote);
}

void EntityBase::IterateThroughAllProperties(std::function<void(QMetaProperty)> func)
{
    const QMetaObject* metaObj = this->metaObject();
    while(metaObj != NULL)
    {
        for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i)
        {
            func(metaObj->property(i));
        }
        metaObj = metaObj->superClass();
    }
}

void EntityBase::Advertise(AdvertisementType::Enum type)
{
    if(!advertised && type != AdvertisementType::Enum::New)
        return;
    if(advertised && type == AdvertisementType::Enum::New)
        return;
    if(this->advertisementPublisher != NULL)
    {
        LOG(Debug) << "Advertising:" << getName()<< " Type:" << type << std::endl;

        advertised = true;
        ros2_components_msg::msg::EntityAdvertisement::SharedPtr msg = std::make_shared<ros2_components_msg::msg::EntityAdvertisement>();

        int64_t ipAddr =0;
        foreach(const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
        {
            if(!interface.name().contains("vmnet"))
            {
                foreach (const QHostAddress &address, interface.allAddresses())
                {
                    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                    {
                        //LOG(Debug) << "Ip address is:" << address.toString().toStdString() << std::endl;
                        if(!address.isLoopback())
                        {
                            ipAddr = address.toIPv4Address();
                            break;
                        }
                    }
                }
            }
        }

        msg->nodename = this->parentNode->get_name();
        msg->advertisementtype = (int)type;
        msg->id = getId();
        msg->machineip = ipAddr;
        if(getParent() != NULL)
        {
            msg->parent = getParent()->getId();
            msg->parenttype = getParent()->getClassName();
        }
        else
        {
            msg->parent = -1;
            msg->parenttype = "";
        }
        msg->type = this->className;
        builtin_interfaces::msg::Time time;
        simpleLogger::set_now(time);
        msg->stamp = time;


        for(auto & child: childs)
        {
            msg->childtypes.push_back(child->getClassName());
            msg->childids.push_back(child->getId());

        }


        msg->componentname = getName();
        LOG(Debug) << "Publishing advertisementmessage in " << getName() << " now" << std::endl;
        advertisementPublisher->publish(msg);

    }
}








}
