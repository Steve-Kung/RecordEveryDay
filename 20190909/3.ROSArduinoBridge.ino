// ROSArduinoBridge.ino
/*
下位机主程序的工作流程
实时读取上位机的指令并作出对应的响应(支持的指令见源文件commands.h)
GET_BAUDRATE 获取串口通讯的比特率
READ_ENCODERS 读取左右轮编码器的计数
MOTOR_SPEEDS 设置左右轮的期望速度
RESET_ENCODERS 重置编码器的计数
UPDATE_PID 更新PID参数
READ_PIDOUT 读取PID计算的PWM值，为后续调整PID参数提供参考
READ_PIDIN 读取一个PID周期内编码器的计数，为后续调整PID参数提供参考
在每个PID周期内，比较左右轮期望速度和实际速度的差异，调整PWM的。
如果AUTO_STOP_INTERVAL毫秒内上位机没有期望速度，就会停止小车运动。
*/
#define BAUDRATE     115200
#define MAX_PWM        255

#include "Arduino.h"
#include "commands.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "diff_controller.h"
#define PID_RATE           30     // Hz
const int PID_INTERVAL = 1000 / PID_RATE;
unsigned long nextPID = PID_INTERVAL;
#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/* 变量初始化 */
// 一对变量来帮助解析串行命令
int arg = 0;
int index = 0;

// 用于保存输入字符的变量
char chr;

// 变量用于保存当前的单字符命令
char cmd;

// 用于保存第一个和第二个参数的字符数组
char argv1[16];
char argv2[16];

// 参数转换为整数
long arg1;
long arg2;

/* 清除当前命令参数 */
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}
/* 运行命令,命令在commands.h中定义 */
int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1); // atoi (表示 ascii to integer)是把字符串转换成整型数的一个函数
  arg2 = atoi(argv2);

  switch (cmd) {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;
    case READ_PIDIN:
      Serial.print( readPidIn(LEFT));
      Serial.print(" ");
      Serial.println( readPidIn(RIGHT));
      break;      
    case READ_PIDOUT:
      Serial.print( readPidOut(LEFT));
      Serial.print(" ");
      Serial.println( readPidOut(RIGHT));
      break;
    case READ_ENCODERS:
      Serial.print(readEncoder(LEFT));
      Serial.print(" ");
      Serial.println(readEncoder(RIGHT));
      break;
    case RESET_ENCODERS:
      resetEncoders();
      resetPID();
      Serial.println("OK");
      break;
    case MOTOR_SPEEDS:
	// 重置自动停止计时器
      lastMotorCommand = millis();
      if (arg1 == 0 && arg2 == 0) {
        setMotorSpeeds(0, 0);
        moving = 0;
      }
      else moving = 1;
      leftPID.TargetTicksPerFrame = arg1;
      rightPID.TargetTicksPerFrame = arg2;
      Serial.println("OK");
      break;
    case UPDATE_PID:
      while ((str = strtok_r(p, ":", &p)) != '\0') {
        pid_args[i] = atoi(str);
        i++;
      }
      Kp = pid_args[0];
      Kd = pid_args[1];
      Ki = pid_args[2];
      Ko = pid_args[3];
      Serial.println("OK");
      break;
    case ANALOG_READ:
      Serial.println(analogRead(arg1));
      break;
    case DIGITAL_READ:
      Serial.println(digitalRead(arg1));
      break;
    case ANALOG_WRITE:
      analogWrite(arg1, arg2);
      Serial.println("OK");
      break;
    case DIGITAL_WRITE:
      if (arg2 == 0) digitalWrite(arg1, LOW);
      else if (arg2 == 1) digitalWrite(arg1, HIGH);
      Serial.println("OK");
      break;
    case PIN_MODE:
      if (arg2 == 0) pinMode(arg1, INPUT);
      else if (arg2 == 1) pinMode(arg1, OUTPUT);
      Serial.println("OK");
      break;
    default:
      Serial.println("Invalid Command");
      break;
  }
}

unsigned long time = 0, old_time = 0;

// 设置功能 - 在启动时运行一次。
void setup() {
  Serial.begin(BAUDRATE);
  initEncoders();
  initMotorController();
  resetPID();
}

void loop() {
  while (Serial.available() > 0) {
	// 读入下一个字符
    chr = Serial.read();
	// 使用CR终止命令
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }
	// 使用空格分隔命令的各个部分
    else if (chr == ' ') {
	  // 逐步论证
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
		// 第一个arg是单字母命令
        cmd = chr;
      }
      else if (arg == 1) {
		// 后续参数可以是多个字符
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }

// 如果我们使用基本控制，则以适当的间隔运行PID计算
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
// 检查我们是否超过了自动停止间隔
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}