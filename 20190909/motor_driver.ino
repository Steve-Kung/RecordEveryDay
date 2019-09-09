// motor_driver.ino
#include "commands.h";
// 左轮电机
int enA = 5;
int in1 = 7;
int in2 = 8;
// 右轮电机
int enB = 6;
int in3 = 9;
int in4 = 10;
boolean directionLeft = false;
boolean directionRight = false;

boolean direction(int i){
  if(i == LEFT){
    return directionLeft;
  }else{
    return directionRight;
  }
}
void initMotorController() {
  // 设置电机控制引脚为输出
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

// 设置左右轮及其速度
// 通过i判断左轮还是右轮，设置spd速度大小
void setMotorSpeed(int i, int spd) {
  if(spd>MAX_PWM){
    spd=MAX_PWM;
  }
  if(spd<-MAX_PWM){
    spd=-1*MAX_PWM;
  }
  if (i == LEFT){
    if(spd>=0){
        directionLeft = FORWARDS;
        digitalWrite(in2, HIGH);
        digitalWrite(in1, LOW);
        analogWrite(enA, spd);
    }else if(spd < 0){
        directionLeft = BACKWARDS;
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(enA, -spd);
    }
  }
  else {
    if(spd>=0){
        directionRight = FORWARDS;
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enB, spd);
    }else if(spd<0){
        directionRight = BACKWARDS;
        digitalWrite(in4, HIGH);
        digitalWrite(in3, LOW);
        analogWrite(enB, -spd);
        }
    }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}