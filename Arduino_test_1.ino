#include<Servo.h>//舵机头文件
#include<DFRobot_DHT11.h>//温湿度头文件
#include<SoftwareSerial.h>


SoftwareSerial mySerial(8,9);//RX,TX

int b=0;
int b1=0;
char c=0;
char c1=0;

//温湿度模块
DFRobot_DHT11 DHT;
#define DHT11_PIN 6
//舵机模块
Servo myservo;

int pos = 0;//舵机的旋转角度
//土壤传感器
#define Moisture A3     //定义A0引脚为A3
#define D0 7            //定义D0引脚为7
//继电器模块
#define relay 4

//挡光板是否旋转，默认为未旋转
int turnaround = 0;

//光敏电阻
int sunBarrier = 0;

//是否需要浇水
int needWater = 0;

float Temp = 0;//温度
float airHumi = 0;//湿度

float soilHumidity = 0;//土壤湿度

 float data1=0;//要发送的数据,温度
 float data2=0;//要发送的数据，空气湿度
 float data3=0;//要发送的数据，土壤湿度
 float data4=0;//要发送的数据，阳光相对值
 float sunindex;//热指数

void setup() 
{
  mySerial.begin(115200);
pinMode(relay,OUTPUT);//设置继电器的引脚为output
//digitalWrite(relay,LOW);

Serial.begin(9600);

//初始化土壤湿度模块
pinMode(Moisture,INPUT);
pinMode(D0,INPUT);

}




void loop() 
{
DHT.read(DHT11_PIN);
myservo.attach(5);//舵机初始化
//阳光
sunBarrier = analogRead(A0);



soilHumidity = analogRead(Moisture);

if(soilHumidity>=800)
{
  needWater = 1;
}

airHumi = DHT.humidity;
//温度数据
Temp = DHT.temperature;

//Serial.println(airHumi);
//Serial.println(Temp);

data1 = Temp;
data2 = airHumi;
data3 = soilHumidity;
data4 = sunBarrier;

sunindex = Temp+(airHumi*0.1);

transmit();//发送数据

sendEverything();


//缺水响应，控制继电器，水泵抽水
if(needWater)
{
digitalWrite(relay,HIGH);
delay(10000);
digitalWrite(relay,LOW);
delay(1000);
needWater = 0;
}

//光照过强，控制舵机进行遮阳处理
if(sunBarrier<=200)//当光敏电阻检测为强光
{
  for(pos;pos<=90;pos++)
  {
    myservo.write(pos);
    delay(15);
  }
turnaround = 1;//已经旋转了
}
//当光照比较弱时，自动旋转回来
if(sunBarrier>=200 && turnaround == 1)
{
  for(pos;pos>=0;pos--)
  {
    myservo.write(pos);
    delay(15);
  }
turnaround = 0;//设置为未旋转状态
}


}

void transmit(){


        Serial.print('a');
        Serial.print(data1);
        Serial.print('b');
        Serial.print(data2);
        Serial.print('c');
        Serial.print(data3);
        Serial.print('d');
        Serial.print(data4);
        Serial.println('#');
        delay(100);
        //发送"a数据1b数据2c数据3#"的字符串必须停顿100毫秒

  }

void SendtoVGUS(int x)//转变数据变量地址 格式
  {

  int v1=x;
  b=v1/256;
  b1=v1 %256;
  c=b;
  c1=b1;
  }

    void sendEverything()
  {
    SendtoVGUS(sunBarrier);
    mySerial.write(0x5A);
    mySerial.write(0xA5);
    mySerial.write(0x05);
    mySerial.write(0x82);
    mySerial.write(0x10);
    mySerial.write(0x10);
    mySerial.write(c);
    mySerial.write(c1);

    SendtoVGUS(Temp);
    mySerial.write(0x5A);
    mySerial.write(0xA5);
    mySerial.write(0x05);
    mySerial.write(0x82);
    mySerial.write(0x10);
    mySerial.write(0x20);
    mySerial.write(c);
    mySerial.write(c1);

    SendtoVGUS(airHumi);
    mySerial.write(0x5A);
    mySerial.write(0xA5);
    mySerial.write(0x05);
    mySerial.write(0x82);
    mySerial.write(0x10);
    mySerial.write(0x30);
    mySerial.write(c);
    mySerial.write(c1);

    SendtoVGUS(sunindex);
    mySerial.write(0x5A);
    mySerial.write(0xA5);
    mySerial.write(0x05);
    mySerial.write(0x82);
    mySerial.write(0x10);
    mySerial.write(0x40);
    mySerial.write(c);
    mySerial.write(c1);

    SendtoVGUS(soilHumidity);
    mySerial.write(0x5A);
    mySerial.write(0xA5);
    mySerial.write(0x05);
    mySerial.write(0x82);
    mySerial.write(0x10);
    mySerial.write(0x50);
    mySerial.write(c);
    mySerial.write(c1);
  
  }
