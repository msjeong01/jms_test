#include "ros/ros.h"
#include "std_msgs/Int16.h"

int main(int argc, char **argv) {
  ros::init(argc, argv, "publisher");
  ros::NodeHandle nh;

  ros::Publisher pub_ = nh.advertise<std_msgs::Int16>("message", 1);
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()) {

    std_msgs::Int16 msg;
    msg.data = count;

    ROS_INFO("Publishing msg: %d", msg.data);
    pub_.publish(msg);
    ++count;
    loop_rate.sleep();
  }

  return 0;
}



// #include "ros/ros.h"
// #include "std_msgs/Int16.h"

// class Publisher {
// public:
//   Publisher() {
//     ros::NodeHandle nh;
//     pub_ = nh.advertise<std_msgs::Int16>("message", 1);
//     count = 0;
//   }

//   void publishMsg() {
//       std_msgs::Int16 msg;
//       msg.data = count;
//       ros::Rate loop_rate(10);
      
//       ROS_INFO("Publishing msg: %d", msg.data);
//       pub_.publish(msg);
//       ++count;
//       loop_rate.sleep();
//   }

// private:
//   ros::Publisher pub_;
//   int count;
// };

// int main(int argc, char **argv) {
//   ros::init(argc, argv, "publisher");
//   Publisher node;

  
//   while (ros::ok()) {
//         node.publishMsg();
//   }

//   return 0;
// }