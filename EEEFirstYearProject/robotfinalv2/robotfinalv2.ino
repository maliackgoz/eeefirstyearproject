#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "MeMCore.h"


MeDCMotor motorL(M1);
MeDCMotor motorR(M2);
MeUltrasonicSensor ultraSensor(PORT_3);

int eski_hareket_yonu = -1;
int hareket_yonu = -1;

int const MAX_MESSAGE_LENGTH = 1000;
static char message[MAX_MESSAGE_LENGTH];

// move forward method
void move_forward(int speed){
  hareket_yonu = 1;
  if (hareket_yonu != eski_hareket_yonu){
for(int anlik_hiz = 0; anlik_hiz < speed; anlik_hiz++){
    motorL.run(-anlik_hiz);
    motorR.run(anlik_hiz);
    // will optimize
    delay(2);
    }
  }
}

// turn left method
void turn_left(int speed){
  hareket_yonu = 2;
  if (hareket_yonu != eski_hareket_yonu){
for(int anlik_hiz = 0; anlik_hiz < speed; anlik_hiz++){
    motorL.run(0);
    motorR.run(anlik_hiz);
    delay(2);
    }
    delay(150000/speed);
    motorL.stop();
    motorR.stop();
  }
}

// turn right method
void turn_right(int speed){
  hareket_yonu = 3;
  if (hareket_yonu != eski_hareket_yonu){
for(int anlik_hiz = 0; anlik_hiz < speed; anlik_hiz++){
    motorL.run(-anlik_hiz);
    motorR.run(0);
    delay(2);
    }
    delay(150000/speed);
    motorL.stop();
    motorR.stop();
  }
}

// bus stop method
void bus_stop(){
  hareket_yonu = 0;
  if (hareket_yonu != eski_hareket_yonu){
    motorL.stop();
    motorR.stop();
    delay(30000);
  }
}

// stop method
void motor_stop(){
  hareket_yonu = 0;
  if (hareket_yonu != eski_hareket_yonu){
    motorL.stop();
    motorR.stop();
  }
}

// stop method with acc
void motor_stop(int speed){
  hareket_yonu = 0;
  if (hareket_yonu != eski_hareket_yonu){
    for(int anlik_hiz = speed; anlik_hiz > 0; anlik_hiz--){
    motorL.run(anlik_hiz);
    motorR.run(anlik_hiz);
    // we can set delay
    delay(2);
    }
    motorL.stop();
    motorR.stop();
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  float distance = ultraSensor.distanceCm();
  Serial.print("Distance: ");
  Serial.print(ultraSensor.distanceCm());
  Serial.println(" cm");
  delay(40);
  
    while (Serial.available() > 0){
   //Create a place to hold the incoming message
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();

   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) ){
     //Add the incoming byte to our message
     message[message_pos] = inByte;
     message_pos++;
   }
   
   //Full message received...
   
   else{
     //Add null character to string
     message[message_pos] = '\0';

     //Print the message (or do other things)
     Serial.println(message);

     //Reset for the next message
     message_pos = 0;
   }
 }

 if(String(message).equals("0") && distance < 8){
    motorL.stop();
    motorR.stop();
  }
  else if (String(message).equals("1")){
    delay(3000);
    bus_stop();
  }
  else if (String(message).equals("2") && distance < 8){
    turn_left(70);
  }
  else if (String(message).equals("3") && distance < 8){
    turn_right(70);
  }
  else{
    move_forward(90);
  }
  message[0] = '\0';
  eski_hareket_yonu = hareket_yonu;
  }
