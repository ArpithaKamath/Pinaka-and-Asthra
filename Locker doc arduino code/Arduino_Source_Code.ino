//Android-controlled Arduino Bluetooth Servo Door Lock by FrenchToastPhilip a.k.a. THE SEXIEST MAN ALIVE
// Modified from initial code to account for unexpect EEPROM values
#include <Servo.h>
#include <EEPROM.h>

Servo servo;
char state;

void setup() {
  // put your setup code here, to run once:
  servo.attach(7);

  if(EEPROM.read(0) == 1) // Reads the EEPROM value stored to know what state the door lock was in before it was last turned off
  {                       // An EEPROM value of 1 means UNLOCKED and a value of 2 means LOCKED
    servo.write(70); // Rotates the servo to the unlocked position
    delay(200);
  }
  else if(EEPROM.read(0) == 2)
  {
    servo.write(120); // Rotates the servo to the locked position
    delay(200);
  }
  // If EEPROM doesn't have expected values, initialize it
  else
  {
    EEPROM.write(0, 1);
    servo.write(70);
    delay(200);
  }
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char data = '0';
    data = Serial.read(); // The variable data is used to store the value sent by the Android app

   /* if(data == '1')
    {
      if(EEPROM.read(0) != 0)
      {
        servo.write(70);
        delay(500);
        servo.write(120);
        delay(500);
      }
    } */

    switch(data)
    {
      case '1': 
        if(EEPROM.read(0) == 1) //An EEPROM value of 1 means it is currently unlocked
        {
          EEPROM.write(0, 2); // Writes the number 2 to address 0 on the Arduino's EEPROM. This value will be used by the Arduino to remember the last state the door lock was in
          Serial.print("3"); // Sends the number 3 to the Android app. To see what this does, please see the Android Studio Project file

          for(int a = 70; a <= 120; a++) // Rotates the servo to the locked position
          {
            servo.write(a);
            delay(15);
            Serial.println(servo.read());
          }
        }
        else if(EEPROM.read(0) == 2) //An EEPROM value of 2 means it i currently locked
        {
          EEPROM.write(0, 1); // Writes the number 1 to address 0 on the Arduino's EEPROM. This value will be used by the Arduino to remember the last state the door lock was in
          Serial.print("4"); // Sends the number 4 to the Android app. The number sent will be used by the app to update the locked/unlocked icon
          for(int x = 120; x >= 70; x--) // Rotates the servo to the unlocked position
          {
            servo.write(x);
            delay(15);
          }
        }
        break;
      case '3': 
      // This part of the switch case statement is used everytime the Arduino is turned on
      // Basically, the Android app sends a command asking the Arduino to send the current state of the lock in order to accordingly update the locked/unlocked icon in the app
        if(EEPROM.read(0) == 1)
        {
          Serial.print("4");
        }
        else if(EEPROM.read(0) == 2)
        {
          Serial.print("3");
        }
        break;
    }
  }
  
}
