#pragma once

#ifndef MODE_CHANGE_SERVER_H
#define MODE_CHANGE_SERVER_H

#include <string>
#include <ros/ros.h>

#include <biibiii_modechange/modeChange.h>

class ModeChangeServer {

public:
    ModeChangeServer() {
        nh_ = ros::NodeHandle("~");
        switchModeService_ = nh_.advertiseService("mode_change", &ModeChangeServer::modeChange_callback, this);
        currentMode_ = "Navigation";
    }
    bool modeChange_callback(biibiii_modechange::modeChange::Request& req, biibiii_modechange::modeChange::Response& res);

private:
    ros::NodeHandle nh_;
    ros::ServiceServer switchModeService_;
    std::string currentMode_;
};

#endif
