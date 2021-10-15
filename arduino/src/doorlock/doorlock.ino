#include <Servo.h>

Servo myservo;
int lock_unlock = 0;

#define SERVO_PIN D7
#define SW_PIN D3

void setup() {
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(SERVO_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);
  myservo.write(90);



}

void loop() { 
  bool valSW = !digitalRead(SW_PIN);

  Serial.println(valSW);

  delay(100);
  if (valSW == 1) {
    lock_unlock = 1;
  }
  else {
    lock_unlock = 0;
  }

  if(lock_unlock == 1){
    
    myservo.write(180);

    delay(1000);
  }
  else {
    myservo.write(90);
  }
}
