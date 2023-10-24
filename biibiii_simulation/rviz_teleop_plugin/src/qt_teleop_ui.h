/********************************************************************************
** Created by: Qt User Interface Compiler version 5.12.8
********************************************************************************/
#pragma once

#ifndef QT_TELEOP_UI_H
#define QT_TELEOP_UI_H

#define emit

#ifndef Q_MOC_RUN

#include <QObject>
#include <QEvent>
#include <QString>
#include <QMutex>
#include <QMouseEvent>

#include <QtCore/QVariant>
#include <QtCore/QRect>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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
#include <QMetaObject>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QStandardItemEditorCreator>

#include <QtWidgets/QWidget>

#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>

#include <ros/ros.h>
#include <ros/console.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

#include <std_srvs/Trigger.h>

// #include <move_base/move_base.h>
#include <move_base_msgs/MoveBaseGoal.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <actionlib_msgs/GoalStatus.h>

#include <nav_msgs/Odometry.h>

#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

//interactive marker
#include <interactive_markers/interactive_marker_server.h>
#include <interactive_markers/menu_handler.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <math.h>
#include <visualization_msgs/MarkerArray.h>

#include<boost/shared_ptr.hpp>

#include <rviz/panel.h>
#include <rviz/tool.h>
#include <rviz/tool_manager.h>

#include <rviz/properties/vector_property.h>
#include <rviz/visualization_manager.h>
#include <rviz/viewport_mouse_event.h>
#include <rviz/mesh_loader.h>
#include <rviz/geometry.h>

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>

#include <pluginlib/class_list_macros.h>

#endif


namespace rviz_teleop_plugin
{

enum CurrentMode{
    Navigation = 0,
    SLAM = 2
};

struct MarkerInfo{
    int number;
    double x;
    double y;
    QString name;
};

class QtTeleopPanel : public rviz::Panel
{
Q_OBJECT

public:
    QtTeleopPanel(QWidget* parent = 0);

public Q_SLOTS:
    // virtual int processMouseEvent(rviz::ViewportMouseEvent& event);

private Q_SLOTS:
    void on_Current_BMS_state(int battery_);
    void updateBMSstate(int battery_);

    void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);

    void goalStatusCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& status_array);
    // void odomCallback(const nav_msgs::Odometry::ConstPtr& msg);
    double calculateRobotMovingDistance(const geometry_msgs::Pose& old_pose, const geometry_msgs::Pose& update_pose);
    void updatePoseToCombobox(int index, const geometry_msgs::Pose& pose);
    void moveToMarkerPoint(bool checked);
    void selectPoseInComboBox(int index);

    //marker
    void initMarker();
    void selectMarkerCallback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback);
    // void receiveMarkerInfo(const MarkerInfo& markerInfo);
    void initMenu();
    void menuCallback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback);

    // void updateMarkerList();
    void updateMarkerColors();
    void createPathLines();

    void on_Move_button_clicked(bool checked);
    void moveToPath();
    void moveToNextMarker();
    void compareMarkerCoordinate();
    void visualizingRobotPath();

    void on_Pause_button_clicked(bool checked);
    void stopToMove();

    void on_Delete_button_clicked(bool checked);
    void deleteRobotPath();
    void deleteMarkers();

    void on_Save_button_clicked(bool checked);
    void saveMarkerInformation();

    void on_Load_button_clicked(bool checked);
    void loadMarkerInformation();

    void on_Linear_doubleSpinBox_valueChanged(double arg1);
    void on_Angular_doubleSpinBox_valueChanged(double arg1);

    void on_Up_Button_pressed(bool checked);
    void on_Backward_Button_pressed(bool checked);
    void on_Left_Button_pressed(bool checked);
    void on_Right_Button_pressed(bool checked);
    void on_Stop_Button_pressed(bool checked);

    void publishVelocity(float linear_vel_, float angular_vel_);


private:
    //Qt
    QWidget *centralwidget;

    //state group
    QGroupBox *State_groupBox;

    QTableWidget *Pose_state_tableWidget;

    QLabel *BMS_state_label;
    QProgressBar *BMS_progressBar;

    //navigation group
    QGroupBox *Navigation_groupBox;
    QPushButton *move_button;
    QPushButton *pause_button;
    QPushButton *delete_button;
    QPushButton *save_button;
    QPushButton *load_button;

    // QTableWidget *Marker_list_tableWidget;
    QComboBox *marker_list_comboBox;

    QMouseEvent *mouse_right;

    //teleop group
    QGroupBox *Teleop_groupBox;

    QLabel *Linear_label;
    QLabel *Angular_label;
    QDoubleSpinBox *Linear_doubleSpinBox;
    QDoubleSpinBox *Angular_doubleSpinBox;

    QPushButton *Up_Button;
    QPushButton *Stop_Button;
    QPushButton *Backward_Button;
    QPushButton *Left_Button;
    QPushButton *Right_Button;

    //other members
    ros::NodeHandle nh_;
    ros::Publisher cmd_vel_pub_;
    ros::Subscriber pose_sub_;
    ros::Publisher marker_pub_;

    // typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goal_client_;
    geometry_msgs::Twist cmd_vel;
    move_base_msgs::MoveBaseGoal goal;

    tf::Quaternion quat;

    int battery_;

    float linear_vel_;
    float angular_vel_;

    double x_;
    double y_;
    double theta_;
    double* theta_to_combo;

    ros::Subscriber odom_sub_;
    int pose_index;
    geometry_msgs::Pose initial_pose;
    geometry_msgs::Pose current_pose;
    geometry_msgs::Pose save_pose;
    geometry_msgs::Pose final_pose;
    std::vector<geometry_msgs::Pose> recorded_poses;
    int* pointer_index;

    // geometry_msgs::PoseConstPtr arrived_pose;
    geometry_msgs::Pose destination_pose;

    bool* goal_signal;
    typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> client_;
    ros::Subscriber status_sub_;
    // nav_core::BaseLocalPlanner* tc_;
    // move_base::MoveBaseActionServer* as_;

    // ros::ServiceClient goal_client_;
    // move_base_msgs::MoveBaseResult goal_result;

    //interactive
    boost::shared_ptr<interactive_markers::InteractiveMarkerServer> marker_server_;
    interactive_markers::MenuHandler menu_handler_;

    geometry_msgs::Point point_0;
    std::vector<geometry_msgs::Point> selectedPoses;

    interactive_markers::MenuHandler::EntryHandle cancel_entry;

    QList<MarkerInfo> list_origin;
    // list_nav;
    // list_done;
    std::string markerData;

Q_SIGNALS:
    // void markerInfoReceived(const MarkerInfo& markerInfo);
    // void reachGoal_signal(bool i);
    void select_index(int number);

};


} // end namespace

#endif // QT_TELEOP_UI_H