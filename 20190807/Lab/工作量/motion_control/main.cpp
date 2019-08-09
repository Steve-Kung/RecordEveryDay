#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>
#include "motion_control.h"
#include "time.h"
#include <QThread>
#define PI 3.14159265359


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // QThread::msleep(1000);
    // QElapsedTimer t;
    // t.start();
    // qDebug()<<"start!!!";
    // clock_t start ,stop ,duratimeTime;
    // start = clock();
    // while(t.elapsed()<=1001);
    // QThread::msleep(20);
    // stop = clock();
    // duratimeTime = (double)(stop - start)/CLOCKS_PER_SEC * 1000;
    // qDebug()<<"assume_time:"<< duratimeTime << "ms";
    // qDebug()<<"end!!!";

    double t_acc, t_dec, t_flat;
    double x_goal = 0.5, v_max = 0.5, a_max = 1.0;
    motion_control(x_goal, v_max, a_max, t_acc, t_dec, t_flat);
    qDebug()<< "t_acc = " << t_acc << "|"
               << "t_dec = " << t_dec << "|"
                  << "t_flat = " << t_flat;
    double T_std = 20; // ms
    int n_acc, n_dec, n_flat;
    n_acc = (int)(t_acc/T_std);
    n_dec = (int)(t_dec/T_std);
    n_flat = (int)(t_flat/T_std);
    qDebug()<< "n_acc = " << n_acc << "|"
               << "n_dec = " << n_dec << "|"
                  << "n_flat = " << n_flat;

    double v_input = 0.0;
    QString hexvalue;
    //acc
    for (int i=0;i<n_acc;i++) {
        v_input += a_max * T_std/1000;
        qDebug()<<"v_input = "<< v_input;
        v2hex(v_input, hexvalue);
        QThread::msleep(20);
    }
    qDebug()<<"------------------------------";
    //flat
    for (int i=0;i<n_flat;i++) {
        qDebug()<<"v_input = "<< v_input;
        v2hex(v_input, hexvalue);
        QThread::msleep(20);
    }
    qDebug()<<"------------------------------";
    //dec
    for (int i=0;i<n_acc;i++) {
        v_input -= a_max * T_std/1000;
        qDebug()<<"v_input = "<< v_input;
        v2hex(v_input, hexvalue);
        QThread::msleep(20);
    }

    qDebug()<<"************************************************";

    double t_w_acc, t_w_dec, t_w_flat;
    double theta_goal = PI/2, w_min = 0.17, w_max = 0.85, alpha_max = 8.5;
    motion_control_w(theta_goal, w_min, w_max, alpha_max, t_w_acc, t_w_dec, t_w_flat);
    qDebug()<< "t_w_acc = " << t_w_acc << "|"
               << "t_w_dec = " << t_w_dec << "|"
                  << "t_w_flat = " << t_w_flat;
    // double T_std = 20; // ms
    int n_w_acc, n_w_dec, n_w_flat;
    n_w_acc = round(t_w_acc/T_std);
    n_w_dec = round(t_w_dec/T_std);
    n_w_flat = round(t_w_flat/T_std);
    qDebug()<< "n_w_acc = " << n_w_acc << "|"
               << "n_w_dec = " << n_w_dec << "|"
                  << "n_w_flat = " << n_w_flat;

    double w_input = 0.0;
    // QString hexvalue;
    //acc
    for (int i=0;i<n_w_acc;i++) {
        w_input += alpha_max * T_std/1000;
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, hexvalue);
        QThread::msleep(20);
    }
    qDebug()<<"***------------------------------***";
    //flat
    for (int i=0;i<n_w_flat;i++) {
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, hexvalue);
        QThread::msleep(20);
    }
    qDebug()<<"***------------------------------***";
    //dec
    for (int i=0;i<n_w_acc;i++) {
        w_input -= alpha_max * T_std/1000;
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, hexvalue);
        QThread::msleep(20);
    }

    return a.exec();
}
