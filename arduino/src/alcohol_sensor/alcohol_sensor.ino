#include "FirebaseESP8266.h" 
#include <ESP8266WiFi.h>
#include <Servo.h>
 
#define FIREBASE_HOST "teamplay1-bf560-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "M5rOAk2s7m1Qrcy2LL8zWqvuBKWjNUe0VZLddRrf"
#define WIFI_SSID "SK_WiFiGIGAB488" // 연결 가능한 wifi의 ssid
#define WIFI_PASSWORD "1907001138" // wifi 비밀번호


#define SERVO_PIN D8
#define pinAl A0
#define red D13
#define green D12
#define blue D11
#define button D3
#define buzzer D2

#define NOTE_F5 261
#define NOTE_F6 349

Servo myservo;
FirebaseData firebaseData;
FirebaseJson json;

bool lState = false;
bool cState = false;
int sign=2;


float ppm = 0; //버튼을 누르는 동안 측정된 값의 평균을 내기 위해 필요한 변수
int count = 0;

void printResult(FirebaseData &data);

void setup() {
  Serial.begin(9600);

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


  
  pinMode(pinAl,INPUT);
  
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

  pinMode(button,INPUT_PULLUP);

  pinMode(SERVO_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);
  myservo.write(90);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop() {
  int valal=analogRead(pinAl);
  
  delay(100);
  
  cState=digitalRead(button);
  delay(20);

  
   if(cState==0){
   ppm+=valal;
   count++;
  }
   else if((cState==1)&&(cState!=lState)){
   ppm/=count;
   Serial.println(ppm);
   
  if(valal>=100&& valal<=400){
    digitalWrite(blue,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    Firebase.setInt(firebaseData,"/alcohol_check/LED",1);
    
    
    myservo.write(180);
    delay(3000);
    
    }
    else if(valal>400 && valal<=650){
    digitalWrite(blue,LOW);
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
    Firebase.setInt(firebaseData,"/alcohol_check/LED",2);

     while(1){ //BUZZER 작동
        tone(buzzer,NOTE_F5);
        delay(800);
        noTone(buzzer);
        delay(800);
        Firebase.getInt(firebaseData,"game3_gesture/LOCK_UNLOCK");
        sign=firebaseData.intData();
        if(sign==1){
          myservo.write(180);
          break;}
        }
      }
    else if(valal>650){
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
      digitalWrite(red,HIGH);
     Firebase.setInt(firebaseData,"/alcohol_check/LED",3);
      for(int i=0; i<10;i++){ //BUZZER 작동
        tone(buzzer,NOTE_F6);
        delay(800);
        noTone(buzzer);
        delay(800);
        }
      }
   }
/*   Firebase.setInt(firebaseData,"ppm",ppm);
//   Firebase.getInt(firebaseData,"test");
    Firebase.getInt(firebaseData,"ppm");
//   firebaseData.getValue("ppm");
   // Serial.println();
     Serial.println(firebaseData.intData());*/
    
 
    ppm=0;
    count=0;
    
   lState=cState;
   
  delay(500);
}
