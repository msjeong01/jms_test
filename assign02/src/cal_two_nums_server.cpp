// #include "ros/ros.h"
// #include "std_srvs/Trigger.h"
// #include "assign02/FirstNum.h"
// #include "assign02/SecondNum.h"
// #include "assign02/Operator.h"

// class CalculatorServer {
// public:
//     CalculatorServer() {
//         nh_ = ros::NodeHandle("~");
//         trigger_status = false;
//         trigger_ = nh_.advertiseService("trigger_check", &CalculatorServer::trigger_check_callback, this);
//         calculate_ = nh_.advertiseService("calculate", &CalculatorServer::calculator_callback, this);
//         ROS_INFO("Ready to calculate");
//     }

//     bool trigger_check_callback(std_srvs::Trigger::Request& req, std_srvs::Trigger::Response& res);
//     void add(float& num1, float& num2);
//     void subtract(float& num1, float& num2);
//     void divide(float& num1, float& num2);
//     void multiply(float& num1, float& num2);
//     double calculator_callback(assign02::FirstNum::Request& req_num1, assign02::SecondNum::Request& req_num2,
//                                assign02::Operator::Request& req_oper);
//     //assign02::FirstNum::Request& req_num1, assign02::SecondNum::Request& req_num2
// private:
//     ros::NodeHandle nh_;
//     ros::ServiceServer trigger_;
//     ros::ServiceServer calculate_;
//     bool trigger_status;
//     double result;
// };

// bool CalculatorServer::trigger_check_callback(std_srvs::Trigger::Request& req,
//                                               std_srvs::Trigger::Response& res) {
//     if (!trigger_status) {
//         trigger_status = true;
//         res.success = true;
//         res.message = "Trigger checked. Start calculation.";

//     } else {
//         trigger_status = false;
//         res.success = false;
//         res.message = "Trigger status is false";
//     }
//     return true;
// }

// void CalculatorServer::add(float& num1, float& num2) {
//     result = num1 + num2;
//     ROS_INFO("Sum: %.2f", result);

// }

// void CalculatorServer::subtract(float& num1, float& num2){
//     result = num1 - num2;
//     ROS_INFO("Subtraction: %.2f", result);
// }

// void CalculatorServer::divide(float& num1, float& num2){
//     if (num2 == 0.0) {
//         ROS_INFO("Division by zero is impossible");
//     }

//     result = num1 / num2;
//     ROS_INFO("Division: %.2f", result);
// }

// void CalculatorServer::multiply(float& num1, float& num2) {
//     result = num1 * num2;
//     ROS_INFO("Multiply: %.2f", result);
// }

// double CalculatorServer::calculator_callback(assign02::FirstNum::Request& req1,
//                                              assign02::SecondNum::Request& req2,
//                                              assign02::Operator::Request& req) {
//     ROS_INFO("Trigger checked. Start calculation.");

//     if (req.operator == "+") {add(req1.num1, req2.num2);}
//     else if (req.operator == "-") {subtract(req1.num1, req2.num2);}
//     else if (req.operator == "/") {divide(req1.num1, req2.num2);}
//     else if (req.operator == "*") {multiply(req1.num1, req2.num2);}
//     else { ROS_INFO("Invalid operator"); }
// }


// int main(int argc, char** argv) {
//     ros::init(argc, argv, "server");
//     CalculatorServer calculatorServer;
//     ros::spin();
//     ros::shutdown();

//     return 0;
// }







// #include "ros/ros.h"
// #include "std_srvs/Trigger.h"
// #include "assign02/TwoNums.h"


// bool triggered_ = false;
// bool trigger_check(std_srvs::Trigger::Request&,
//                    std_srvs::Trigger::Response& res) {
//     if (!triggered_) {
//         triggered_ = true;
//         res.success = true;
//         res.message = "Trigger status is True.";
//         return true;
//      } 
//     else {
//         triggered_ = false;
//         res.success = false;
//     }
//         res.message = "Trigger status is false";

//     return true;
// }

// bool add(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res) {
//     res.result = req.num1 + req.num2;
//     ROS_INFO("Sum: %.2f", res.result);
//     return true;
// }

// bool subtract(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res) {
//     res.result = req.num1 - req.num2;
//     ROS_INFO("Subtraction: %.2f", res.result);
//     return true;
// }

// bool divide(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res) {
//     if (req.num2 == 0.0) {
//         ROS_INFO("Division by zero is impossible");
//         return false;
//     }

//     res.result = req.num1 / req.num2;
//     ROS_INFO("Division: %.2f", res.result);
//     return true;
// }

// bool multiply(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res) {
//     res.result = req.num1 * req.num2;
//     ROS_INFO("Multiply: %.2f", res.result);
//     return true;

// }

// bool calculate_two_nums(assign02::TwoNums::Request& req,
//                         assign02::TwoNums::Response& res) {

//     while (!triggered_ && ros::ok()) {
//         ROS_INFO("Trigger is not checked");
//         ros::Duration(0.1).sleep();
//         ros::spinOnce();
//     }

//     // if (!triggered_) {
//     //     ROS_INFO("Trigger status: False");
//     //     return false;
//     // }


//     if(triggered_) {
//         ROS_INFO("Trigger checked. Start calculation.");

//         if (req.oper == "+") {
//             add(req, res);

//         } else if (req.oper == "-") {
//             subtract(req, res);

//         } else if (req.oper == "/") {
//             divide(req, res);

//         } else if (req.oper == "*") {
//             multiply(req, res);
//         } else {
//             ROS_INFO("Invalid operator");
//             return false;
//         }
//     } 
// }


// int main(int argc, char** argv) {
//     ros::init(argc, argv, "server");
//     ros::NodeHandle nh;

//     ros::ServiceServer service_trigger = nh.advertiseService("trigger_check", trigger_check);
//     ros::ServiceServer service_calculate = nh.advertiseService("calculate", calculate_two_nums);
//     ROS_INFO("Ready to calculate");
    
//     ros::spin();
//     ros::shutdown();

//     return 0;
// }



#include "ros/ros.h"
#include "std_srvs/Trigger.h"
#include "assign02/TwoNums.h"
#include "CalculatorServer.h"

bool CalculatorServer::trigger_check_callback(std_srvs::Trigger::Request& req,
                                              std_srvs::Trigger::Response& res) {
    if (!trigger_status) {
        trigger_status = true;
        res.success = true;
        res.message = "Trigger checked. Start calculation.";
    } else {
        trigger_status = false;
        res.success = false;
        res.message = "Trigger status is false";
    }
    return true;
}

bool CalculatorServer::add(assign02::TwoNums::Request& req, assign02::TwoNums::Response& res) {
    res.result = req.num1 + req.num2;
    ROS_INFO("Sum: %.2f", res.result);
    return true;
}

bool CalculatorServer::subtract(assign02::TwoNums::Request& req,
                                assign02::TwoNums::Response& res) {
    res.result = req.num1 - req.num2;
    ROS_INFO("Subtraction: %.2f", res.result);
    return true;
}

bool CalculatorServer::divide(assign02::TwoNums::Request& req,
                              assign02::TwoNums::Response& res) {
    if (req.num2 == 0.0) {
        ROS_INFO("Division by zero is impossible");
        return false;
    }

    res.result = req.num1 / req.num2;
    ROS_INFO("Division: %.2f", res.result);
    return true;
}

bool CalculatorServer::multiply(assign02::TwoNums::Request& req,
                                assign02::TwoNums::Response& res) {
    res.result = req.num1 * req.num2;
    ROS_INFO("Multiply: %.2f", res.result);
    return true;
}

bool CalculatorServer::calculator_callback(assign02::TwoNums::Request& req,
                                           assign02::TwoNums::Response& res) {
    while (!trigger_status && ros::ok()) {
        ROS_INFO("Trigger is not checked");
        ros::Duration(0.1).sleep();
        ros::spinOnce();
    }

    if (trigger_status) {
        ROS_INFO("Trigger checked. Start calculation.");

        if (req.oper == "+") {
            return add(req, res);
        } else if (req.oper == "-") {
            return subtract(req, res);
        } else if (req.oper == "/") {
            return divide(req, res);
        } else if (req.oper == "*") {
            return multiply(req, res);
        } else {
            ROS_INFO("Invalid operator");
            return false;
        }
    }

    return false;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "server");
    CalculatorServer calculatorServer;
    ros::spin();
    ros::shutdown();

    return 0;
}
