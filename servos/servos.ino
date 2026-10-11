#include <Servo.h>
#include <SoftwareSerial.h>



#define SERVO_MIN 1
#define SERVO_MAX 179
#define SERVO_SPEED 4   //FROM 1 TO 4

//#define ANALOG_IN A1    //Where the analog signal is measured
#define USE_BLE       //------------------------------Uncomment when using HC-08 Bluetooth module

/*CAREFUL WHEN CHANGING - BLE MODULE MUST HAVE CORRECT BAUD BEFORE CHANGING THIS BAUD RATE*/
#define BAUD 115200  

#define SERVO_1 8
#define SERVO_2 9
#define SERVO_3 10
#define SERVO_4 11

#ifdef USE_BLE
  #define BLE_RX 6      //------------------------------Connected to HC-08's TX
  #define BLE_TX 7      //------------------------------Connected to HC-08's RX

  SoftwareSerial ble(BLE_RX,BLE_TX);
#endif




Servo fing1, fing2, fing3, fing4;     //------------------------------Servo for flexing the finger



void setup() {

  Serial.begin(BAUD);
#ifdef USE_BLE
  ble.begin(BAUD);  
#endif


if(SERVO_SPEED>5 || SERVO_SPEED<1)
{
  Serial.println("ERROR WITH SERVO_SPEED");
  while(1){};
}


#ifdef USE_BLE
  pinMode(BLE_RX,INPUT);
  pinMode(BLE_TX,OUTPUT);
#endif
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println("....");


  fing1.attach(SERVO_1);
  fing2.attach(SERVO_2);
  fing3.attach(SERVO_3);
  fing4.attach(SERVO_4);

  delay(100);

  fing1.write(2);
  fing2.write(2);
  fing3.write(2);
  fing4.write(2);


  delay(2000);
  Serial.println("READY");

}








String response="", chitchat;
const unsigned long servo_timer=7-SERVO_SPEED%5, led_timer=150,servo_stop=10000;
unsigned long led_before=0, servo_before=0, servo_cooldown=0;
bool servo_done=false;



//USING MILLIS INSTEAD OF DELAY TO MAKE IT NON BLOCKING
void loop() 
{

//----------------------BLUETOOTH SETUP--------------------------------
//Uncomment when needing to change HC-08 settings
/*
  if(Serial.available()) 
  {
    chitchat=Serial.readString();// read the incoming data as string
    Serial.println(chitchat);
    ble.write(chitchat.c_str());
  }
*/

//----------------------END OF BLUETOOTH SETUP-------------------------



//----------------------SERVOS MOVEMENT--------------------------------

  if(millis()-servo_before>=servo_timer)
  {
    if(ble.available())                     //Read from HC
    {
      response=ble.readString();
      Serial.println(response);
    }

      switch(response[0])
      {
        case 'A':
          set_hand_gesture_step(1,0,0,1);   //rock n roll
        break;
        case 'B':
          set_hand_gesture_step(0,1,1,0);   //anti rock
        break;
        case 'C':
          set_hand_gesture_step(0,1,0,0);   //fuck u
        break;
        case 'D':
          set_hand_gesture_step(0,0,0,1);   //surf
        break;
        case 'E':
          set_hand_gesture_step(0,0,0,0);   //fist
        break;

        //Add more hand
        //gestures here...

        default:
          set_hand_gesture_step(1,1,1,1);   //relaxed
          response="";
        break;
      }
      
      servo_before=millis();

      if(servo_done && millis()-servo_cooldown>=servo_stop)
      {
          set_hand_gesture_step(1,1,1,1);   //relaxed
          servo_done=false;
      }
    }

  


//----------------------END OF SERVOS MOVEMENT-------------------------


  if(millis()-led_before>=led_timer)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    led_before=millis();
  }


}










//-----------------------------------------HELPER FUNCTIONS-----------------------------------------
//--------------------------------------------------------------------------------------------------


//LOWER VALUE OF SERVO MEANS NERVE IS "ACTIVATED", GREATER MEANS NERVE IS "RELAXED" - NONBLOCKING FUNCTION
/*
* @brief                Change angle of the servo, without getting out of range, by one step
* @param    extend :    0 for contracting the finger, 1 for extending it
* @param    finger :    Servo structure
*/
void change_finger_step(bool extend, Servo finger)
{
  if(extend && finger.read() < SERVO_MAX)
    finger.write(finger.read()+1);    
  else if(!extend && finger.read() > SERVO_MIN)
    finger.write(finger.read()-1);
  else
  {
    servo_done=true;
    servo_cooldown=millis();
  }
}


//FINGERS FROM 1 TO 4, FROM INDEX TO PINKY
/*
* @brief                Set all fingers for the hand, by one step
* @param    f1,f2...:   Bool value; 0 for contracted, 1 for extended
*/
void set_hand_gesture_step(bool f1, bool f2, bool f3 , bool f4)
{
  change_finger_step(f1,fing1);
  change_finger_step(f2,fing2);
  change_finger_step(f3,fing3);
  change_finger_step(f4,fing4);
}



