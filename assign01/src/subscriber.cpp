// #include "ros/ros.h"
// #include "std_msgs/Int16.h"

// void messageCallback(const std_msgs::Int16::ConstPtr& msg) {
//   ROS_INFO("Received: %d", msg->data);
// }

// int main(int argc, char **argv) {
//   ros::init(argc, argv, "subscriber");
//   ros::NodeHandle nh;

//   ros::Subscriber sub_ = nh.subscribe("message", 1, messageCallback);
//   ros::Rate loop_rate(1);

//   while (ros::ok()) {
//     ros::spinOnce();
//     loop_rate.sleep();
//   }

//   return 0;
// }

// #include "ros/ros.h"
// #include "std_msgs/Int16.h"

// void messageCallback(const std_msgs::Int16::ConstPtr& msg) {
//   ros::Rate loop_rate(1);
//   ROS_INFO("Received: %d", msg->data);
//   loop_rate.sleep(); //콜백함수가가 1초에 한 번씩 호출
// }

// int main(int argc, char **argv) {
//   ros::init(argc, argv, "subscriber");
//   ros::NodeHandle nh;

//   ros::Subscriber sub_ = nh.subscribe("message", 1, messageCallback);

//   ros::spin(); //콜백함수 계속 호출

//   return 0;
// }

#include "ros/ros.h"
#include "std_msgs/Int16.h"

class Subscriber {
public:
  Subscriber() {
    ros::NodeHandle nh;
    sub_ = nh.subscribe("message", 1, &Subscriber::messageCallback, this);
  }

  void messageCallback(const std_msgs::Int16::ConstPtr& msg) {
    ros::Rate loop_rate(1);
    ROS_INFO("Received: %d", msg->data);
    loop_rate.sleep();
  }

private:
  ros::Subscriber sub_;
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "subscriber");
  Subscriber node;

  ros::spin();

  return 0;
}

