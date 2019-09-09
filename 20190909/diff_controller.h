// diff_controller.h
// 左右轮的PID用的是同一套Kp,Kd,Ki,Ko， 如果你的两个电机特性差异比较大，就要对这块做下优化
typedef struct {
  double TargetTicksPerFrame;    // 每帧目标滴答数
  long Encoder;                  // 当前编码器数值
  long PrevEnc;                  // 上一次编码器数值
  int PrevInput;                // 上一次输入值
  int ITerm;                    // 积分项
  long output;                    // last motor setting
}
SetPointInfo;

SetPointInfo leftPID, rightPID;

int Kp = 20;
int Kd = 12;
int Ki = 0;
int Ko = 50;

unsigned char moving = 0; // is the base in motion?

// 重置左右电机PID相关系数为0
void resetPID(){
   leftPID.TargetTicksPerFrame = 0.0;
   leftPID.Encoder = readEncoder(LEFT);
   leftPID.PrevEnc = leftPID.Encoder;
   leftPID.output = 0;
   leftPID.PrevInput = 0;
   leftPID.ITerm = 0;

   rightPID.TargetTicksPerFrame = 0.0;
   rightPID.Encoder = readEncoder(RIGHT);
   rightPID.PrevEnc = rightPID.Encoder;
   rightPID.output = 0;
   rightPID.PrevInput = 0;
   rightPID.ITerm = 0;
}

//pid调节
void doPID(SetPointInfo * p) {
  long Perror;
  long output;
  int input;
  input = p->Encoder - p->PrevEnc;
  Perror = p->TargetTicksPerFrame - input;

  output = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko;
  p->PrevEnc = p->Encoder;

  output += p->output;
  if (output >= MAX_PWM)
    output = MAX_PWM;
  else if (output <= -MAX_PWM)
    output = -MAX_PWM;
  else
    p->ITerm += Ki * Perror;

  p->output = output;
  p->PrevInput = input;
}
// 更新pid到电机速度
void updatePID() {
  leftPID.Encoder = readEncoder(LEFT);
  rightPID.Encoder = readEncoder(RIGHT);

  if (!moving){
    if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0) resetPID();
    return;
  }

  doPID(&rightPID);
  doPID(&leftPID);
  setMotorSpeeds(leftPID.output, rightPID.output);
}

// 读PID输入，读取一个PID周期内编码器的计数，为后续调整PID参数提供参考
long readPidIn(int i) {
  long pidin=0;
    if (i == LEFT){
    pidin = leftPID.PrevInput;
  }else {
    pidin = rightPID.PrevInput;
  }
  return pidin;
}

// 读PID输出，读取PID计算的PWM值，为后续调整PID参数提供参考
long readPidOut(int i) {
  long pidout=0;
    if (i == LEFT){
    pidout = leftPID.output;
  }else {
    pidout = rightPID.output;
  }
  return pidout;
}