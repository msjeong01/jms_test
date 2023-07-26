class CalculatorServer {
public:
    CalculatorServer() {
        nh_ = ros::NodeHandle("~");
        trigger_status = false;
        trigger_ = nh_.advertiseService("trigger_check", &CalculatorServer::trigger_check_callback, this);
        calculate_ = nh_.advertiseService("calculate", &CalculatorServer::calculator_callback, this);
        ROS_INFO("Ready to calculate");
    }

    bool trigger_check_callback(std_srvs::Trigger::Request& req, std_srvs::Trigger::Response& res);
    bool add(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res);
    bool subtract(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res);
    bool divide(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res);
    bool multiply(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res);
    bool calculator_callback(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res);

private:
    ros::NodeHandle nh_;
    ros::ServiceServer trigger_;
    ros::ServiceServer calculate_;
    bool trigger_status;
};