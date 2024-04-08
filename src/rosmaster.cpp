#include <ros/ros.h>
#include <ros/master.h>

int main(int argc, char **argv) {
    // Initialize ROS
    ros::init(argc, argv, "test_node");
    ros::NodeHandle nh;

    // Define the topic name
    std::string topic_name = "/test";

    // Get the topic type
    std::string message_type;
    ros::master::V_TopicInfo topics;
    ros::master::getTopics(topics);
    for (const auto& topic : topics) {
        if (topic.name == topic_name) {
            message_type = topic.datatype;
            std::cout << "type : " << topic.datatype << std::endl;
            break;
        }
    }

    ROS_INFO("message type: %s", message_type.c_str());


    return 0;
}