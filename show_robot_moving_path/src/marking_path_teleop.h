/********************************************************************************
** Created by: Qt User Interface Compiler version 5.12.8
********************************************************************************/
#pragma once

#ifndef MARKING_PATH_TELEOP_H
#define MARKING_PATH_TELEOP_H

#ifndef Q_MOC_RUN

#include <QObject>
#include <QEvent>
#include <QString>
#include <QMouseEvent>
#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include <QProgressBar>
#include <QWidget>
#include <mutex>

#include <QtCore/QVariant>
#include <QtCore/QRect>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QStandardItemEditorCreator>
#include <QMetaObject>

#include <QtWidgets/QWidget>

#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>

#include <actionlib/client/simple_action_client.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <actionlib_msgs/GoalStatus.h>

#include <boost/shared_ptr.hpp>

#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>

#include <math.h>
#include <memory>

#include <move_base_msgs/MoveBaseGoal.h>
#include <move_base_msgs/MoveBaseAction.h>

#include <ros/ros.h>
#include <rviz/panel.h>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <vector>

#include <pluginlib/class_list_macros.h>

#endif


namespace show_robot_moving_path
{

class MarkingPathAndTeleop : public rviz::Panel
{
Q_OBJECT

public:
    MarkingPathAndTeleop(QWidget* parent = 0);

public Q_SLOTS:

private Q_SLOTS:
    void update_BMS_state();

    void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);
    double calculate_theta(const geometry_msgs::Quaternion &orientation);

    double calculate_Moving_Distance(const geometry_msgs::Pose& old_pose, const geometry_msgs::Pose& update_pose);
    void update_pose_to_comboBox();
    void update_final_pose();
    void write_pose_in_comboBox(int index, const geometry_msgs::Pose& pose);
    void visual_point_by_marker();

    void goalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& status_array);

    void index_in_comboBox_selected(int index);
    void change_selected_indexMarker_color(int index);
    void move_To_MarkerPoint(bool checked);
    void delete_index_and_marker(bool checked);

    void linear_doubleSpinBox_valueChanged(double arg1);
    void angular_doubleSpinBox_valueChanged(double arg1);

    void forward_Button_pressed(bool checked);
    void backward_Button_pressed(bool checked);
    void left_Button_pressed(bool checked);
    void right_Button_pressed(bool checked);
    void stop_Button_pressed(bool checked);

    void publish_Velocity(float linear_vel_, float angular_vel_);


private:
    QWidget *centralwidget;

    //state group
    QGroupBox *State_groupBox;

    QTableWidget *Pose_state_tableWidget;

    QLabel *BMS_state_label;
    QProgressBar *BMS_progressBar;

    //navigation group
    QGroupBox *Navigation_groupBox;
    QPushButton *move_button;
    QPushButton *delete_button;

    QComboBox *pose_list_comboBox;

    //teleop group
    QGroupBox *Teleop_groupBox;

    QLabel *Linear_label;
    QLabel *Angular_label;
    QDoubleSpinBox *Linear_doubleSpinBox;
    QDoubleSpinBox *Angular_doubleSpinBox;

    QPushButton *Forward_Button;
    QPushButton *Stop_Button;
    QPushButton *Backward_Button;
    QPushButton *Left_Button;
    QPushButton *Right_Button;

    //other members
    ros::NodeHandle nh_;

    //bms
    int bms_value;

    //teleop
    ros::Publisher cmd_vel_pub_;
    geometry_msgs::Twist cmd_vel;
    float linear_vel_;
    float angular_vel_;

    //pose
    double x_;
    double y_;
    double theta_;

    int pose_index;
    double* update_theta;
    double* theta_to_combo;

    ros::Subscriber pose_sub_;
    tf::Quaternion quat;

    geometry_msgs::Pose current_pose;
    geometry_msgs::Pose initial_pose;
    geometry_msgs::Pose save_pose;
    geometry_msgs::Pose final_pose;
    std::vector<geometry_msgs::Pose> recorded_poses;

    //goal status
    ros::Subscriber status_sub_;
    move_base_msgs::MoveBaseGoal goal;
    // bool* reached_goal_check;
    bool* delete_button_pressed;
    bool* move_button_pressed;
    bool* check_new_goal;
    // std::shared_ptr<actionlib_msgs::GoalStatusArray> status_array;
    int* execute_trigger;
    bool lock_goal_trigger;


    //marker
    int* select_index;
    ros::Publisher marker_pub_;
    visualization_msgs::MarkerArray marker_array;
    visualization_msgs::Marker marker;
    visualization_msgs::Marker delete_marker;
    std::vector<visualization_msgs::Marker> recorded_markers;
    geometry_msgs::Pose destination_pose;
    typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> client_;
};


} // end namespace

#endif // MARKING_PATH_TELEOP_H