//lambda
[&](const ros::TimerEvent&) {
    std_msgs::String msg;
    ss << "chatter messages: " << chatter_count;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
    chatter_pub.publish(msg);
    chatter_count++;
}