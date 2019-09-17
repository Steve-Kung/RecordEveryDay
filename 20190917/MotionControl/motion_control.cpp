#include "motion_control.h"
#include <math.h>
#include <QThread>
#include <QDebug>
#include <cmath>

const double PI = 3.1415926535898;
// 横向轮子距离一半 单位mm
const double diPan_a = 1106/2.0;
// 纵向轮子距离一半 单位mm
const double diPan_b = 1180/2.0;
// 麦克纳姆轮子直径 单位mm
const double maiLun_d = 205;
// 离散周期 单位ms
const double T_std = 20;
// 减速比 50：1
const int reductionRatio = 50;
// 电机转一圈，电机输出脉冲数，不带减速器
const int FSPR = 60;
const int SPR = reductionRatio * FSPR;

// 轮子O型排布 轮子1234命名按逆时针象限顺序排布
//1、小车整体速度Vx,Vy、角速度W分解到四个电机转速控制 正方向：Vx向右，Vy向前
void move(double vx, double vy, double w, double &v1, double &v2, double &v3, double &v4)
{
    v1 = vy - vx + w * (diPan_a + diPan_b);
    v2 = vy + vx - w * (diPan_a + diPan_b);
    v3 = vy - vx - w * (diPan_a + diPan_b);
    v4 = vy + vx + w * (diPan_a + diPan_b);
}

// 2、小车前后左右前进一米，顺时针、逆时针旋转45度角的梯形加减速规划

// 根据目标距离，梯形加减速三段时间规划
// x_goal:  m
// v_max:   m/s
// a_max:   m/s2
// t_acc, t_dec, t_flat:    s->ms
// n_acc, n_dec, n_flat:    离散化处理    
void compute_motion_1d(double x_goal, double v_max, double a_max, int &n_acc, int &n_dec, int &n_flat)
{
    double t_acc, t_dec, t_flat;
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
    n_acc = (int)(t_acc/T_std);
    n_dec = (int)(t_dec/T_std);
    n_flat = (int)(t_flat/T_std);
}

// 处理电机v速度和大小转换为指令格式
void v2hex(double v_input, QString &hexvalue)
{
    // 10ms发的脉冲数
    double num_input;
    num_input = fabs(v_input * SPR/(100 * PI * maiLun_d * 0.001));
    QString hexvalue00 =  QString("%1").arg((int)num_input,2,16,QLatin1Char('0'));
    QString hexvalue01;
    if (v_input > 0){
        hexvalue01 = QString("00");
    }
    else{
        hexvalue01 = QString("01");
    }
    hexvalue = hexvalue00 + hexvalue01;
}

// 处理四个轮子的速度转换成下发指令
void hex_sum(double v1, double v2, double v3, double v4, QString &v_command)
{
    QString hexvalue1, hexvalue2, hexvalue3, hexvalue4;
    v2hex(v1, hexvalue1);
    v2hex(v2, hexvalue2);
    v2hex(v3, hexvalue3);
    v2hex(v4, hexvalue4);
    v_command = QString("fffe") + hexvalue1 + hexvalue2 + hexvalue3 + hexvalue4;
    qDebug()<<v_command;
}

//行驶直线距离x_goal
void run_now(double x_goal)
{
    //auto SCP = SCummunicate::instance();
    
    double v_max = 0.2, a_max = 0.5;
    int n_acc, n_dec, n_flat;
    double v_input = 0.0;
    double vx = 0, vy = 0, w = 0;
    double v1, v2, v3, v4;
    
    QString v_command;

    if(x_goal>0){
        compute_motion_1d(x_goal, v_max, a_max, n_acc, n_dec, n_flat);
        //acc
        for (int i=0; i<n_acc; i++) {
            v_input += a_max * T_std/1000;
            vy = v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_q(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
        //flat
        for (int i=0; i<n_flat; i++) {
            vy = v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_q(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
        //dec
        for (int i=0; i<n_dec; i++) {
            v_input -= a_max * T_std/1000;
            vy = v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_q(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
    }
    else {
        x_goal = -x_goal; //m
        compute_motion_1d(x_goal, v_max, a_max, n_acc, n_dec, n_flat);
        //acc
        for (int i=0; i<n_acc; i++) {
            v_input += a_max * T_std/1000;
            vy = -1 * v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_h(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
        //flat
        for (int i=0; i<n_flat; i++) {
            vy = -1 * v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_h(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
        //dec
        for (int i=0; i<n_dec; i++) {
            v_input -= a_max * T_std/1000;
            vy = -1 * v_input; 
            move(vx, vy, w, v1, v2, v3, v4);
            hex_sum(v1, v2, v3, v4, v_command);
            //v2hex_h(v_input, v_command);
            //SCP->sendDataUpToLow(v_command);
            QThread::msleep(T_std);
        }
    }
}
