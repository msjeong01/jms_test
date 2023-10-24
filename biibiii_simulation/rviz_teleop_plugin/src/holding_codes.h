using namespace visualization_msgs;
using namespace interactive_markers;

namespace rviz_teleop_plugin
{

class QtTeleopPanel : public rviz::Panel
{
public:
    ~QtTeleopPanel();

public Q_SLOTS:

protected Q_SLOTS:
    void on_X_doubleSpinBox_valueChanged(double arg1);
    void on_Y_doubleSpinBox_valueChanged(double arg1);

    void on_View_course_checkBox_toggled(int state);
    void visualizePath();
    void hidePath();

    void on_Start_point_comboBox(int index);
    void on_End_point_comboBox(int index);
    void updateComboBox();

    void on_Mapping_checkBox_toggled(int state);
    void on_Map_save_path_plainTextEdit(std::string title);
    void on_Map_save_Button_clicked(bool checked);
    void activateSLAMMode();
    void deactivateSLAMMode();

    void on_Work_state_changed(int current_mode_);


    void on_Send_Button_clicked(bool checked);

protected:
    //navigation
    QLabel *X_label;
    QDoubleSpinBox *X_doubleSpinBox;
    QLabel *Y_label;
    QDoubleSpinBox *Y_doubleSpinBox;
    QPushButton *Navigation_Send_Button;

    //view
    QCheckBox *View_course_checkBox;

    //move
    QLabel *Course_start_point_label;
    QLabel *Course_end_point_label;

    QComboBox *Course_start_point_comboBox;
    QComboBox *Course_end_point_comboBox;

    QPushButton *Move_to_course_button;

    //slam
    QGroupBox *Slam_groupBox;

    QCheckBox *Mapping_checkBox;
    QPlainTextEdit *Map_save_path_plainTextEdit;
    QPushButton *Map_save_Button;

    QLabel *Work_state_label;
    QLabel *Work_state_textBox;
    // QMutex *mutex;

    //
    QMenuBar *menubar;
    QMenu *menuteleop_panel;
    QStatusBar *statusbar;

    // int current_mode_;
    int view_mode_;



Q_SIGNALS:
    void signal_ModeChange(int current_mode_);


};

enum ViewMode{
    viewOff = 0,
    viewOn = 2
};


class PathMarkerTool : public rviz::Tool
{
Q_OBJECT

public:
    PathMarkerTool();
    ~PathMarkerTool();

    virtual void onInitialize();

    virtual void activate();
    virtual void deactivate();

    // virtual void load( const rviz::Config& config );
    // virtual void save( rviz::Config config ) const;
public Q_SLOTS:
    virtual int processMouseEvent(rviz::ViewportMouseEvent& event);

private:
    void makePathMarker(const Ogre::Vector3& position);
    // void receiveMarkerDataFromPanel();

    Ogre::SceneManager* scene_manager_;
    std::vector<Ogre::SceneNode*> path_marker_nodes_;
    Ogre::SceneNode* node;
    Ogre::SceneNode* moving_path_marker_node_;
    // std::string marker_resource_;
    visualization_msgs::Marker marker_resource_;
    rviz::VectorProperty* current_marker_property_;
    Ogre::Entity* entity;
    Ogre::Vector3 intersection;
    Ogre::Plane groud_plane;

    ros::NodeHandle nh_;


Q_SIGNALS:
    void markerInfoReceived(const MarkerInfo& markerInfo);
};


} // end namespace

#endif // QT_TELEOP_UI_H