#ifndef NODECONTAINER_H
#define NODECONTAINER_H


#include "rclcpp/rclcpp.hpp"
#include "ros2_components_exceptions.h"
namespace  ros2_components {


/**
 * @brief The NodeContainer class encapsules a ros node. It provides status information and sync/async methods for spinning the node.
 */
class NodeContainer
{
public:
    typedef std::shared_ptr<NodeContainer> SharedPtr;
    NodeContainer(rclcpp::node::Node::SharedPtr _ros_node, int64_t _node_id, std::string _name);

    virtual ~NodeContainer();
    /**
     * @brief Spin the node synchronous.
     * @param timeout - in case timeout is != -1 it will add the node to an singlethreaded executor and call spin once on the node with the given timeout. Otherwise spin_some is called.
     */
    void Spin(std::chrono::nanoseconds timeout = std::chrono::nanoseconds(-1));
    /**
     * @brief SpinAsync - Spins the node in a second thread by calling spin_some on the node with the given loop rate
     */
    void SpinAsync();
    /**
     * @brief GetRosNode
     * @return  The internal rosnode
     */
    rclcpp::node::Node::SharedPtr GetRosNode();
    /**
     * @brief GetIsSpinning
     * @return True in case the node is spinning at the moment (Is also set to true in case of async spin)
     */
    bool GetIsSpinning() const;
    /**
     * @brief GetIsSpinningAsync
     * @return True in case the node is spinning async.
     */
    bool GetIsSpinningAsync() const;
    /**
     * @brief GetLoopRate
     * @return  The loop rate for async spinning
     */
    int GetLoopRate() const;
    /**
     * @brief SetLoopRate
     * @param value The loop rate for async spinning
     */
    void SetLoopRate(int value);

    /**
     * @brief GetNodeName
     * @return - the full name of the node. The full name consists of the base name and the id
     */
    std::string GetNodeName() const;
    /**
     * @brief GetNodeNameBase
     * @return - only the base name
     * Example: MyNode110 would be the full name. The base name would be MyNode
     */
    std::string GetNodeNameBase() const;
    /**
     * @brief GetNodeId
     * @return
     */
    int64_t GetNodeId() const;

    /**
     * @brief Ok
     * @return False in case node was destroyed or rclcpp::ok returns false
     */
    bool Ok();

private:
    rclcpp::node::Node::SharedPtr ros_node;
    rclcpp::executors::SingleThreadedExecutor executor;

    bool isSpinning = false;
    bool isSpinningAsync = false;
    bool abort = false;
    int loop_rate  = 40;

    int64_t node_id;
    std::string node_name;

    std::shared_ptr<std::thread> spinThread;
};

}
#endif // NODECONTAINER_H
