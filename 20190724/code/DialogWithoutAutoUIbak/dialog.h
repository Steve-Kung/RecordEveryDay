#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    //类构造函数和析构函数
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    //控件成员声明
    //必须放在类的私有区
    //文件中用到哪个类，开始的时候引用此类头文件
    //创建UI界面拥有的控件对象
    QLabel *label1,*label2;
    QLineEdit *lineEdit;
    QPushButton *button;

private slots:
    void showArea();
};

#endif // DIALOG_H
