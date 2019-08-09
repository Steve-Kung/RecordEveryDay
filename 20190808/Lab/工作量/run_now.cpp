#include "run_now.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>
#include "motion_control.h"
#include "time.h"
#include <QThread>
#include "coderthread.h"
#include "GloablData.h"
#include "ba2hex.h"

#define PI 3.14159265359


void run_now(double x_goal)
{

    double t_acc, t_dec, t_flat;
    // x_goal = 1.0; //m
    double v_max = 0.5, a_max = 1.0;
    motion_control(x_goal, v_max, a_max, t_acc, t_dec, t_flat);

    double T_std = 20; // ms
    int n_acc, n_dec, n_flat;
    n_acc = (int)(t_acc/T_std);
    n_dec = (int)(t_dec/T_std);
    n_flat = (int)(t_flat/T_std);

    double v_input = 0.0;
    QString v_command;
    coderthread * serialConnectSend = new coderthread();
    //acc
    for (int i=0;i<n_acc;i++) {
        v_input += a_max * T_std/1000;
        v2hex(v_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }
    //flat
    for (int i=0;i<n_flat;i++) {
        v2hex(v_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }
    //dec
    for (int i=0;i<n_dec;i++) {
        v_input -= a_max * T_std/1000;
        v2hex(v_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }
}

void run_w_now(double theta_goal)
{
    double t_w_acc, t_w_dec, t_w_flat;
    double w_min = 0.17, w_max = 0.85, alpha_max = 8.5;
    motion_control_w(theta_goal, w_min, w_max, alpha_max, t_w_acc, t_w_dec, t_w_flat);
    qDebug()<< "t_w_acc = " << t_w_acc << "|"
               << "t_w_dec = " << t_w_dec << "|"
                  << "t_w_flat = " << t_w_flat;
    double T_std = 20; // ms
    int n_w_acc, n_w_dec, n_w_flat;
    n_w_acc = round(t_w_acc/T_std);
    n_w_dec = round(t_w_dec/T_std);
    n_w_flat = round(t_w_flat/T_std);
    qDebug()<< "n_w_acc = " << n_w_acc << "|"
               << "n_w_dec = " << n_w_dec << "|"
                  << "n_w_flat = " << n_w_flat;

    double w_input = w_min;
    QString v_command;
    coderthread * serialConnectSend = new coderthread();
    // QString hexvalue;
    //acc
    for (int i=0;i<n_w_acc;i++) {
        w_input += alpha_max * T_std/1000;
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }
    qDebug()<<"***------------------------------***";
    //flat
    for (int i=0;i<n_w_flat;i++) {
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }
    qDebug()<<"***------------------------------***";
    //dec
    for (int i=0;i<n_w_acc;i++) {
        w_input -= alpha_max * T_std/1000;
        qDebug()<<"w_input = "<< w_input;
        w2hex(w_input, v_command);

        serialConnectSend->command = v_command;
        QByteArray senddata;
        StringToHex(serialConnectSend->command,senddata);
        coderSerial->write(senddata);
        coderSerial->flush();

        QThread::msleep(20);
    }

    w2hex(0.0, v_command);

    serialConnectSend->command = v_command;
    QByteArray senddata;
    StringToHex(serialConnectSend->command,senddata);
    coderSerial->write(senddata);
    coderSerial->flush();

}
