#include <IRremote.h>

#define rightMotor 10
#define leftMotor 11
#define rightDirection 12 // Right Motor: HIGH = Backwards, LOW = Forward
#define leftDirection 13  // Left Motor : HIGH = Forward  , LOW = Backwards
#define IR_PIN 5
#define light 6
#define buzzer 4
#define loaded 2700
#define disabled 2705
#define reset 7

IRrecv irrecv(IR_PIN);
IRsend irsend;
decode_results results;

const int BTState = 2;
int state = 0;
int vSpeed = 255; // Default speed, from 0 to 255
int vSpeedturn = 150;
bool armed = 0;
bool paralysed = 0;

void setup()
{
  // Defining rightDirection's Pin as Output
  pinMode(rightDirection, OUTPUT);
  // Defining leftDirection's Pin as Output
  pinMode(leftDirection, OUTPUT);
  // Defining light's Pin as Output
  pinMode(light, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(reset, INPUT);
  digitalWrite(reset, LOW);

  // Initialize serial communication at 9600 bits per second:
  // Serial.begin(9600);
  // irrecv.enableIRIn();
  // Serial.println("IR enabled");

  digitalWrite(buzzer, HIGH);
  delay(5000);
  digitalWrite(buzzer, LOW);
}

void loop()
{
  //Stop car when connection lost or bluetooth disconnected
  if (digitalRead(BTState) == LOW)
  {
    state = 'S';
  }

  //Save incoming data to variable 'state'
  if (Serial.available())
  {
    state = Serial.read();
  }
  /***********************Forward******************************/
  //If state is equal with letter 'F', car will go forward!
  if (state == 'F')
  {
    digitalWrite(rightDirection, HIGH);
    digitalWrite(leftDirection, LOW);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeed);
  }
  /**********************Forward Left**************************/
  //If state is equal with letter 'G', car will go forward left
  else if (state == 'G')
  {
    digitalWrite(rightDirection, HIGH);
    digitalWrite(leftDirection, LOW);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeedturn);
  }
  /**********************Forward Right*************************/
  //If state is equal with letter 'I', car will go forward right
  else if (state == 'I')
  {
    digitalWrite(rightDirection, HIGH);
    digitalWrite(leftDirection, LOW);
    analogWrite(leftMotor, vSpeedturn);
    analogWrite(rightMotor, vSpeed);
  }
  /***********************Backward*****************************/
  //If state is equal with letter 'B', car will go backward
  else if (state == 'B')
  {
    digitalWrite(rightDirection, LOW);
    digitalWrite(leftDirection, HIGH);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeed);
  }
  /**********************Backward Left*************************/
  //If state is equal with letter 'H', car will go backward left
  else if (state == 'H')
  {
    digitalWrite(rightDirection, LOW);
    digitalWrite(leftDirection, HIGH);
    analogWrite(leftMotor, vSpeedturn);
    analogWrite(rightMotor, vSpeed);
  }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', car will go backward right
  else if (state == 'J')
  {
    digitalWrite(rightDirection, LOW);
    digitalWrite(leftDirection, HIGH);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeedturn);
  }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
  else if (state == 'L')
  {
    digitalWrite(rightDirection, LOW);
    digitalWrite(leftDirection, LOW);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeed);
  }
  /***************************Right****************************/
  //If state is equal with letter 'R', wheels will turn right
  else if (state == 'R')
  {
    digitalWrite(rightDirection, HIGH);
    digitalWrite(leftDirection, HIGH);
    analogWrite(leftMotor, vSpeed);
    analogWrite(rightMotor, vSpeed);
  }
  /************************Stop********************************/
  //If state is equal with letter 'S', stop the car
  else if (state == 'S')
  {
    analogWrite(leftMotor, 0);
    analogWrite(rightMotor, 0);
  }

  if (irrecv.decode(&results))
  {
    if (results.bits <= 16)
    {
      switch (results.value)
      {
      // When the loaded code is received
      case loaded:
        armed = 1;
        Serial.println("armed");
        break;

      // When the disabling code is received
      case disabled:
        paralysed = 1;
        Serial.println("disabled");
        break;
      }
    }
    irrecv.resume(); // Receive the next value
    irrecv.enableIRIn();
  }

  if (armed == 1)
  {
    digitalWrite(light, HIGH);
    //If state is equal with letter 'W', turn leds on or off
    if (state == 'W' || state == 'w')
    {
      for (int m = 0; m < 3; m++)
      {
        irsend.sendSony(2705, 12);
        delay(40);
      }
      irrecv.resume(); // Receive the next value
      irrecv.enableIRIn();

      // Reset armed and LED back to non-loaded state
      armed = 0;
      digitalWrite(light, LOW);
    }
  }
  if (paralysed == 1)
  {
    pinMode(reset, OUTPUT);
  }
}
