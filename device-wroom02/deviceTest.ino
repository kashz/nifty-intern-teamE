#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <WebSocketsServer.h>

#include <ESP8266WiFi.h>
#include <DeviceHubClient.h>


/****************************************************************************/
//  Function: This program can be used to measure heart rate, the lowest
//        pulse in the program be set to 30.Use an external interrupt
//        to measure it.
//  Hardware: Grove - Ear-clip Heart Rate Sensor, Grove - Base Shield,
//        Grove - LED
//  Arduino IDE: Arduino-1.0
//  Author:  FrankieChu
//  Date:    Jan 22, 2013
//  Version: v1.0
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/******************************************************************************/
#define LED 4//indicator, Grove - LED is connected with D4 of Arduino
boolean led_state = LOW;//state of LED, each time an external interrupt
//will change the state of LED
unsigned char counter;
unsigned long temp[21];
unsigned long sub;
bool data_effect = true;
unsigned int heart_rate;//the measurement result of heart rate

const int max_heartpluse_duty = 2000;//you can change it follow your system's request.
//2000 meams 2 seconds. System return error
//if the duty overtrip 2 second.

/******************************************************************************/
char* wifiSsid ;
char* wifiPass ;

char* SsidList[2] = {"your-ssid-01","your-ssid-02"};
char* PassList[2] = {"your-pass-01","your-pass-02"};

char* apiKey = "***";
int deviceId = ***;
String eventType1 = "heart_rate";
String eventType2 = "ssid";
String eventData1;
String eventData2;
DeviceHubClient deviceHubClient;

unsigned int prev_hr = 0;
unsigned int hr_sum = 0;
unsigned int hr_ave = 0;
unsigned int prev_hr_ave = 0;
unsigned int calc_count;

unsigned int j = 0;
unsigned int ssid_num;

boolean flag = false;
/******************************************************************************/

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Please ready your chest belt.");
  delay(5000);
  arrayInit();
  Serial.println("Heart rate test begin.");
  attachInterrupt(0, interrupt, RISING);      //set interrupt 0,digital port 2

  wifiSsid = SsidList[j];
  wifiPass = PassList[j];

  /*
  for(j=0; j<2; j++){
      Serial.printf("j is %d___SsidList[%d] is %s\n",j,j,SsidList[j]);
  }
  j=0;
  */
  
  deviceHubClient.setModeInfo(deviceId, String(apiKey));  // 認証情報の登録

}
void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("Connecting to %s/%s...\r\n", wifiSsid, wifiPass);
    WiFi.begin(wifiSsid, wifiPass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      // APへの接続に失敗したときの処理
      if(j == 0){
        j ++;
      }else{
        j = 0;  
      }
      wifiSsid = SsidList[j];
      wifiPass = PassList[j];
      
      Serial.print("current_ssid_is:\t");
      Serial.println(wifiSsid);

      return;
    }
    Serial.println("WiFi connected");
  }
  else {
    // wi-fiに繋がっているときの処理
    //Serial.println("wifi connected");
    if(flag == true){
       deviceHubClient.triggerEvent(eventType1, eventData1);  // イベント送信
       deviceHubClient.triggerEvent(eventType2, eventData2);
       Serial.printf("send!!");

       Serial.println(wifiSsid);
       flag = false;
    }
  }
     
  digitalWrite(LED, led_state);//Update the state of the indicator
  delay(100);
}

/*Function: calculate the heart rate*/
void sum()
{
  if (data_effect)
  {
    heart_rate = 1200000 / (temp[20] - temp[0]); //60*20*1000/20_total_time
    
    Serial.print("Heart_rate_is:\t");
    Serial.println(heart_rate);
    Serial.print("Prev_Heart_rate_is:\t");
    Serial.println(prev_hr);

    Serial.print("Heart_rate_Sum_is:\t");
    Serial.println(hr_sum);
    Serial.print("Heart_rate_Average_is:\t");
    Serial.println(hr_ave);

    Serial.print("calc_count_is:\t");
    Serial.println(calc_count);

    Serial.print("j_is:\t");
    Serial.println(j);
    
    if(heart_rate != prev_hr){  // 心拍数が変化した場合
        //flag = true;
        prev_hr = heart_rate;
        hr_sum += heart_rate;
        calc_count ++;
        
        if(calc_count >= 0){
            hr_ave = hr_sum / calc_count;
            hr_sum = 0; 
            calc_count = 0;
            flag = true;
        }else{
            flag = false;  
        }
    }else{
        //flag = false;
    }
    /*
    if(hr_ave != prev_hr_ave){
        flag = true;
        prev_hr_ave = hr_ave;
    }else{
        flag = false;  
    }
    */
    
    eventData1 = "{\"value\": " + String(hr_ave) + "}";

    ssid_num = j+1;
    eventData2 = "{\"value\": " + String(ssid_num) + "}";
    
    //eventData = "{\"value\": " + String(wifiSsid) + "}";
    //eventData = "{\"value\": " + String(hr_ave) + " : "+ String(wifiSsid) + "}";
  }
  data_effect = 1; //sign bit
}
/*Function: Interrupt service routine.Get the sigal from the external interrupt*/
void interrupt()
{
  temp[counter] = millis();
  Serial.println(counter, DEC);
  //Serial.println(temp[counter]);
  switch (counter)
  {
    case 0:
      sub = temp[counter] - temp[20];
      //Serial.println(sub);
      break;
    default:
      sub = temp[counter] - temp[counter - 1];
      //Serial.println(sub);
      break;
  }
  if (sub > max_heartpluse_duty) //set 2 seconds as max heart pluse duty
  {
    data_effect = 0; //sign bit
    counter = 0;
    Serial.println("Heart rate measure error,test will restart!" );
    arrayInit();
  }
  if (counter == 20 && data_effect)
  {
    counter = 0;
    sum();
  }
  else if (counter != 20 && data_effect)
    counter++;
  else
  {
    counter = 0;
    data_effect = 1;
  }

}
/*Function: Initialization for the array(temp)*/
void arrayInit()
{
  for (unsigned char i = 0; i < 20; i ++)
  {
    temp[i] = 0;
  }
  temp[20] = millis();
}
