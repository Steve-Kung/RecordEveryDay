// enA频率控制小车快慢
// in1高 in2低 电机正转
// in1低 in2高 电机反转
// in1低 in2低 电机停转
// 电机1
int enA = 5;
int in1 = 7;
int in2 = 8;
// 电机2
int enB = 6;
int in3 = 9;
int in4 = 10;

//初始化
void setup()
{
  // 设置所有的电机引脚为输出模式
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
// demo1
void demoOne()
{
  // demo1功能：两电机以各自不同方向，相同速度运行
  // 打开电机1，正转
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // 设置电机转速200，转速范围（0~255）
  analogWrite(enA, 200);
  // 打开电机2，反转
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  // 设置电机转速200，转速范围（0~255）
  analogWrite(enB, 200);
  delay(2000);
  // 改变电机转速方向
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH); 
  //arduino 时间函数
  //millis() 获取机器运行的时间长度, 单位毫秒. 系统最长的记录时间接近50天, 如果超出时间将从0开始.
  //		警告:时间为 unsigned long类型, 如果用 int 保存时间将得到错误结果
  //delay(ms) 参数为unsigned long, 因此在延时参数超过32767(int型最大值)时, 需要用"UL"后缀表示为无符号长整型
  //		delay((unsigned long)tdelay * 100UL);
  //delayMicroseconds(us) 延时, 单位为微妙(1毫秒有1000微妙). 如果延时的时间有几千微妙, 那么建议使用 delay 函数. 
  //		目前参数最大支持16383微妙
  delay(2000);
  // 使电机停止
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
// demo2
void demoTwo()
{
  // demo2功能：两电机以各自不同速度运行
  // 最大速度取决于电机本身和其两端电压
  // PWM值通过 analogWrite()写入 ，为最大速度的分数
  // 打开电机1，反转
  // 打开电机2，反转
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  // 从零到最大速度进行加速
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // 从最大速度到零进行减速
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // 使电机停止
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}
void loop()
{
  demoOne(); //先运行demo1
  delay(1000);
  demoTwo(); //再运行demo2
  delay(1000);
}