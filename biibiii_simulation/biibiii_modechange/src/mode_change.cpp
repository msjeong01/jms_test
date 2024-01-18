#include "mode_change.h"

bool ModeChangeServer::modeChange_callback(biibiii_modechange::modeChange::Request& req,
                                           biibiii_modechange::modeChange::Response& res)
{
    if(req.mode == "SLAM") {
        ROS_INFO("Switch to SLAM mode");

        res.success = true;
        res.message = "Start SLAM mode";

    } else if(req.mode == "Navigation") {
        ROS_INFO("Switch to Navigation mode");

        res.success = true;
        res.message = "Start Navigation mode";
    } else {
        ROS_ERROR("Unknown mode");

        res.success = false;
        res.message = "Unknown mode";
    }
    return true;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "mode_change_server");
    ModeChangeServer modeChangeServer;

    ros::spin();
    ros::shutdown();

    return 0;
}
