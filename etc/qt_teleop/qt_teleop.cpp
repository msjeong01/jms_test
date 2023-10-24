#include "qt_teleop.h"
#include "./ui_qt_teleop.h"

qt_teleop::qt_teleop(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::qt_teleop)
{
    ui->setupUi(this);
}

qt_teleop::~qt_teleop()
{
    delete ui;
}


void qt_teleop::on_Up_Button_pressed()
{

}

void qt_teleop::on_Down_Button_pressed()
{

}

void qt_teleop::on_Left_Button_pressed()
{

}

void qt_teleop::on_RIght_Button_pressed()
{

}

void qt_teleop::on_Stop_Button_pressed()
{

}

void qt_teleop::on_Send_Button_clicked(bool checked)
{

}

void qt_teleop::on_X_doubleSpinBox_valueChanged(double arg1)
{

}

void qt_teleop::on_Y_doubleSpinBox_valueChanged(double arg1)
{

}

void qt_teleop::on_Linear_doubleSpinBox_valueChanged(double arg1)
{

}

void qt_teleop::on_Angular_doubleSpinBox_valueChanged(double arg1)
{

}
