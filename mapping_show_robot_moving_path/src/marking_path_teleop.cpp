#include "marking_path_teleop.h"

namespace mapping_show_robot_moving_path
{

Mapping_MarkingPathAndTeleop::Mapping_MarkingPathAndTeleop(QWidget* parent)
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
    BMS_progressBar = new QProgressBar;
    BMS_progressBar->setRange(0, 100);
    BMS_progressBar->setValue(100);

    //pose
    QHBoxLayout* pose_state_layout = new QHBoxLayout;
    Pose_state_tableWidget = new QTableWidget(1, 3);

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
    QGroupBox *Navigation_groupBox = new QGroupBox("Navigation");
    QVBoxLayout *navigation_layout = new QVBoxLayout(Navigation_groupBox);

    QHBoxLayout *nav_button_layout = new QHBoxLayout;
    move_button = new QPushButton("Move");
    delete_button = new QPushButton("delete");

    QHBoxLayout* pose_list_layout = new QHBoxLayout;
    pose_list_comboBox = new QComboBox;

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
    Forward_Button = new QPushButton("Forward");
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
    nav_button_layout->addWidget(delete_button);
    pose_list_layout->addWidget(pose_list_comboBox);

    //add teleop widget
    velocity_layout->addWidget(Linear_label);
    velocity_layout->addWidget(Linear_doubleSpinBox);
    velocity_layout->addWidget(Angular_label);
    velocity_layout->addWidget(Angular_doubleSpinBox);

    teleop_panel_layout->addWidget(Forward_Button, 0, 1);
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
    navigation_layout->addLayout(pose_list_layout);

    teleop_layout->addLayout(velocity_layout);
    teleop_layout->addLayout(teleop_panel_layout);

    setLayout(mainlayout);

    //connect function
    connect(move_button, SIGNAL(clicked(bool)), this, SLOT(move_To_MarkerPoint(bool)));
    connect(delete_button, SIGNAL(clicked(bool)), this, SLOT(delete_index_and_marker(bool)));
    connect(pose_list_comboBox, SIGNAL(activated(int)), this, SLOT(index_in_comboBox_selected(int)));
    connect(pose_list_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(write_pose_in_comboBox(int)));

    connect(Linear_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(linear_doubleSpinBox_valueChanged(double)));
    connect(Angular_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(angular_doubleSpinBox_valueChanged(double)));

    connect(Forward_Button, SIGNAL(clicked(bool)), this, SLOT(forward_Button_pressed(bool)));
    connect(Backward_Button, SIGNAL(clicked(bool)), this, SLOT(backward_Button_pressed(bool)));
    connect(Left_Button, SIGNAL(clicked(bool)), this, SLOT(left_Button_pressed(bool)));
    connect(Right_Button, SIGNAL(clicked(bool)), this, SLOT(right_Button_pressed(bool)));
    connect(Stop_Button, SIGNAL(clicked(bool)), this, SLOT(stop_Button_pressed(bool)));

    pose_index = 0;
    bms_value = 100;
    update_theta = new double(0.0);
    theta_to_combo = new double(0.0);
    move_button_pressed = new bool(false);
    delete_button_pressed = new bool(false);
    check_new_goal = new bool(true);
    execute_trigger = new int(1.0);
    lock_goal_trigger = true;

    initial_pose.position.x = 0.0;
    initial_pose.position.y = 0.0;
    initial_pose.orientation.w = 1.0;
    distance_from_origin = 0.0;


    // pose_sub_ = nh_.subscribe("amcl_pose", 10, &Mapping_MarkingPathAndTeleop::poseCallback, this);
    // status_sub_ = nh_.subscribe("/move_base/status", 10, &Mapping_MarkingPathAndTeleop::goalStatusCallback, this);
    mapping_sub_ = nh_.subscribe("/mapping_pose", 10, &Mapping_MarkingPathAndTeleop::mappingPoseCallback, this);
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    marker_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("passed_point_marker", 1);
}

void Mapping_MarkingPathAndTeleop::update_BMS_state()
{
    qDebug() << "update bms state!";
    bms_value = BMS_progressBar->value();
    BMS_progressBar->setValue(bms_value - 10);

    if(bms_value <= 30) {
        qDebug() << "bms state is dangerous!";
        BMS_progressBar->setStyleSheet("QProgressBar::chunk { background: red;}");
    }

}

// void Mapping_MarkingPathAndTeleop::poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
// {
//     double x_ = msg->pose.pose.position.x;
//     double y_ = msg->pose.pose.position.y;
//     double theta_ = calculate_theta(msg->pose.pose.orientation);

//     current_pose = msg->pose.pose;
//     *update_theta = theta_;

//     Pose_state_tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(x_)));
//     Pose_state_tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(y_)));
//     Pose_state_tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(theta_)));

//     update_pose_to_comboBox();
// }

// double Mapping_MarkingPathAndTeleop::calculate_theta(const geometry_msgs::Quaternion &orientation)
// {
//     tf::Quaternion quat;
//     tf::quaternionMsgToTF(orientation, quat);
//     double roll, pitch, yaw;
//     tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);

//     return yaw;
// }


void Mapping_MarkingPathAndTeleop::write_pose_in_comboBox(int index, const geometry_msgs::Pose& pose)
{
    if(index>=0 && index<=recorded_poses.size()){
        QString list = QString("[%1] x: %2, y: %3, theta: %4")
                            .arg(index)
                            .arg(pose.position.x)
                            .arg(pose.position.y)
                            .arg(*update_theta);
        pose_list_comboBox->addItem(list);
    }
}

void Mapping_MarkingPathAndTeleop::visual_point_by_marker()
{
    int id = 0;

    for(const auto& pose : recorded_poses){
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

        recorded_markers.push_back(marker);
        marker_array.markers.push_back(marker);
    }
    marker_pub_.publish(marker_array);
}

void Mapping_MarkingPathAndTeleop::goalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& status_array)
{
    bool reached_goal_check = true;

    if(!status_array->status_list.empty()){
        actionlib_msgs::GoalStatus goal_status = status_array->status_list.back();
        if(goal_status.status == actionlib_msgs::GoalStatus::SUCCEEDED){
            if(reached_goal_check && lock_goal_trigger){
                if(*execute_trigger == 2){
                    qDebug() << "robot reached on nav goal point!";
                    update_final_pose();
                    update_BMS_state();
                    *execute_trigger = 1;
                }
            }
            else if(*move_button_pressed) {
                qDebug() << "start goint to send goal point!";
                *move_button_pressed = false;
            }
        }
    }
}

void Mapping_MarkingPathAndTeleop::index_in_comboBox_selected(int index)
{
    destination_pose = recorded_poses[index];
    qDebug() << "select index: " << index;
    change_selected_indexMarker_color(index);
}

void Mapping_MarkingPathAndTeleop::change_selected_indexMarker_color(int index)
{
    qDebug() << "change marker color to red" << " index:" << index;
    for (auto& marker : recorded_markers) {
        marker.action = visualization_msgs::Marker::MODIFY;
        if(marker.id == index){
            marker.color.r = 1.0;
            marker.color.g = 0.0;
            marker.color.b = 0.0;
            marker.color.a = 1.0;
        } else{
            marker.color.r = 1.0;
            marker.color.g = 1.0;
            marker.color.b = 0.0;
            marker.color.a = 1.0;
        }
        marker_array.markers.push_back(marker);
    }
    marker_pub_.publish(marker_array);
}

void Mapping_MarkingPathAndTeleop::move_To_MarkerPoint(bool checked)
{
    qDebug() << "move button clicked!";
    *move_button_pressed = true;

    client_ goal_client_("move_base", true);
    goal_client_.waitForServer();

    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = destination_pose.position.x;
    goal.target_pose.pose.position.y = destination_pose.position.y;
    goal.target_pose.pose.orientation.w = 1.0;

    goal_client_.sendGoal(goal);
}

void Mapping_MarkingPathAndTeleop::delete_index_and_marker(bool checked)
{
    qDebug() << "delete button pressed!";
    *delete_button_pressed = true;

    recorded_poses.clear();
    pose_list_comboBox->clear();

    for(int i=0; i<recorded_markers.size(); i++){
        delete_marker = recorded_markers[i];
        delete_marker.action = visualization_msgs::Marker::DELETE;
        marker_array.markers.push_back(delete_marker);
    }
    marker_pub_.publish(marker_array);
    recorded_markers.clear();

    lock_goal_trigger = true;
    pose_index = 0;
    update_pose_to_comboBox();
}

//teleop
void Mapping_MarkingPathAndTeleop::linear_doubleSpinBox_valueChanged(double arg1)
{
}

void Mapping_MarkingPathAndTeleop::angular_doubleSpinBox_valueChanged(double arg1)
{
}

void Mapping_MarkingPathAndTeleop::forward_Button_pressed(bool checked)
{
    qDebug() << "forward button pressed";
    linear_vel_ = Linear_doubleSpinBox->value();
    publish_Velocity(linear_vel_, angular_vel_);

}

void Mapping_MarkingPathAndTeleop::backward_Button_pressed(bool checked)
{
    qDebug() << "backward button pressed";
    linear_vel_ = -Linear_doubleSpinBox->value();
    publish_Velocity(linear_vel_, -angular_vel_);
}

void Mapping_MarkingPathAndTeleop::left_Button_pressed(bool checked)
{
    qDebug() << "Left button pressed";
    angular_vel_ = Angular_doubleSpinBox->value();
    publish_Velocity(linear_vel_, angular_vel_);

}

void Mapping_MarkingPathAndTeleop::right_Button_pressed(bool checked)
{
    qDebug() << "Right button pressed";
    angular_vel_ = -Angular_doubleSpinBox->value();
    publish_Velocity(linear_vel_, angular_vel_);

}

void Mapping_MarkingPathAndTeleop::stop_Button_pressed(bool checked)
{
    qDebug() << "Stop button pressed";
    linear_vel_ = 0.0;
    angular_vel_ = 0.0;
    publish_Velocity(linear_vel_, angular_vel_);
    lock_goal_trigger = true;
}

void Mapping_MarkingPathAndTeleop::publish_Velocity(float linear_vel_, float angular_vel_)
{
    lock_goal_trigger = false;

    cmd_vel.linear.x = linear_vel_;
    cmd_vel.linear.y = 0.0;
    cmd_vel.linear.z = 0.0;

    cmd_vel.angular.x = 0.0;
    cmd_vel.angular.y = 0.0;
    cmd_vel.angular.z = angular_vel_;

    cmd_vel_pub_.publish(cmd_vel);
    ROS_INFO("Linear vel: %.2f, Angular vel: %.2f", linear_vel_, angular_vel_);
}


void Mapping_MarkingPathAndTeleop::mapping_update()
{
    geometry_msgs::Pose current_pose;

    double dx = current_pose.position.x - initial_pose.position.x;
    double dy = current_pose.position.y - initial_pose.position.y;
    double distance = sqrt(dx * dx + dy * dy);

    double distance_moved = distance_from_origin + distance;

    if (distance_moved >= 1.0) {
        geometry_msgs::Pose pose;

        pose.position.x = current_pose.position.x;
        pose.position.y = current_pose.position.y;
        pose.orientation = initial_pose.orientation;

        recorded_poses.push_back(pose);
        visual_point_by_marker();

        distance_from_origin = 0.0;

        initial_pose = current_pose;
    }
}

void Mapping_MarkingPathAndTeleop::mappingPoseCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    geometry_msgs::Pose pose = *msg;

    double x_ = pose.position.x;
    double y_ = pose.position.y;
    double theta_ = 0.0;

    current_pose = pose;
    *update_theta = theta_;

    Pose_state_tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(x_)));
    Pose_state_tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(y_)));
    Pose_state_tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(theta_)));


    mapping_update();
}



} // end namespace

PLUGINLIB_EXPORT_CLASS(mapping_show_robot_moving_path::Mapping_MarkingPathAndTeleop, rviz::Panel)