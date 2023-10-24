namespace rviz_teleop_plugin
{
class QtTeleopPanel;

/// Tool
PathMarkerTool::PathMarkerTool()
    : rviz::Tool()
{
    moving_path_marker_node_ = NULL;
    current_marker_property_ = NULL;
    // connect(, SIGNAL(markerInfoReceived(const MarkerInfo&)), this, SLOT(receiveMarkerInfo(const MarkerInfo&)));

}

PathMarkerTool::~PathMarkerTool()
{
    for(unsigned i=0; i < path_marker_nodes_.size(); i++)
    {
        scene_manager_->destroySceneNode(path_marker_nodes_[i]);
    }
}

void PathMarkerTool::onInitialize()
{
    // Ogre::Sphere sphere;
    // sphere.setRadius(0.01);
    Ogre::Entity* entity = scene_manager_->createEntity("Path Marker", Ogre::SceneManager::PT_SPHERE);
    entity->setMaterialName("BaseYellow");

    moving_path_marker_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
    moving_path_marker_node_->attachObject(entity);

    moving_path_marker_node_->setScale(0.0005, 0.0005, 0.0005);
    moving_path_marker_node_->setVisible(false);
}

void PathMarkerTool::activate()
{
    if(moving_path_marker_node_)
    {
        moving_path_marker_node_->setVisible(true);

        current_marker_property_ = new rviz::VectorProperty("Marker " + QString::number(path_marker_nodes_.size()));
        current_marker_property_->setReadOnly(true);
        getPropertyContainer()->addChild(current_marker_property_);
    }

}

void PathMarkerTool::deactivate()
{
    if(moving_path_marker_node_)
    {
        moving_path_marker_node_->setVisible(false);
        delete current_marker_property_;
        current_marker_property_ = NULL;
    }
}

int PathMarkerTool::processMouseEvent(rviz::ViewportMouseEvent& event)
{
    if(!moving_path_marker_node_){
        return Render;
    }

    Ogre::Vector3 intersection;
    Ogre::Plane groud_plane(Ogre::Vector3::UNIT_Z, 0.0f);
    if(rviz::getPointOnPlaneFromWindowXY(event.viewport,
                                         groud_plane,
                                         event.x, event.y, intersection))

    {
        moving_path_marker_node_->setVisible(true);
        moving_path_marker_node_->setPosition(intersection);
        current_marker_property_->setVector(intersection);

        if(event.leftDown()){
            makePathMarker(intersection);
            current_marker_property_ = NULL;

            MarkerInfo MarkerInfo;
            MarkerInfo.x = event.x;
            MarkerInfo.y = event.y;
            // MarkerInfo.name = ""
            emit markerInfoReceived(MarkerInfo);

            return Render | Finished;
        }
    }
    else
    {
        moving_path_marker_node_->setVisible(false);
    }
    return Render;
}

void PathMarkerTool::makePathMarker(const Ogre::Vector3& position)
{
    Ogre::SceneNode* node = scene_manager_->getRootSceneNode()->createChildSceneNode();
    // Ogre::Entity* entity = scene_manager_->createEntity("Path Marker", Ogre::SceneManager::PT_SPHERE);

    node->attachObject(entity);
    node->setVisible(true);
    node->setPosition(position);
    path_marker_nodes_.push_back(node);
}


QtTeleopPanel::QtTeleopPanel(QWidget* parent)
    : rviz::Panel(parent)
{
    QVBoxLayout* mainlayout = new QVBoxLayout;

    //navigation
    Navigation_groupBox = new QGroupBox("Navigation");
    QVBoxLayout *navigation_layout = new QVBoxLayout(Navigation_groupBox);

    QHBoxLayout* position_layout = new QHBoxLayout;

    X_label = new QLabel("X:");
    X_doubleSpinBox = new QDoubleSpinBox;
    X_doubleSpinBox->setMaximum(10.000);
    X_doubleSpinBox->setMinimum(-10.000);
    X_doubleSpinBox->setSingleStep(0.0001);
    X_doubleSpinBox->setDecimals(4);

    Y_label = new QLabel("Y:");
    Y_doubleSpinBox = new QDoubleSpinBox;
    Y_doubleSpinBox->setMaximum(10.000);
    Y_doubleSpinBox->setMinimum(-10.000);
    Y_doubleSpinBox->setSingleStep(0.0001);
    Y_doubleSpinBox->setDecimals(4);
    Navigation_Send_Button = new QPushButton("Send");

    //view
    QHBoxLayout *view_layout = new QHBoxLayout;
    View_course_checkBox = new QCheckBox;
    View_course_checkBox->setText("View");

    //move
    QHBoxLayout* position_layout = new QHBoxLayout;

    QLabel *Course_start_point_label = new QLabel("Start");
    Course_start_point_comboBox = new QComboBox;

    QLabel *Course_end_point_label = new QLabel("End");
    Course_end_point_comboBox = new QComboBox;

    //slam
    Slam_groupBox = new QGroupBox("SLAM");
    QHBoxLayout* slam_layout = new QHBoxLayout(Slam_groupBox);

    Mapping_checkBox = new QCheckBox;
    Mapping_checkBox->setText("Mapping");

    Map_save_path_plainTextEdit = new QPlainTextEdit;
    Map_save_path_plainTextEdit->setFixedHeight(25);

    Map_save_Button = new QPushButton("Save");

    //work
    QHBoxLayout *work_n_bms_state_layout = new QHBoxLayout;
    QLabel *Work_state_label = new QLabel("Mode:");
    
    Work_state_textBox = new QLabel;
    Work_state_textBox->setText("Navigation");

    //position_layout
    position_layout->addWidget(X_label);
    position_layout->addWidget(X_doubleSpinBox);
    position_layout->addWidget(Y_label);
    position_layout->addWidget(Y_doubleSpinBox);
    position_layout->addWidget(Navigation_Send_Button);

    //view layout
    view_layout->addWidget(View_course_checkBox);

    view_layout->addWidget(Course_start_point_label);
    view_layout->addWidget(Course_start_point_comboBox);

    view_layout->addWidget(Course_end_point_label);
    view_layout->addWidget(Course_end_point_comboBox);
    view_layout->addWidget(Move_to_course_button);

    //slam
    slam_layout->addWidget(Mapping_checkBox);
    slam_layout->addWidget(Map_save_path_plainTextEdit);
    slam_layout->addWidget(Map_save_Button);

    //state
    work_n_bms_state_layout->addWidget(Work_state_label);
    work_n_bms_state_layout->addWidget(Work_state_textBox);

    //mainlayout
    mainlayout->addWidget(Slam_groupBox);

    navigation_layout->addLayout(position_layout);
    navigation_layout->addLayout(view_layout);

    slam_layout->addLayout(slam_layout);


    //connect function
    connect(Mapping_checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_Mapping_checkBox_toggled(int)));

    connect(Map_save_Button, SIGNAL(clicked(bool)), this, SLOT(on_Map_save_Button_clicked(bool)));

    connect(X_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_X_doubleSpinBox_valueChanged(double)));
    connect(Y_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_Y_doubleSpinBox_valueChanged(double)));
    connect(Navigation_Send_Button, SIGNAL(clicked(bool)), this, SLOT(on_Send_Button_clicked(bool)));

    connect(View_course_checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_View_course_checkBox_toggled(int)));
    
    connect(Move_to_course_button, SIGNAL(clicked(bool)), this, SLOT(on_Move_to_course_button_clicked(bool)));

    connect(Course_start_point_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Start_point_comboBox(int)));    
    connect(Course_end_point_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_End_point_comboBox(int)));

    connect(this, SIGNAL(signal_ModeChange(int)), this, SLOT(on_Work_state_changed(int)));

}


void QtTeleopPanel::on_X_doubleSpinBox_valueChanged(double arg1)
{
    qDebug() << "X value changed!";
}

void QtTeleopPanel::on_Y_doubleSpinBox_valueChanged(double arg1)
{
    qDebug() << "Y changed!";
}

void QtTeleopPanel::on_View_course_checkBox_toggled(int state)
{
    if(state == viewOn) {
        view_mode_ = viewOn;
        qDebug() << "Course View ON";
        visualizePath();

    } else if(state == viewOff){
        view_mode_ = viewOff;
        qDebug() << "View OFF";
        hidePath();
    }
}

void QtTeleopPanel::visualizePath()
{
}

void QtTeleopPanel::hidePath()
{
}

void QtTeleopPanel::on_Start_point_comboBox(int index)  //updated by updateComboBox()
{
    if (index >= 0 && index < selectedPoses.size()) {
        start_point_ = selectedPoses[index];
        qDebug() << "Start point selected: " << start_point_.x << ", " << start_point_.y;
    }
}

void QtTeleopPanel::on_End_point_comboBox(int index)  //updated by updateComboBox()
{
    if (index >= 0 && index < selectedPoses.size()) {
        end_point_ = selectedPoses[index];
        qDebug() << "End point selected: " << end_point_.x << ", " << end_point_.y;
    }
}

void QtTeleopPanel::selectMarkerCallback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback)
{
    if(feedback->event_type == visualization_msgs::InteractiveMarkerFeedback::BUTTON_CLICK){
        if(feedback->marker_name == "start_point"){
            // int index = Course_start_point_comboBox->currentIndex();
            // on_Start_point_comboBox(index);

        } else if(feedback->marker_name == "end_point"){
            // int index = Course_end_point_comboBox->currentIndex();
            // on_End_point_comboBox(index);
        }
    }
}

void QtTeleopPanel::on_Send_Button_clicked(bool checked)
{
    // actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> goal_client_("move_base", true);
    goal_client_ goal_client_("move_base", true);
    goal_client_.waitForServer();

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = X_doubleSpinBox->value();
    goal.target_pose.pose.position.y = Y_doubleSpinBox->value();

    goal.target_pose.pose.orientation.w = 1.0;
    
    ROS_INFO("Sending goal");
    goal_client_.sendGoal(goal);

    // goal_client_.waitForResult(); //waiting error...

    if (goal_client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
        ROS_INFO("Robot reached the goal");
    } else {
        ROS_ERROR("Failed to reach the goal");
    }
}

//save every pose information in the list and select some poses acoording to interval between poses
//that forward direction of robot and save as index of combobox
void QtTeleopPanel::updateComboBox()
{
    Course_start_point_comboBox->clear();
    Course_end_point_comboBox->clear();

    std::vector<geometry_msgs::Point> updatedPoses;

    for (double x = 0.0; x <= 10.0; x += 1.0) {
        geometry_msgs::Point pose;
        pose.x = x;
        pose.y = 0.0;
        updatedPoses.push_back(pose);
    }

    selectedPoses = updatedPoses;

    for (int i = 0; i < selectedPoses.size(); i++) {
        Course_start_point_comboBox->addItem(QString("Pose %1").arg(i));
        Course_end_point_comboBox->addItem(QString("Pose %1").arg(i));
    }

    Course_start_point_comboBox->setCurrentIndex(0);
    Course_end_point_comboBox->setCurrentIndex(0);
    
    start_point_ = selectedPoses[0];
    end_point_ = selectedPoses[0];
}

void QtTeleopPanel::on_Mapping_checkBox_toggled(int state)
{
    if (state == SLAM) {
        emit signal_ModeChange(SLAM);

        activateSLAMMode();

    } else if(state == Navigation){
        emit signal_ModeChange(Navigation);

        deactivateSLAMMode();
    }
}

void QtTeleopPanel::on_Work_state_changed(int current_mode_)
{
    if(current_mode_ == SLAM) {
        const QString& text = "SLAM";
        const char* text_ = text.toStdString().c_str();
        Work_state_textBox->setText(text_);

    } else if(current_mode_ == Navigation) {
        const QString& text = "Navigation";
        const char* text_ = text.toStdString().c_str();
        Work_state_textBox->setText(text_);
    }
}

void QtTeleopPanel::activateSLAMMode()
{
}

void QtTeleopPanel::deactivateSLAMMode()
{
}

void QtTeleopPanel::on_Map_save_Button_clicked(bool checked)
{
    std::string map_name = Map_save_path_plainTextEdit->toPlainText().toStdString();
    // saveMap(map_name);
}

void QtTeleopPanel::saveMap(const std::string& map_name)
{
}

void QtTeleopPanel::customEvent(QEvent* event)
{
    if (event->type() == QEvent::User) {
        UpdateLabelEvent* updateEvent = static_cast<UpdateLabelEvent*>(event);

        QString text = updateEvent->getNewText();
        qDebug() << "Change mode to SLAM!!!, mode:" << text;
        Work_state_textBox->setText(text);
    }
}

void QtTeleopPanel::updateWorkState(oonst std::string& work_state) {
    displayWorkState(work_state);
}

} // end namespace

// PLUGINLIB_EXPORT_CLASS(rviz_teleop_plugin::PathMarkerTool, rviz::Tool)


int QtTeleopPanel::processMouseEvent(rviz::ViewportMouseEvent& event)
{
    Ogre::Sphere sphere;
    Ogre::Entity* entity = scene_manager_->createEntity("teleop marker", Ogre::SceneManager::PT_SPHERE);
    entity->setMaterialName("BaseWhite");

    moving_path_marker_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
    moving_path_marker_node_->attachObject(entity);

    moving_path_marker_node_->setScale(0.0005, 0.0005, 0.0005);

    if(moving_path_marker_node_)
    {
        moving_path_marker_node_->setVisible(true);

        current_marker_property_ = new rviz::VectorProperty("Marker " + QString::number(path_marker_nodes_.size()));
        current_marker_property_->setReadOnly(true);
        // getPropertyContainer()->addChild(current_marker_property_);
    }

    if(!moving_path_marker_node_){
        return 0;
    }

    Ogre::Vector3 intersection;
    Ogre::Plane groud_plane(Ogre::Vector3::UNIT_Z, 0.0f);

    if(rviz::getPointOnPlaneFromWindowXY(event.viewport,
                                         groud_plane,
                                         event.x, event.y, intersection))

    {
        moving_path_marker_node_->setVisible(true);
        moving_path_marker_node_->setPosition(intersection);
        current_marker_property_->setVector(intersection);

        if(event.leftDown()){
            current_marker_property_ = NULL;

            MarkerInfo MarkerInfo;
            MarkerInfo.x = event.x;
            MarkerInfo.y = event.y;

            emit markerInfoReceived(MarkerInfo);
        }
    }
    else
    {
        moving_path_marker_node_->setVisible(false);
    }
    return 1;
}
