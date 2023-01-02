/*
   LINEAR CONTROL SYSTEMS FINAL PROJECT
   ------------------------------------
   ARDUINO-BASED WRITING MACHINE
   IUST , April 2022
   ------------------------------------
   Arvin Delavari 99411497
   Mohammad Arman Yazdi 99413271
   ------------------------------------
*/

//  Data communication protocol :
//  XXX;XXX;XXX;XXX;\n
//  SERIAL INPUT : X_INPUT;X_DIRECTION;Y_INPUT;Y_DIRECTION;\n

#include <String.h>

#define X_STEP 2
#define Y_STEP 3

#define X_DIR 5
#define Y_DIR 6

#define ENABLE 8

unsigned long rpm = 5000;
unsigned long int DELAY = (3*100000)/rpm;

void setup() 
{
  Serial.begin(9600);
  pinMode(X_STEP , OUTPUT);
  pinMode(X_DIR , OUTPUT);
  pinMode(Y_STEP , OUTPUT);
  pinMode(Y_DIR , OUTPUT);
  pinMode(ENABLE , OUTPUT);
  // digitalWrite(Y_DIR , HIGH);// HIGH = - , LOW = +
}

void loop() 
{
  String Input_Serial_Data;
  if (Serial.available())
    Input_Serial_Data = Serial.readStringUntil('\n');
  Serial.println(Input_Serial_Data);

  int Index_1 = Input_Serial_Data.indexOf(';');
  int Index_2 = Input_Serial_Data.indexOf(';', Index_1 + 1);
  int Index_3 = Input_Serial_Data.indexOf(';', Index_2 + 1);
  int Index_4 = Input_Serial_Data.indexOf(';', Index_3 + 1);

  String X_INPUT     = Input_Serial_Data.substring(0 , Index_1);
  String X_DIRECTION = Input_Serial_Data.substring(Index_1 + 1 , Index_2);
  String Y_INPUT     = Input_Serial_Data.substring(Index_2 + 1 , Index_3);
  String Y_DIRECTION = Input_Serial_Data.substring(Index_3 + 1 , Index_4);

  Serial.println(X_INPUT);
  Serial.println(X_DIRECTION);
  Serial.println(Y_INPUT);
  Serial.println(Y_DIRECTION);

  int input_X = X_INPUT.toInt();
  int input_Y = Y_INPUT.toInt();


  if (X_DIRECTION == "+")
  {
    Serial.println("X MOVEMENT +");
    digitalWrite(X_DIR , HIGH);// HIGH = + , LOW = -
    for (int i = 0 ; i <= input_X ; i++)
    {
      digitalWrite(X_STEP , HIGH);
      delayMicroseconds (DELAY);
      digitalWrite(X_STEP , LOW);
      delayMicroseconds (DELAY);
    }
  }
  if (X_DIRECTION == "-")
  {
    Serial.println("X MOVEMENT -");
    digitalWrite(X_DIR , LOW);// HIGH = + , LOW = -
    for (int i = 0 ; i <= input_X ; i++)
    {
      digitalWrite(X_STEP , HIGH);
      delayMicroseconds (DELAY);
      digitalWrite(X_STEP , LOW);
      delayMicroseconds (DELAY);
    }
  }
  input_X = 0;



  if (Y_DIRECTION == "+")
  {
    Serial.println("Y MOVEMENT +");
    digitalWrite(Y_DIR , LOW);// HIGH = - , LOW = +
    for (int j = 0 ; j <= input_Y ; j++)
    {
      digitalWrite(Y_STEP , HIGH);
      delayMicroseconds (DELAY);
      digitalWrite(Y_STEP , LOW);
      delayMicroseconds (DELAY);
    }
  }
  if (Y_DIRECTION == "-")
  {
    Serial.println("Y MOVEMENT -");
    digitalWrite(Y_DIR , HIGH);// HIGH = - , LOW = +
    for (int j = 0 ; j <= input_Y ; j++)
    {
      digitalWrite(Y_STEP , HIGH);
      delayMicroseconds (DELAY);
      digitalWrite(Y_STEP , LOW);
      delayMicroseconds (DELAY);
    }
  }
  input_Y = 0;

}
