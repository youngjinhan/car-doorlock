#include "FirebaseESP8266.h" // 반드시 파이어베이스 라이브러리를 먼저 추가해야 합니다.
#include <ESP8266WiFi.h>
 
#define FIREBASE_HOST "teamplay1-bf560-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "M5rOAk2s7m1Qrcy2LL8zWqvuBKWjNUe0VZLddRrf"
#define WIFI_SSID "Oasis" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "54188797a" // wifi 비밀번호

#define pinSW1  D2
#define pinSW2  D3
#define pinLED1 D4
#define pinLED2 D7
#define pinCDS  A0
#define sensor  D8
#define buzzer  D5
int val = 0;

FirebaseData firebaseData;
FirebaseJson json;
 
void printResult(FirebaseData &data);
void setup() // wifi 접속 과정.
{
  Serial.begin(9600);
 
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop()
{
  val = digitalRead(sensor);
  if (val == HIGH) {
    Serial.println("Hi");
    tone(buzzer, 261.6);
    delay(50); 
    noTone(buzzer);
    delay(50);
  }
  else{
    Serial.println("Bye");
    delay(50);
  }

  Firebase.setBool(firebaseData, "IR_sensor/Sensor", digitalRead(sensor));
  digitalWrite(buzzer,firebaseData.boolData());
  
  delay(200); 
}
