#ifndef QT_TELEOP_H
#define QT_TELEOP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class qt_teleop; }
QT_END_NAMESPACE

class qt_teleop : public QMainWindow
{
    Q_OBJECT

public:
    qt_teleop(QWidget *parent = nullptr);
    ~qt_teleop();

private slots:
    void on_Up_Button_pressed();
    void on_Down_Button_pressed();
    void on_Left_Button_pressed();
    void on_RIght_Button_pressed();
    void on_Stop_Button_pressed();

    void on_Send_Button_clicked(bool checked);

    void on_X_doubleSpinBox_valueChanged(double arg1);
    void on_Y_doubleSpinBox_valueChanged(double arg1);

    void on_Linear_doubleSpinBox_valueChanged(double arg1);
    void on_Angular_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::qt_teleop *ui;
};
#endif // QT_TELEOP_H
