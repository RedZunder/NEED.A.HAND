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
 // pinMode(ANALOG_IN, INPUT);
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
const unsigned long servo_timer=7-SERVO_SPEED%5, led_timer=150;
unsigned long led_before=0, servo_before=0;

//uint16_t analog_read=0;   //value in Volts



//USING MILLIS INSTEAD OF DELAY TO MAKE IT NON BLOCKING
void loop() 
{
//----------------------SIGNAL PROCESSING--------------------------------

 // analog_read=analogRead(ANALOG_IN)*5/1023; //1023 reading represents 5V

//----------------------END OF SIGNAL PROCESSING-------------------------




//----------------------BLUETOOTH SETUP--------------------------------

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
    if(ble.available())          //Read from HC
    {
      response=ble.readString();
      Serial.println(response);
    }

      if(response[0]=='A')   //ROCK-N-ROLL
      {
        change_finger_step(1,fing1);
        change_finger_step(0,fing2);
        change_finger_step(0,fing3);
        change_finger_step(1,fing4);
        Serial.println("ROCK N ROLL");
      }
      else if(response[0]=='B')
      {
        change_finger_step(0,fing1);
        change_finger_step(1,fing2);
        change_finger_step(1,fing3);
        change_finger_step(0,fing4);
        Serial.println("NOT ROCK");

      
      servo_before=millis();
    }

  }
/*
Serial.println("S1:");
Serial.print(fing1.read());
Serial.print("  S2:");
Serial.print(fing2.read());
Serial.print("  S3:");
Serial.print(fing3.read());
Serial.println("");
*/
//----------------------END OF SERVOS MOVEMENT-------------------------


  if(millis()-led_before>=led_timer)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    led_before=millis();
  }


}










//-----------------------------------------HELPER FUNCTIONS-----------------------------------------
//--------------------------------------------------------------------------------------------------


//LOWER VALUE OF SERVO MEANS NERVE IS "ACTIVATED", GREATER MEANS NERVE IS "RELAXED" - BLOCKING FUNCTION [[DEPRECATED]]
/*
uint8_t change_finger(bool extend, Servo finger)
{

  Serial.print("  Flex:");
  Serial.println(finger.read());

   if(extend)
  {
    while(finger.read() < SERVO_MAX)
    {
      finger.write(finger.read()+1);
      delay(1);
    }
  }
  else
  { 
    while(finger.read() > SERVO_MIN)
    {
      finger.write(finger.read()-1);
      delay(1);
    }
  }

  
  //finger.write(extend?SERVO_MAX-1:SERVO_MIN+1);
return 0;
}*/



//LOWER VALUE OF SERVO MEANS NERVE IS "ACTIVATED", GREATER MEANS NERVE IS "RELAXED" - NONBLOCKING FUNCTION
/*
* @param    extend :    0 for contracting the finger, 1 for extending it
* @param    finger :    Servo structure
*/
void change_finger_step(bool extend, Servo finger)
{
  if(extend && finger.read() < SERVO_MAX)
  {
    finger.write(finger.read()+1);    
  }
  else if(!extend && finger.read() > SERVO_MIN)
  { 
    finger.write(finger.read()-1);
  }
  else
    return;
}






