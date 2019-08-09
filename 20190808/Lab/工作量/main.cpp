#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QByteArray>
#include <QMutex>
#include <QDebug>

#include "lidarthread.h"
#include "coderthread.h"
#include "GloablData.h"
#include "ba2hex.h"

#include "run_now.h"
#define PI 3.14159265359

QSerialPort * coderSerial;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // w.show();

    //initializ the serial port of main PC connect with STM32


    coderSerial= new QSerialPort();
    qDebug()<<"main :"<<coderSerial;
    coderSerial->setPortName("/dev/ttyUSB0");
    coderSerial->open(QIODevice::ReadWrite);
    coderSerial->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    coderSerial->setParity(QSerialPort::NoParity);
    coderSerial->setFlowControl(QSerialPort::NoFlowControl);
    coderSerial->setDataBits(QSerialPort::Data8);
    coderSerial->setStopBits(QSerialPort::OneStop);


    //----------------------------------

//    coderthread * serialConnectSend = new coderthread();
//    QString v_command;
//    // v_command = QString("fffe") +
//    serialConnectSend->command = "fffe1000100000000000";
//    // serialConnectSend->command = "fffe0000000000000000";
//    QByteArray senddata;
//    StringToHex(serialConnectSend->command,senddata);
//    coderSerial->write(senddata);
//    coderSerial->flush();
//    QThread::msleep(1000);

//    double x_goal = 0.5;
//    run_now(x_goal);

    double theta_goal = PI/2.0;
    run_w_now(theta_goal);

    //    通过串口向下位机发送小车速度信号
    //-----------------------------------

    //     接收下位机发送过来的编码器信号
    QThread * serialThreadreceive = new QThread;
    coderthread * serialConnectreceive = new coderthread();

    QObject::connect(serialThreadreceive,SIGNAL(started()),serialConnectreceive,SLOT(MonitoringCoderEnable()));

    serialConnectreceive->moveToThread(serialThreadreceive);



    QThread * lidarThread = new QThread();
    LidarThread * readLidar = new LidarThread();
    QObject::connect(lidarThread,SIGNAL(started()),readLidar,SLOT(MonitoringLidarEnable()));

    readLidar->moveToThread(lidarThread);


    serialThreadreceive->start();
    lidarThread->start();

    //main thread send rplidar read command;
    QUdpSocket * lidarStartSocket = new QUdpSocket();
    extern const int lidar_thread_port;
    QString command = "start";
    qDebug()<<"start";
    lidarStartSocket->writeDatagram(command.toLatin1(),QHostAddress::LocalHost,lidar_thread_port);


    return a.exec();
}
