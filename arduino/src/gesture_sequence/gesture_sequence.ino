#include "FirebaseESP8266.h" 
#include <ESP8266WiFi.h>
#include <Arduino_APDS9960.h>
 
#define FIREBASE_HOST "teamplay1-bf560-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "M5rOAk2s7m1Qrcy2LL8zWqvuBKWjNUe0VZLddRrf"
#define WIFI_SSID "dlink-940C" 
#define WIFI_PASSWORD "cnvft88853" 

int seq_1 = 0;
int seq_2 = 0;
int seq_3 = 0;
int seq_4 = 0;
int count = 0;
int a;
int b;
int c;
int d;
int gesture;
int ges[4];
int _loop = 0;



FirebaseData firebaseData;
FirebaseJson json;
 
void printResult(FirebaseData &data);
void setup() // wifi 접속 과정.
{
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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  
  Firebase.setInt(firebaseData, "game3_gesture/SEQUENCE/1st", seq_1);
  Firebase.setInt(firebaseData, "game3_gesture/SEQUENCE/2nd", seq_2);
  Firebase.setInt(firebaseData, "game3_gesture/SEQUENCE/3rd", seq_3);
  Firebase.setInt(firebaseData, "game3_gesture/SEQUENCE/4th", seq_4);
  Firebase.setInt(firebaseData, "game3_gesture/LOCK_UNLOCK", -1);
  if (!APDS.begin()){
    Serial.println("APDS9960 ERROR!");
    while(1);
  }
  else {
    Serial.println("APDS9960 ON!");
  }
}

void loop()
{
      if (APDS.gestureAvailable()){
        Serial.println("DETECTED");
        gesture = APDS.readGesture();
        switch (gesture) {
          case GESTURE_UP: Serial.println("up"); delay(1000); break;
          case GESTURE_DOWN: Serial.println("down"); delay(1000); break;
          case GESTURE_LEFT: Serial.println("left"); delay(1000); break;
          case GESTURE_RIGHT: Serial.println("right"); delay(1000); break;
          default: break;
        }
        Firebase.setInt(firebaseData, "game3_gesture/GESTURE", gesture + 1);
        count = count + 1;
        Serial.println(count);
        Serial.println(gesture + 1);

      }

      if (count == 1){
        ges[0] = gesture + 1;
      }
      if (count == 2){
        ges[1] = gesture + 1;
      }
      if (count == 3){
        ges[2] = gesture + 1;
      }
      if (count == 4){
        ges[3] = gesture + 1;
      }
    



      if (count == 4) {
        Firebase.getString(firebaseData, "game3_gesture/SEQUENCE/1st");
        String sequence_1st=firebaseData.stringData();
        Firebase.getString(firebaseData, "game3_gesture/SEQUENCE/2nd");
        String sequence_2nd=firebaseData.stringData();
        Firebase.getString(firebaseData, "game3_gesture/SEQUENCE/3rd");
        String sequence_3rd=firebaseData.stringData();
        Firebase.getString(firebaseData, "game3_gesture/SEQUENCE/4th");
        String sequence_4th=firebaseData.stringData();

        a = sequence_1st.toInt();
        b = sequence_2nd.toInt();
        c = sequence_3rd.toInt();
        d = sequence_4th.toInt();

        Serial.println(a);
        Serial.println(b);
        Serial.println(c);
        Serial.println(d);

        Serial.println(ges[0]);
        Serial.println(ges[1]);
        Serial.println(ges[2]);
        Serial.println(ges[3]);
        

        if (ges[0] == a && ges[1] == b && ges[2] == c && ges[3] == d){
                Serial.println("Success");
                Firebase.setInt(firebaseData, "game3_gesture/LOCK_UNLOCK", 1);
        }
        else {
           Firebase.setInt(firebaseData, "game3_gesture/LOCK_UNLOCK", 0); 
        }
        delay(300);
        Firebase.setInt(firebaseData, "game3_gesture/LOCK_UNLOCK", -1);   
        count=0;   
      }

    //Serial.println(firebaseData.); 
}
