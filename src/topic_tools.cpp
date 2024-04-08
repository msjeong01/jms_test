// #include <ros/ros.h>
// #include <topic_tools/shape_shifter.h>
// #include <syscon_msgs/RobotState.h>

// void topicCallback(const topic_tools::ShapeShifter::ConstPtr& msg,
//                    const std::string& topic_name)
// {
//     const std::string& datatype = msg->getDataType();
//     // const std::string& definition = msg->getMessageDefinition();

//     std::vector<uint8_t> buffer;

//     buffer.resize( msg->size() );
//     ros::serialization::OStream stream(buffer.data(), buffer.size());
//     msg->write(stream);

//     syscon_msgs::RobotState robot_state;

//     ros::serialization::IStream istream(buffer.data(), buffer.size());
//     ros::serialization::deserialize(istream, robot_state);

//     std::cout << "robot_state: " << std::endl << robot_state << std::endl;

//     // ROS_INFO("Topic datatype: %s", datatype.c_str());
//     // ROS_INFO("Topic message definition: %s", definition.c_str());
// }

// int main(int argc, char** argv)
// {
//     ros::init(argc, argv, "test_node");
//     ros::NodeHandle nh;

//     const std::string topic_name = "/test";

//     boost::function<void(const topic_tools::ShapeShifter::ConstPtr&)> callback =
//         boost::bind(&topicCallback, _1, topic_name);


//     ros::Subscriber sub = nh.subscribe<topic_tools::ShapeShifter>(topic_name, 1, callback);

//     ros::spin();
//     return 0;
// }

/* output
    RID:
    type:
    pose:
        x: 0
        y: 0
        theta: 0
    radius: 0
    workstate: 1
*/


#include <ros/ros.h>
#include <topic_tools/shape_shifter.h>
#include <ros_type_introspection/ros_introspection.hpp>

void subCallback(const topic_tools::ShapeShifter::ConstPtr& msg,
                 const std::string& topic_name,
                 RosIntrospection::Parser& parser,
                 topic_tools::ShapeShifter& shape_shifter,
                 ros::NodeHandle& nh,
                 ros::Publisher& pub)

{
    const std::string& md5sum = msg->getMD5Sum();
    const std::string& datatype = msg->getDataType();
    const std::string& definition = msg->getMessageDefinition();

    parser.registerMessageDefinition(topic_name, RosIntrospection::ROSType(datatype), definition);

    std::vector<uint8_t> buffer;
    RosIntrospection::FlatMessage   flat_container;
    RosIntrospection::RenamedValues renamed_value;

    buffer.resize( msg->size() );
    ros::serialization::OStream stream(buffer.data(), buffer.size());
    msg->write(stream);

    parser.deserializeIntoFlatContainer( topic_name, RosIntrospection::Span<uint8_t>(buffer), &flat_container, 100);
    parser.applyNameTransform(topic_name, flat_container, &renamed_value );

    for (auto it: flat_container.name)
    {
        const std::string& key    = it.first.toStdString();
        const std::string& value  = it.second;
        printf(" %s = %s\n", key.c_str(), value.c_str() );
    }
    for (auto it: renamed_value)
    {
        const std::string& key = it.first;
        const RosIntrospection::Variant& value   = it.second;
        printf(" %s = %f\n", key.c_str(), value.convert<double>() );
    }

    const std::string new_topic_name = "/new_test";
    shape_shifter.morph(md5sum, datatype, definition, "");
    pub = shape_shifter.advertise(nh, new_topic_name, 1);
    // ros::Publisher pub = nh.advertise<topic_tools::ShapeShifter>(new_topic_name, 1);
    pub.publish(shape_shifter);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "test_node");
    ros::NodeHandle nh;
    ros::Publisher pub;

    const std::string topic_name = "/test";

    RosIntrospection::Parser parser;
    topic_tools::ShapeShifter shape_shifter;


    boost::function<void(const topic_tools::ShapeShifter::ConstPtr&)> subcallback =
        boost::bind(&subCallback, _1, topic_name, boost::ref(parser), boost::ref(shape_shifter), boost::ref(nh), boost::ref(pub));

    ros::Subscriber sub = nh.subscribe<topic_tools::ShapeShifter>(topic_name, 1, subcallback);

    ros::spin();
    return 0;
}

/* output
    /test/RID =
    /test/type =
    /test/pose/x = 0.000000
    /test/pose/y = 0.000000
    /test/pose/theta = 0.000000
    /test/radius = 0.000000
    /test/workstate = 1.000000

$ rostopic list
    /new_test
    /rosout
    /rosout_agg
    /test
    syscon@syscon0116 ~/catkin_ws$ rostopic info /new_test
    Type: syscon_msgs/RobotState

    Publishers:
    * /test_node (http://localhost:41327/)

    Subscribers: None
*/