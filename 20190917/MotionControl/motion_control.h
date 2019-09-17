#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H

#include<QString>

void move(double vx, double vy, double w, double &v1, double &v2, double &v3, double &v4);

void compute_motion_1d(double x_goal, double v_max, double a_max, int &n_acc, int &n_dec, int &n_flat);

void v2hex(double v_input, QString &hexvalue);

void hex_sum(double v1, double v2, double v3, double v4, QString &v_command);

void run_now(double x_goal);

#endif // MOTION_CONTROL_H
