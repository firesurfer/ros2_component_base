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


#pragma once

/*std*/
#include <vector>
#include <functional>
#include <iostream>
#include <mutex>
#include <fstream>
#include <string>
#include <memory>

/*logger*/
#include "ros2_simple_logger/Logger.h"


/*ros*/
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/subscription.hpp"

/*ros2_components*/
#include "Reflect.h"
#include "ComponentInfo.h"
#include "ros2_components_exceptions.h"
#include "NodeContainer.h"

/*Qt5*/
#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>


Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
Q_DECLARE_METATYPE(std::string)


using namespace std;
using namespace std::placeholders;

namespace ros2_components
{

class EntityBase : public QObject, public std::enable_shared_from_this<EntityBase>
{
    Q_OBJECT
public:

    typedef std::shared_ptr<EntityBase> SharedPtr;
    EntityBase(int64_t _id, bool _subscribe, NodeContainer::SharedPtr _nodeContainer, std::string _className);
    EntityBase(int64_t _id, bool _subscribe, NodeContainer::SharedPtr _nodeContainer, std::string _className, std::string _componentName);
    virtual ~EntityBase();

    /**
     * @brief getId
     * @return The Component ID
     */
    int64_t getId() const;
    /**
     * @brief getName
     * @return The name of the current component
     * This name is generated from getClassName and getId
     */
    std::string getName() const;
    /**
     * @brief getClassName
     * @return The class name
     */
    std::string getClassName() const;
    /**
     * @brief isVirtual
     * @return In case this is a virtual Entity (A Entity without a direct hardware representation) this will be true
     */
    bool isVirtual() const;
    /**
     * @brief isSubscriber
     * @return Is this a subscriber or a publisher
     */
    bool isSubscriber() const;
    /**
     * @brief getParentNode
     * @return The rosnode associated with this entity
     */
    rclcpp::Node::SharedPtr getParentNode() const;

    /**
     * @brief getNodeContainer
     * @return
     */
    NodeContainer::SharedPtr getNodeContainer() const;

    /**
     * @brief addChild
     * @return adds a child to the childs vector
     */
    virtual void addChild(std::shared_ptr<EntityBase> child, bool remote =false);
    /**
     * @brief removeChild Removes the given child
     * @param child child to remote
     */
    virtual void removeChild(std::shared_ptr<EntityBase> child, bool remote = false);
    /**
     * @brief getChildById throws exeption if id isnt available
     * @param id
     * @return the child with the given id
     */
    virtual std::shared_ptr<EntityBase> getChildById(int64_t id);

    /**
     *  @brief getChildById<T> calls getChildById and casts to the given type T
     */
    template<typename T>
    std::shared_ptr<T> getChildById(int64_t id)
    {
        EntityBase::SharedPtr child = getChildById(id);
        std::shared_ptr<T> casted_child = dynamic_pointer_cast<T>(child);
        if(!casted_child)
            throw EntityCastException();
        return casted_child;
    }
    /**
     * @brief getAllChilds
     * @return all childs of this entity
     */
    virtual std::vector<std::shared_ptr<EntityBase>> getAllChilds(){return childs;}
    /**
     * @brief countChilds
     * @return the number of childs
     */
    virtual uint64_t countChilds();

    /**
     * @brief getParent
     * @return the parent of this entity
     */
    virtual std::shared_ptr<EntityBase> getParent();
    /**
     * @brief publish
     */
    virtual bool publish()=0;
    /**
     * @brief getTopicName
     * @return topic name of the ros2 publisher or subscription
     *
     */
    virtual std::string getTopicName();

    /*
     * Some methods for setting and getting stuff
     * TODO Document them correctly
     */
    virtual bool isActive(){return active;}
    void setActive(bool act){this->active = act;}
    virtual std::string getDescription(){return this->description;}
    virtual void setDescription(std::string des){this->description = des;}

    /**
     * @brief iterateThroughAllProperties
     * @param func
     */
    void iterateThroughAllProperties(std::function<void(Element*)> func);

    /**
     * @brief IterateThroughAllChilds
     * @param func
     * //TODO iterator ?
     */
    void iterateThroughAllChilds(std::function<void(EntityBase::SharedPtr)> func);

    /**
     * @brief IterateThroughAllChildsOfType
     * //TODO real iterator?
     */
    template<typename T>
    void iterateThroughAllChildsOfType(std::function<void(std::shared_ptr<T>)> func)
    {
        auto callbackFunc = [&](EntityBase::SharedPtr item)
        {
            std::shared_ptr<T> ch = dynamic_pointer_cast<T>(item);
            if(ch != NULL)
                func(ch);

        };
        iterateThroughAllChilds(callbackFunc);
    }

    /**
     * @brief CountChildsOfType
     * @return Number of childs of the given type
     */
    template<typename T>
    int countChildsOfType()
    {
        int count = 0;
        for(EntityBase::SharedPtr & ent: this->childs)
        {
            std::shared_ptr<T> ch = dynamic_pointer_cast<T>(ent);
            if(ch != NULL)
                count++;
        }
        return count;
    }

    /**
     * @brief GetChildsOfType
     * @return All childs of the given type T
     * @remark Consider using the Iteration functions
     */
    template<typename T>
    std::vector<T> getChildsOfType()
    {
        std::vector<T> childsOfT;
        for(EntityBase::SharedPtr & ent: this->childs)
        {
            std::shared_ptr<T> ch = dynamic_pointer_cast<T>(ent);
            if(ch != NULL)
                childsOfT.push_back(ch);
        }
        return childsOfT;
    }

    /**
     * @brief makeVirtual sets this entity as virtual
     * @remark call this if it is a rebuild of an entity
     */
    void makeVirtual()
    {
        virtualEntity = true;
    }

protected:
    /**
      * @brief setParent
      * @param par
      * sets the parent of this entity
      */
    virtual void setParent(std::shared_ptr<EntityBase> par);
    /**
     * @brief parent
     * The node -> we need to pass in order to create the publisher, subscription and parameter client
     */
    std::shared_ptr<rclcpp::Node> parentNode;
    /**
     * @brief nodeContainer - wraps up the ros2 node
     */
    NodeContainer::SharedPtr nodeContainer;
    /**
     * Helper method so we can use REFLECT on a variable
     */
    template <class T>
    void addElement(string type,T &data)
    {
        Element* elem = dynamic_cast<Element*>(new SpecificElement<T>(type , data));
        if(elem != nullptr)
            internalmap.push_back(elem);
    }
    /**
     * Vector all elements that had used REFLECT on them are stored in
     */
    std::vector<Element*> internalmap;

    /**
     * Contains children of this entity -> used for building some kind of tree
     */
    std::vector<std::shared_ptr<EntityBase>> childs;
    /**
     * Parent of this entity
     */
    std::weak_ptr<EntityBase> parent;
    /**
     * @brief pubBase
     */
    std::shared_ptr<rclcpp::PublisherBase> pubBase;
    /**
     * @brief subBase
     * Baseclass for the subscription -> used for determing topic name
     */
    std::shared_ptr<rclcpp::SubscriptionBase> subBase;

    /**
     * @brief active
     * is this entity active?
     */
    bool active;
    /**
     * @brief description
     * used defined description
     */
    std::string description;
protected slots:

signals:
    void childAdded(EntityBase::SharedPtr child,std::shared_ptr<EntityBase> parent, bool remote);
    void childRemoved(EntityBase::SharedPtr child,std::shared_ptr<EntityBase> parent, bool remote);
    void entityDeleted(ComponentInfo info);
    void newData(); //TODO SharedPtr ?
    void storageUpdate(std::string key);

private:
    /**
     * @brief id
     * Component ID
     */
    int64_t id;
    /**
     * @brief virtualEntity
     * Is it a real component
     */
    bool virtualEntity;
    /**
     * @brief subscriber
     */
    bool subscriber;
    /**
     * @brief Name of the implementing class
     */
    std::string className;
    /**
     * @brief name of the component
     */
    std::string name;



};



}
