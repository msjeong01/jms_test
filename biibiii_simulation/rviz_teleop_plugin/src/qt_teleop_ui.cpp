#include "qt_teleop_ui.h"

namespace rviz_teleop_plugin
{

QtTeleopPanel::QtTeleopPanel(QWidget* parent)
    : rviz::Panel(parent)
{
    //main layout
    QVBoxLayout* mainlayout = new QVBoxLayout;

    //state groupbox
    QGroupBox *State_groupBox = new QGroupBox("State");
    QVBoxLayout *state_layout = new QVBoxLayout(State_groupBox);

    //bms
    QHBoxLayout *bms_state_layout = new QHBoxLayout;
    BMS_state_label = new QLabel("BMS");
    QProgressBar *BMS_progressBar = new QProgressBar;
    BMS_progressBar->setRange(0, 100);

    //pose
    QHBoxLayout* pose_state_layout = new QHBoxLayout;
    Pose_state_tableWidget = new QTableWidget(1, 3);
    //add method of controlling QTableWidget size as fitting as contents of columns

    QStringList pose_headerLabels;
    pose_headerLabels << "x" << "y" << "theta";
    Pose_state_tableWidget->setHorizontalHeaderLabels(pose_headerLabels);

    QStringList verticalLabels;
    verticalLabels << "pose";
    Pose_state_tableWidget->setVerticalHeaderLabels(verticalLabels);

    Pose_state_tableWidget->setItem(0, 0, new QTableWidgetItem("0"));
    Pose_state_tableWidget->setItem(0, 1, new QTableWidgetItem("0"));
    Pose_state_tableWidget->setItem(0, 2, new QTableWidgetItem("0"));

    //navigation groupbox
    Navigation_groupBox = new QGroupBox("Navigation");
    QVBoxLayout *navigation_layout = new QVBoxLayout(Navigation_groupBox);

    QHBoxLayout *nav_button_layout = new QHBoxLayout(Navigation_groupBox);
    move_button = new QPushButton("Move");
    pause_button = new QPushButton("Pause");
    delete_button = new QPushButton("Delete");
    save_button = new QPushButton("Save");
    load_button = new QPushButton("Load");

    // QHBoxLayout* marker_list_layout = new QHBoxLayout;
    // Marker_list_tableWidget = new QTableWidget(1, 3);
    // QStringList nav_headerLabels;
    // nav_headerLabels << "" << "X." << "Y" ;
    // Marker_list_tableWidget->setHorizontalHeaderLabels(nav_headerLabels);
    // QCheckBox* marker_checkBox = new QCheckBox;
    // Marker_list_tableWidget->setCellWidget(0, 0, marker_checkBox);
    //add QCheckBoxlist = []
   
    QHBoxLayout* marker_list_layout = new QHBoxLayout;
    marker_list_comboBox = new QComboBox;


    //teleop groupbox
    QGroupBox *Teleop_groupBox = new QGroupBox("Teleop");
    QVBoxLayout *teleop_layout = new QVBoxLayout(Teleop_groupBox);

    QHBoxLayout* velocity_layout = new QHBoxLayout;
    Linear_label = new QLabel("Linear vel");
    Linear_doubleSpinBox = new QDoubleSpinBox;
    Linear_doubleSpinBox->setMaximum(1.0);
    Linear_doubleSpinBox->setSingleStep(0.01);
    Linear_doubleSpinBox->setValue(0.3);

    Angular_label = new QLabel("Angular vel");
    Angular_doubleSpinBox = new QDoubleSpinBox;
    Angular_doubleSpinBox->setMaximum(1.0);
    Angular_doubleSpinBox->setSingleStep(0.01);
    Angular_doubleSpinBox->setValue(0.3);

    QGridLayout* teleop_panel_layout = new QGridLayout;
    Up_Button = new QPushButton("Forward");
    Stop_Button = new QPushButton("Stop");
    Backward_Button = new QPushButton("Backward");
    Left_Button = new QPushButton("Turn Left");
    Right_Button = new QPushButton("Turn Right");

    //add state widget
    bms_state_layout->addWidget(BMS_state_label);
    bms_state_layout->addWidget(BMS_progressBar);

    pose_state_layout->addWidget(Pose_state_tableWidget);

    //add Navigation widget
    nav_button_layout->addWidget(move_button);
    nav_button_layout->addWidget(pause_button);
    nav_button_layout->addWidget(delete_button);
    nav_button_layout->addWidget(save_button);
    nav_button_layout->addWidget(load_button);

    // marker_list_layout->addWidget(Marker_list_tableWidget);
    marker_list_layout->addWidget(marker_list_comboBox);

    //add teleop widget
    teleop_layout->addWidget(Teleop_groupBox);

    velocity_layout->addWidget(Linear_label);
    velocity_layout->addWidget(Linear_doubleSpinBox);
    velocity_layout->addWidget(Angular_label);
    velocity_layout->addWidget(Angular_doubleSpinBox);

    teleop_panel_layout->addWidget(Up_Button, 0, 1);
    teleop_panel_layout->addWidget(Backward_Button, 2, 1);
    teleop_panel_layout->addWidget(Left_Button, 1, 0);
    teleop_panel_layout->addWidget(Stop_Button, 1, 1);
    teleop_panel_layout->addWidget(Right_Button, 1, 2);

    //set mainlayout
    mainlayout->addWidget(State_groupBox);
    mainlayout->addWidget(Navigation_groupBox);
    mainlayout->addWidget(Teleop_groupBox);

    state_layout->addLayout(bms_state_layout);
    state_layout->addLayout(pose_state_layout);

    navigation_layout->addLayout(nav_button_layout);
    navigation_layout->addLayout(marker_list_layout);

    teleop_layout->addLayout(velocity_layout);
    teleop_layout->addLayout(teleop_panel_layout);

    setLayout(mainlayout);

    //connect function
    // connect(move_button, SIGNAL(clicked(bool)), this, SLOT(on_Move_button_clicked(bool)));
    connect(pause_button, SIGNAL(clicked(bool)), this, SLOT(on_Pause_button_clicked(bool)));
    connect(delete_button, SIGNAL(clicked(bool)), this, SLOT(on_Delete_button_clicked(bool)));
    connect(save_button, SIGNAL(clicked(bool)), this, SLOT(on_Save_button_clicked(bool)));
    connect(load_button, SIGNAL(clicked(bool)), this, SLOT(on_Load_button_clicked(bool)));

    connect(move_button, SIGNAL(clicked(bool)), this, SLOT(moveToMarkerPoint(bool)));
    connect(marker_list_comboBox, SIGNAL(activated(int)), this, SLOT(selectPoseInComboBox(int)));
    // connect(this, SIGNAL(select_index(int)), this, moveToMarkerPoint(bool, int));

    connect(marker_list_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePoseToCombobox(int)));

    connect(Linear_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_Linear_doubleSpinBox_valueChanged(double)));
    connect(Angular_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_Angular_doubleSpinBox_valueChanged(double)));

    connect(Up_Button, SIGNAL(clicked(bool)), this, SLOT(on_Up_Button_pressed(bool)));
    connect(Backward_Button, SIGNAL(clicked(bool)), this, SLOT(on_Backward_Button_pressed(bool)));
    connect(Left_Button, SIGNAL(clicked(bool)), this, SLOT(on_Left_Button_pressed(bool)));
    connect(Right_Button, SIGNAL(clicked(bool)), this, SLOT(on_Right_Button_pressed(bool)));
    connect(Stop_Button, SIGNAL(clicked(bool)), this, SLOT(on_Stop_Button_pressed(bool)));

    goal_signal = new bool(false);
    theta_to_combo = new double(0.0);
    pointer_index = new int(0);
    // destination_pose = new geometry_msgs::Pose(0);

    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    pose_sub_ = nh_.subscribe("amcl_pose", 10, &QtTeleopPanel::poseCallback, this);

    marker_server_.reset(new interactive_markers::InteractiveMarkerServer("path_marker", "", false));
    // marker_pub_ = nh_.advertise<visualization_msgs::Marker>("Point_marker", 1);
    initMenu();
    initMarker();

    // odom_sub_ = nh_.subscribe("odom", 10, &QtTeleopPanel::odomCallback, this);
    pose_index = 0;
    status_sub_ = nh_.subscribe("/move_base/status", 10, &QtTeleopPanel::goalStatusCallback, this);
    // client_ goal_client_("move_base", true);
    // goal_client_.sendGoalAndWait(boost::bind(&QtTeleopPanel::goalStatusCallback, _1));

    // goal_client_ = nh_.serviceClient<move_base_msgs::MoveBaseAction>("move_base");
    // recorded_poses = [];
    marker_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("my_marker", 1);

}

void QtTeleopPanel::on_Current_BMS_state(int battery_)
{

}

void QtTeleopPanel::updateBMSstate(int battery_)
{
}

void QtTeleopPanel::poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
    double x_ = msg->pose.pose.position.x;
    double y_ = msg->pose.pose.position.y;

    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
    double roll, pitch, theta_;
    tf::Matrix3x3(quat).getRPY(roll, pitch, theta_);

    Pose_state_tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(x_)));
    Pose_state_tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(y_)));
    Pose_state_tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(theta_)));

    current_pose = msg->pose.pose;

    if(pose_index == 0) {
        initial_pose = current_pose;
        recorded_poses.push_back(initial_pose);

        *theta_to_combo = theta_;
        updatePoseToCombobox(pose_index, initial_pose);
        qDebug() << pose_index << "initial pose:" << initial_pose.position.x << "" << initial_pose.position.y << "" << theta_;
        pose_index++;
    }else if(*goal_signal == true){
        final_pose = current_pose;
        recorded_poses.push_back(final_pose);

        *theta_to_combo = theta_;
        updatePoseToCombobox(pose_index, final_pose);
        qDebug() << pose_index << "final pose:" << final_pose.position.x << "" << theta_;
    }else { //*goal_signal == false
        double distance = calculateRobotMovingDistance(save_pose, current_pose);

        if(distance >= 1.0){
        qDebug() << "distance is 1.0!";
        save_pose = current_pose;
        recorded_poses.push_back(save_pose);

        *theta_to_combo = theta_;
        updatePoseToCombobox(pose_index, save_pose);
        qDebug() << pose_index << "save pose:" << save_pose.position.x << "" << save_pose.position.y << "" << theta_;
        pose_index++;
        *pointer_index = pose_index;
        }

    }
    // for (const auto& pose : recorded_poses) {
    //     qDebug() << "recorded pose: x=" << pose.position.x << " y=" << pose.position.y;
    // }
    visualization_msgs::MarkerArray marker_array;
    int id = 0;
    for(const auto& pose : recorded_poses){
        visualization_msgs::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time();
        marker.ns = "recorded_poses";
        marker.id = id++;
        marker.type = visualization_msgs::Marker::SPHERE;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose = pose;
        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        marker_array.markers.push_back(marker);
    }
    marker_pub_.publish(marker_array);
}

    // move_base_msgs::MoveBaseActionResult result = goal_client_.getState();
    // actionlib_msgs::GoalStatus
    // actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goal_client_("move_base", true);
    // actionlib::SimpleClientGoalState goal_state = goal_client_.getState();
// void QtTeleopPanel::odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
// {
//     double distance = calculateRobotMovingDistance(save_pose, current_pose);

//     const actionlib_msgs::GoalStatusArray::ConstPtr& status_array;
//     actionlib_msgs::GoalStatus goal_status = status_array->status_list[0];

//     if(goal_status.status == actionlib_msgs::GoalStatus::SUCCEEDED){
//         qDebug() << "reached the goal";
//         if(distance<=1.0){
//             final_pose = current_pose;
//             recorded_poses.push_back(final_pose);

//             updatePoseToCombobox(pose_index, final_pose);
//             qDebug() << pose_index << "final pose:" << final_pose.position.x << "" << final_pose.position.y << "" << final_pose.orientation.y;
//         }
//     } else if(distance >= 1.0){
//         qDebug() << "distance is 1.0!";
//         save_pose = current_pose;
//         recorded_poses.push_back(save_pose);

//         updatePoseToCombobox(pose_index, save_pose);
//         qDebug() << pose_index << "save pose:" << save_pose.position.x << "" << save_pose.position.y << "" << save_pose.orientation.w;
//         pose_index++;
//     }
// }

// void QtTeleopPanel::goalStatusCallback(const actionlib::SimpleClientGoalState& state)
    // if(tc_->isGoalReached()){
    //     as_->setSucceeded(move_base_msgs::MoveBaseResult(), "Goal reached.");
    //     qDebug() << move_base_msgs::MoveBaseResult();
    // }
    // if(goal_status.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
void QtTeleopPanel::goalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& status_array)
{
    static bool print_goal = false;
    if(!status_array->status_list.empty()){
        actionlib_msgs::GoalStatus goal_status = status_array->status_list.back();
        if(goal_status.status == actionlib_msgs::GoalStatus::SUCCEEDED){
            if(!print_goal){
                qDebug() << "goal!!!!!!!!!!";
                *goal_signal = true;
                print_goal = true;
            }
        }
    }
}

double QtTeleopPanel::calculateRobotMovingDistance(const geometry_msgs::Pose& old_pose, const geometry_msgs::Pose& update_pose)
{
    double dx = old_pose.position.x - update_pose.position.x;
    double dy = old_pose.position.y - update_pose.position.y;
    double result = std::sqrt(dx*dx + dy*dy);
    return result;
}


void QtTeleopPanel::updatePoseToCombobox(int index, const geometry_msgs::Pose& pose)
{
    if(index>=0 && index<=recorded_poses.size()){
        // const geometry_msgs::Pose& pose = recorded_poses[index];
        QString list = QString("[%1] x: %2, y: %3, theta: %4")
                            .arg(index)
                            .arg(pose.position.x)
                            .arg(pose.position.y)
                            .arg(*theta_to_combo);
        marker_list_comboBox->addItem(list);
    }
}

void QtTeleopPanel::selectPoseInComboBox(int index)
{
    destination_pose = recorded_poses[index];
    qDebug() << "select index: " << index;
}

void QtTeleopPanel::moveToMarkerPoint(bool checked)
{
    qDebug() << "checked: " << checked;
    qDebug() << "move button checked!";

    client_ goal_client_("move_base", true);
    goal_client_.waitForServer();

    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = destination_pose.position.x;
    goal.target_pose.pose.position.y = destination_pose.position.y;
    goal.target_pose.pose.orientation.w = 1.0;

    goal_client_.sendGoal(goal);
    if(goal_client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
        qDebug() << "send goal success!!!";
    }
}

//navigation
void QtTeleopPanel::initMarker()
{
    visualization_msgs::InteractiveMarker point_marker;
    point_marker.header.frame_id = "map";
    point_marker.name = "point_0";
    point_marker.description = "0st point marker";

    visualization_msgs::InteractiveMarkerControl control;
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::BUTTON;
    control.always_visible = true;

    visualization_msgs::Marker marker_visual;
    marker_visual.type = visualization_msgs::Marker::SPHERE;
    marker_visual.scale.x = 0.05;
    marker_visual.scale.y = 0.05;
    marker_visual.scale.z = 0.05;

    marker_visual.color.r = 1.0;
    marker_visual.color.g = 1.0;
    marker_visual.color.b = 0.0;
    marker_visual.color.a = 1.0;

    control.markers.push_back(marker_visual);
    point_marker.controls.push_back(control);

    marker_server_->insert(point_marker, boost::bind(&QtTeleopPanel::selectMarkerCallback, this, _1));
    menu_handler_.apply(*marker_server_, point_marker.name);

    marker_server_->applyChanges();
}

void QtTeleopPanel::selectMarkerCallback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback)
{
    if(feedback->event_type == visualization_msgs::InteractiveMarkerFeedback::BUTTON_CLICK){
        if(feedback->marker_name == "point_0"){
            //
        } else {
        }
    }
}


// void QtTeleopPanel::receiveMarkerInfo(const MarkerInfo& markerInfo)
// {
// }

void QtTeleopPanel::initMenu()
{
    cancel_entry = menu_handler_.insert("CANCEL", boost::bind(&QtTeleopPanel::menuCallback, this, _1));

    menu_handler_.setCheckState(cancel_entry, interactive_markers::MenuHandler::NO_CHECKBOX);
}

void QtTeleopPanel::menuCallback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback)
{
    interactive_markers::MenuHandler::EntryHandle handle = feedback->menu_entry_id;
    interactive_markers::MenuHandler::CheckState state;
    menu_handler_.getCheckState(handle, state);

    // if (feedback->event_type == interactive_markers::MenuHandler::NO_CHECKED){
    if(state == interactive_markers::MenuHandler::NO_CHECKBOX){
        if (feedback->menu_entry_id == cancel_entry) {
            qDebug() << "CANCEL Entry pressed";
        }
    }
    menu_handler_.reApply(*marker_server_);
    marker_server_->applyChanges();
}

// void QtTeleopPanel::updateMarkerList()
// {
//     Marker_list_tableWidget->setRowCount(list_origin.size());

//     for (int i = 0; i < list_origin.size(); ++i) {
//         QTableWidgetItem* nameItem = new QTableWidgetItem(list_origin[i].name);
//         QTableWidgetItem* xItem = new QTableWidgetItem(QString::number(list_origin[i].x));
//         QTableWidgetItem* yItem = new QTableWidgetItem(QString::number(list_origin[i].y));

//         Marker_list_tableWidget->setItem(i, 0, nameItem);
//         Marker_list_tableWidget->setItem(i, 1, xItem);
//         Marker_list_tableWidget->setItem(i, 2, yItem);
//     }
// }

void QtTeleopPanel::updateMarkerColors()
{
}

void QtTeleopPanel::createPathLines()
{
}

void QtTeleopPanel::on_Move_button_clicked(bool checked)
{
    // moveToPath();
    qDebug() << "move button cliked";
}


void QtTeleopPanel::moveToPath()
{
//     goal_client_ goal_client_("move_base", true);
//     goal_client_.waitForServer();

//     move_base_msgs::MoveBaseGoal goal;

//     goal.target_pose.header.frame_id = "map";
//     goal.target_pose.header.stamp = ros::Time::now();

//     goal.target_pose.pose.position.x = point_0.x;
//     goal.target_pose.pose.position.y = point_0.y;

//     goal.target_pose.pose.orientation.w = 1.0;

//     ROS_INFO("Sending goal");
//     goal_client_.sendGoal(goal);


//     if (goal_client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
//         ROS_INFO("Robot reached the goal");
//     } else {
//         ROS_ERROR("Failed to reach the goal");
//     }
}

void QtTeleopPanel::moveToNextMarker()
{
}

void QtTeleopPanel::compareMarkerCoordinate()
{
    
}

void QtTeleopPanel::visualizingRobotPath()
{
    // visualization_msgs::Marker path_marker;
    // path_marker.header.frame_id = "base_footprint";
    // path_marker.ns = "robot_path";
    // path_marker.id = 0;
    // path_marker.type = visualization_msgs::Marker::LINE_STRIP;
    // path_marker.action = visualization_msgs::Marker::ADD;
    // path_marker.pose.orientation.w = 1.0;
    // path_marker.scale.x = 0.02;

    // for (const geometry_msgs::Point& pose : selectedPoses) {
    //     path_marker.points.push_back(pose);
    // }

    // path_marker.color.a = 1.0;
    // path_marker.color.r = 0.0;
    // path_marker.color.g = 0.0;
    // path_marker.color.b = 1.0;

    // visualization_msgs::MarkerArray marker_array;
    // marker_array.markers.push_back(path_marker);
    // marker_pub_.publish(marker_array);
}

void QtTeleopPanel::on_Pause_button_clicked(bool checked)
{
    stopToMove();
    qDebug() << "pause button cliked";
}

void QtTeleopPanel::stopToMove()
{
}

void QtTeleopPanel::on_Delete_button_clicked(bool checked)
{
    deleteRobotPath();
    qDebug() << "delete button cliked";

}

void QtTeleopPanel::deleteRobotPath()
{
}

void QtTeleopPanel::deleteMarkers()
{
}

void QtTeleopPanel::on_Save_button_clicked(bool checked)
{
    saveMarkerInformation();
    qDebug() << "save button cliked";
}

void QtTeleopPanel::saveMarkerInformation()
{
}

void QtTeleopPanel::on_Load_button_clicked(bool checked)
{
    loadMarkerInformation();
    qDebug() << "load button cliked";
}

void QtTeleopPanel::loadMarkerInformation()
{
}

//teleop
void QtTeleopPanel::on_Linear_doubleSpinBox_valueChanged(double arg1)
{
}

void QtTeleopPanel::on_Angular_doubleSpinBox_valueChanged(double arg1)
{
}

void QtTeleopPanel::on_Up_Button_pressed(bool checked)
{
    qDebug() << "Up button pressed";
    linear_vel_ = Linear_doubleSpinBox->value();
    publishVelocity(linear_vel_, angular_vel_);
}

void QtTeleopPanel::on_Backward_Button_pressed(bool checked)
{
    qDebug() << "Down button pressed";
    linear_vel_ = -Linear_doubleSpinBox->value();
    publishVelocity(linear_vel_, -angular_vel_);
}

void QtTeleopPanel::on_Left_Button_pressed(bool checked)
{
    qDebug() << "Left button pressed";
    angular_vel_ = Angular_doubleSpinBox->value();
    publishVelocity(linear_vel_, angular_vel_);
}

void QtTeleopPanel::on_Right_Button_pressed(bool checked)
{
    qDebug() << "Right button pressed";
    angular_vel_ = -Angular_doubleSpinBox->value();
    publishVelocity(linear_vel_, angular_vel_);
}

void QtTeleopPanel::on_Stop_Button_pressed(bool checked)
{
    qDebug() << "Stop button pressed";
    linear_vel_ = 0.0;
    angular_vel_ = 0.0;
    publishVelocity(linear_vel_, angular_vel_);
}

void QtTeleopPanel::publishVelocity(float linear_vel_, float angular_vel_)
{
    cmd_vel.linear.x = linear_vel_;
    cmd_vel.linear.y = 0.0;
    cmd_vel.linear.z = 0.0;

    cmd_vel.angular.x = 0.0;
    cmd_vel.angular.y = 0.0;
    cmd_vel.angular.z = angular_vel_;

    cmd_vel_pub_.publish(cmd_vel);
    ROS_INFO("Linear vel: %.2f, Angular vel: %.2f", linear_vel_, angular_vel_);
}




} // end namespace

PLUGINLIB_EXPORT_CLASS(rviz_teleop_plugin::QtTeleopPanel, rviz::Panel)