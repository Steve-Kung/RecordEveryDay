#include "motion_control.h"
#include <math.h>
#include <QCoreApplication>
#include <QDebug>
#define PI 3.14159265359

// 定义最大速度 v_max = 0.5m/s 须知
// 目标距离 x_goal = 1m 须知
// 最大加速度 a_max = 1m/s2 须知
// 加速段时间 t_acc
// 减速段时间 t_dec
// 匀速段时间 t_flat
// 输入目标距离x_goal，得到三段时间t


void motion_control(double x_goal, double v_max, double a_max, double &t_acc, double &t_dec, double &t_flat)
{
    double x_threshold = pow(v_max, 2)/a_max;
    if (x_goal <= x_threshold)
    {
        t_flat = 0 * 1000;
        t_acc = t_dec = sqrt(x_goal/a_max) * 1000;
    }
    else
    {
        t_acc = t_dec = v_max/a_max * 1000;
        t_flat = (x_goal - x_threshold)/v_max * 1000;
    }
}

void v2hex(double v_input, QString &v_command)
{
    double num_input;
    num_input = (v_input * 1560/(100 * PI * 6.3 * 0.01));
    QString hexvalue =  QString("%1").arg((int)num_input,2,16,QLatin1Char('0'));
    v_command = QString("fffe") + hexvalue + QString("00") + hexvalue + QString("00") + QString("00000000");
}



//-----------------------------------------------------------------------
// 定义最大角速度 w_max = 0.5 rad/s 须知
// 最小角速度 w_min = 0.18 rad/s 须知
// 目标角度 theta_goal = PI/2 rad 须知
// 最大加速度 alpha_max = 1 rad/s2 须知
// 加速段时间 t_w_acc
// 减速段时间 t_w_dec
// 匀速段时间 t_w_flat
// 输入目标角度theta_goal，得到三段时间t_w


void motion_control_w(double theta_goal, double w_min, double w_max, double alpha_max, double &t_w_acc, double &t_w_dec, double &t_w_flat)
{
    double theta_threshold = (pow(w_max, 2)-pow(w_min, 2))/alpha_max;
    if (theta_goal <= theta_threshold)
    {
        t_w_flat = 0 * 1000;
        t_w_acc = t_w_dec = ((-2*w_min+sqrt(4*pow(w_min, 2)+4*alpha_max*theta_goal))/(2*alpha_max)) * 1000;
    }
    else
    {
        t_w_acc = t_w_dec = (w_max-w_min)/alpha_max * 1000;
        t_w_flat = (theta_goal - theta_threshold)/w_max * 1000;
    }
}

void w2hex(double w_input, QString &v_command)
{
    double v_input;
    v_input = w_input*15.4*0.01/2.0;
    qDebug()<<"v_input = "<< v_input;
    double num_input;
    num_input = (v_input * 1560/(100 * PI * 6.3 * 0.01));
    qDebug()<<"num_input:"<< num_input;
    QString hexvalue =  QString("%1").arg((int)num_input,2,16,QLatin1Char('0'));// k为int型或char型都可
    qDebug()<<"hexvalue:"<< hexvalue;
    // QString v_command;
    v_command = QString("fffe") + hexvalue + QString("01") + hexvalue + QString("00") + QString("00000000");
    qDebug()<<"v_command = "<< v_command;
}

