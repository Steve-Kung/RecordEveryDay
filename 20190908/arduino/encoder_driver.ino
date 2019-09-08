// encoder_driver.ino
// 当轮子转动，pin2和pin3引脚就会产生0号和1号中断，每次产生中断，根据轮子转动的方向计数器+1或-1
#include "motor_driver.h"
#include "commands.h";

/* encode */
volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;
int left_rotate = 0;
int right_rotate = 0;

void initEncoders(){
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  attachInterrupt(0, encoderLeftISR,  CHANGE); // 左轮的A相
  attachInterrupt(1, encoderLeftISR,  CHANGE); // 左轮的B相
  attachInterrupt(4, encoderRightISR, CHANGE); // 右轮的A相
  attachInterrupt(5, encoderRightISR, CHANGE); // 右轮的B相
}

// 根据左轮转动的方向计数器+和-
void encoderLeftISR(){
    if(direction(LEFT) == BACKWARDS){
        left_enc_pos--;
    }else{
        left_enc_pos++;
    }
}

// 根据右轮转动的方向计数器+和-
void encoderRightISR(){
    if(direction(RIGHT) == BACKWARDS){
      right_enc_pos--;
    }else{
      right_enc_pos++;
    }
}

// 读编码器数值
long readEncoder(int i) {
  long encVal = 0L; // 归零操作
  if (i == LEFT)  {
    noInterrupts();
    encVal = left_enc_pos;
    interrupts();
  }
  else {
    noInterrupts();
    encVal = right_enc_pos;
    interrupts();
  }
  return encVal;
 }

//分别指定AB相编码器复位，数值为0
void resetEncoder(int i) {
  if (i == LEFT){
    noInterrupts();
    left_enc_pos=0L;
    interrupts();
    return;
  } else { 
    noInterrupts();
    right_enc_pos=0L;
    interrupts();
    return;
  }
}
//指定AB相编码器都复位，数值为0
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}