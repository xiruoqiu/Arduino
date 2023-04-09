#define BLINKER_WIFI
#include <Blinker.h>
#include <ESP8266WiFi.h>          
#include <DNSServer.h>

#define relay D0 //继电器的引脚

        
volatile float hum = 0;
volatile float tem = 0;
volatile float soil = 0;
volatile float sun = 0;

float data1=0;//要接收的数据，温度
float data2=0;//要接收的数据，空气湿度
float data3=0;//要接收的数据，土壤湿度
float data4=0;//要接收的数据，阳光相对值

char auth[] = "6cebcc402a7c";
char ssid[] = "xiruoqiu";
char pswd[] = "12345678";
BlinkerNumber Number1("num-humi");
BlinkerNumber Number2("num-temp");
BlinkerNumber Number3("num-soil");
BlinkerNumber Number4("num-sun");

void dataStorage()
{
   Blinker.dataStorage("num-humi", hum);
   Blinker.dataStorage("num-temp", tem);
   Blinker.dataStorage("num-soil", soil);
   Blinker.dataStorage("num-sun",sun);
}

BlinkerText Text1("tex-it2");
BlinkerText Text2("tex-it3");
BlinkerText Text3("tex-it4");
BlinkerButton Button1("btn-water");

void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(relay,HIGH);
    //delay(15);
    //Text2.print("正在浇水");
    delay(10000);
    digitalWrite(relay,LOW);
    //Text2.print("浇水完成");
    //delay(2000);
}

void setup()
{
  Serial.begin(9600);
  Blinker.begin(auth, ssid, pswd); 
  Button1.attach(button1_callback);
  pinMode(relay,OUTPUT);//设置继电器的引脚为output

}

void loop()
{

  receive();//接收数据
  Serial.println(data1);//显示接收到的数据
  Serial.println(data2);//显示接收到的数据
  Serial.println(data3);//显示接收到的数据
  Serial.println(data4);//显示接收到的数据
  tem = data1;
  hum = data2;
  soil = data3;
  sun = data4;

  Number1.print(hum);
  Number2.print(tem);
  Number3.print(soil);
  Number4.print(sun);

    if (soil > 700) 
    {
    Text1.print("植物缺水");
    }
  
   else if (soil <=700&& soil>=350) 
   {
    Text1.print("植物土壤湿度正常");
   }
   else if(soil<=200)
   {
     Text1.print("浇水过多");
   }

    if (sun > 1000) 
    {
    Text3.print("植物现在处于无光环境");
    }
  
   else if (sun <=1000&& sun>=200) 
   {
    Text3.print("植物光照正常");
   }
   else if(sun<=200)
   {
     Text3.print("植物处于强光环境");
   }

   if(digitalRead(relay)==HIGH)
   {
    //delay(15);
    Text2.print("正在浇水");
   }
   else if(digitalRead(relay)==LOW)
   {
     //delay(15);
     Text2.print("浇水完成");

   }

Blinker.run();

}

void receive(){

  String comdata = "";//存放接收到的原始字符串
  String buf1="";//存放解析出的第一个字符串
  String buf2="";//存放解析出的第二个字符串
  String buf3="";//存放解析出的第三个字符串
  String buf4="";
    while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);//一个一个接收数据，每次停顿2毫秒
    }
    if (comdata.length() > 0)//如果有数据收到
    {
      int ja,jb,jc,jd,je;//存放收到的字符串中标志字母的下标

for(int i = 0; i < comdata.length(); i++)
{

  if(comdata[i]=='a'){ja=i;}
  if(comdata[i]=='b'){jb=i;}
  if(comdata[i]=='c'){jc=i;}
  if(comdata[i]=='d'){jd=i;}
  if(comdata[i]=='#'){je=i;}

  }//找到约定标志字母的下标

 for(int k=ja+1;k<jb;k++){
  buf1+=char(comdata[k]);
  }
  for(int k=jb+1;k<jc;k++){
  buf2+=char(comdata[k]);
  }
  for(int k=jc+1;k<jd;k++){
  buf3+=char(comdata[k]);
  }
  for(int k=jd+1;k<je;k++){
  buf4+=char(comdata[k]);

  }//根据约定字母所在的位置依次解析出各个数据
  data1=buf1.toFloat();//将解析得到的字符串转换为float类型
  data2=buf2.toFloat();
  data3=buf3.toFloat();
  data4=buf4.toFloat();

    }

  }
