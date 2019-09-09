#define BAUDRATE     115200
#define LEFT            0	//左轮A相
#define RIGHT           1	//右轮A相
#define FORWARDS true
#define BACKWARDS false

//如果一个变量所在的代码段可能会意外地导致变量值改变那此变量应声明为volatile，
//比如并行多线程等。在arduino中，唯一可能发生这种现象的地方就是和中断有关的代码段，成为中断服务程序。
// 中断函数中使用的变量需要定义为 volatile 类型.
volatile long encoderLeft = 0L;
volatile long encoderRight = 0L;

//初始化编码器
void initEncoders(){
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  //中断函数（中断源，中断触发函数，中断触发信号）
  //中断源可选值为0或1，一般分别对应2号和3号引脚
  //需要中断的函数名
  //LOW（低电平触发）、CHANGE（变化时触发）、RISING（低电平变为高电平触发）、FALLING（高电平变为低电平触发）
  attachInterrupt(0, encoderLeftISR, CHANGE);  
  attachInterrupt(1, encoderRightISR, CHANGE);
}

//中断触发函数
void encoderLeftISR(){
    encoderLeft++;
}

//中断触发函数
void encoderRightISR(){
    encoderRight++;
}


//读左轮或右轮编码器
long readEncoder(int i) {
  long encVal = 0L;
  if (i == LEFT)  {
     noInterrupts(); //关中断
    //detachInterrupt(0); //取消中断;取消指定类型的中断.
    encVal = encoderLeft;
    interrupts(); //开中断
    //attachInterrupt(0, Code_left, FALLING);
  }
  else {
    noInterrupts();	//关中断
    //detachInterrupt(1);
    encVal = encoderRight;
    interrupts();	//开中断
    //attachInterrupt(1, Code_right, FALLING);
  }
  return encVal;
}

//指定左轮或右轮编码器复位，数值为0
void resetEncoder(int i) {
  if (i == LEFT){
    noInterrupts();
    encoderLeft = 0L;
    interrupts();
  }else {
    noInterrupts();
    encoderRight = 0L;
     interrupts();
  }
}
//左右轮编码器复位，数值都为0
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

//初始化
 void setup() {
  // 串口通信函数说明：
  // begin()：打开串口，参数波特率；Serial.begin(BAUDRATE);
  // available()：获取串口上可读取的数据的字节数,函数返回可读取的字节数；Serial.available();
  //				该数据是指已经到达并存储在接收缓存（共有64字节）中。available()继承自Stream实用类。
  // read()：读串口数据，read()继承自Stream实用类。Serial.read();
  //		返回值：串口上第一个可读取的字节（如果没有可读取的数据则返回-1）- int型。
  // flush(): 刷新串口数据
  // print(): 往串口发数据,串口输出数据函数。语法:1)Serial.print(val);2)Serial.print(val,format)
  // 参数: val: 打印的值，任意数据类型;format: 输出的数据格式，包括整数类型和浮点型数据的小数点位数。
  // println(): 串口输出数据函数。与Serial.print()不同的是输出数据带回车符。
  // write(): 写二进制数据到串口，数据是一个字节一个字节地发送的，若以字符形式发送数字请使用print()代替。
  // peak(): 串口读取函数。返回的是串口数据中下一字节的内容
  // serialEvent(): 当串口有数据到达时调用该函数（然后使用Serial.read()捕获该数据）。
  Serial.begin(BAUDRATE);
  initEncoders();
  resetEncoders();
}

void loop() {
   long lval=readEncoder(0);
   long rval=readEncoder(1);
   Serial.print("left: ");
   Serial.print(lval);
   Serial.print("; right: ");
   Serial.println(rval);
   delay(30);
}