#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H
#include <QCoreApplication>


void motion_control(double x_goal, double v_max, double a_max, double &t_acc, double &t_dec, double &t_flat);

void v2hex(double v_input, QString &hexvalue);

void motion_control_w(double theta_goal, double w_min, double w_max, double alpha_max, double &t_w_acc, double &t_w_dec, double &t_w_flat);

void w2hex(double w_input, QString &hexvalue);

#endif // MOTION_CONTROL_H
