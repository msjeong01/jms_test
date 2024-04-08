#include <ros/ros.h>
#include <zmq.hpp>

int main(int argc, char**argv){
    ros::init(argc, argv, 'request_node');
    ros::NodeHandle nh;

    // ros::Time::init();
    // ros::Rate loop_rate(10);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    std::cout << "Connecting to server.. " << std::endl;
    socket.connect("tcp://localhost:6666");

    while(ros::ok()){
        zmq::message_t request;

    }

}