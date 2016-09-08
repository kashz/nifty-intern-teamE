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

/******************************************************************************/
#define MAX_AP_NUM 2
/******************************************************************************/

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

char* SsidList[MAX_AP_NUM] = {"your-SSID-01","your-SSID-02"};    
char* PassList[MAX_AP_NUM] = {"your-PASS-01","your-PASS-02"};

char* apiKey = "your API Key";
int deviceId = ***;
String eventType = "ssid_hr";
//String eventType1 = "heart_rate";
//String eventType2 = "ssid";
String eventData;
//String eventData1;
//String eventData2;
DeviceHubClient deviceHubClient;

unsigned int prev_hr = 0;
unsigned int hr_sum = 0;
unsigned int hr_ave = 0;
unsigned int prev_hr_ave = 0;

unsigned int calc_count;
unsigned int ssid_num = 0;
unsigned int ssid_hr;

boolean send_flag = false;
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

  // 最初に接続するAPのSSIDとPASSを設定
  wifiSsid = SsidList[ssid_num];
  wifiPass = PassList[ssid_num];
  
  deviceHubClient.setModeInfo(deviceId, String(apiKey));  // 認証情報の登録

}
void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    // Wi-Fiに接続していないときの処理
    Serial.printf("Connecting to %s/%s...\r\n", wifiSsid, wifiPass);
    WiFi.begin(wifiSsid, wifiPass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      // APへの接続に失敗したときの処理
      
      if(ssid_num < MAX_AP_NUM - 1){
        // まだ接続していない
        ssid_num ++;
      }else{
        ssid_num = 0; 
      }
      wifiSsid = SsidList[ssid_num];
      wifiPass = PassList[ssid_num];
      
      Serial.print("current_ssid_is:\t");
      Serial.println(wifiSsid);
      arrayInit();
      
      return;
    }
    Serial.println("WiFi connected");
  }
  else {
    // Wi-Fiに接続しているときの処理
    //Serial.println("wifi connected");
    if(send_flag == true){
       deviceHubClient.triggerEvent(eventType, eventData);  // イベント送信
       /*
       deviceHubClient.triggerEvent(eventType1, eventData1);  
       deviceHubClient.triggerEvent(eventType2, eventData2);
       */
       Serial.printf("send!!");
       Serial.println(wifiSsid);
       send_flag = false;
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

    if(heart_rate != prev_hr){  // 心拍数が変化した場合
        prev_hr = heart_rate;       // 心拍数を更新
        hr_sum += heart_rate;     // 合計心拍数に加算
        calc_count ++;               // 計算回数をインクリメント
        
        if(calc_count >= 0){
            hr_ave = hr_sum / calc_count;   // 平均心拍数を計算
            hr_sum = 0;                             // 合計心拍数を0に戻す
            calc_count = 0;                         // 計算回数を0に戻す
            ssid_hr = (ssid_num+1)*1000 + hr_ave;   // 送信するデータを算出
            send_flag = true;
        }else{
            send_flag = false;  
        }
    }
    
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

    Serial.print("ssid_num_is:\t");
    Serial.println(ssid_num);

    Serial.print("ssid_hr_is:\t");
    Serial.println(ssid_hr);

    /*
    Serial.print("ssid_pos_is:\t");
    Serial.println(ssid_pos);
    */
    
    
    /*
    if(hr_ave != prev_hr_ave){
        send_flag = true;
        prev_hr_ave = hr_ave;
    }else{
        send_flag = false;  
    }
    */
    
    //eventData1 = "{\"value\": " + String(hr_ave) + "}";
    
    //ssid_pos = ssid_num+1;
    //eventData2 = "{\"value\": " + String(ssid_pos) + "}";

    //ssid_hr = (ssid_num+1)*1000 + hr_ave;
    
    eventData = "{\"value\": " + String(ssid_hr) + "}";
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
