/*
  Joystick and Serial Read

  Control a Servo with a Joystick and Serial Input

  Serial Input commands are used to trim the Servo position
  If Serial input is available, read bytes until a delimiter, and parse the buffer for the command
  Expected commands: 0: UP, 1: DOWN, 2: LEFT, 3: RIGHT
  
  
  MCU is an ATmega328 on VMA102 - (Arduino Nano Clone, Old Bootloader)
  
  created 2019-12-26
  by Jeremy Savor

  This example code is in the public domain.
*/

#include <Servo.h>

// Constants
int STICK_MIN = 0;
int STICK_MAX = 1024;
int POS_MIN = 5;
int POS_MAX = 175;
int CENTER = 450;
int OFFSET_INC = 5;

// Pin Defines
int PIN_RX = A1;
int PIN_SERVO_X = 9; // Digital Pin 9

// Global Variables
int highLow = HIGH;
int rx;
int counter = 0;
int posX = CENTER;
int posX_offset = 0;
Servo servoX;

// Char Buffers
char str_rx[10];
char buf[100];
const int MAX = 20;
char read_buf[MAX];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RX, INPUT);
  servoX.attach(PIN_SERVO_X);
  Serial.begin(9600);
}

void loop() {
  counter += 1;
  if ( counter % 25 == 0 ) {
    digitalWrite(LED_BUILTIN, highLow);
    highLow = toggleHighLow(highLow);
  }

  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.readBytesUntil('\n', read_buf, MAX);
    Serial.print("Incoming: ");
    switch (read_buf[0]) {
      case '0':
        Serial.println("UP");
        break;
      case '1':
        Serial.println("DOWN");
        break;
      case '2':
        posX_offset -= OFFSET_INC;
        Serial.print("LEFT");
        Serial.print(" Offset: ");
        Serial.println(posX_offset);
        break;
      case '3':
        posX_offset += OFFSET_INC;
        Serial.print("RIGHT");
        Serial.print(" Offset: ");
        Serial.println(posX_offset);
        break;
      default:
        sprintf(buf, "UNKNOWN: %s", read_buf[0]);
        Serial.println(read_buf);
        break;
    }
    Serial.flush();
    digitalWrite(LED_BUILTIN, LOW);
    delay(1);
  }
  else {
    rx = analogRead(PIN_RX);
    ry = analogRead(PIN_RY);
    posX = joystick_to_servo_position(rx);
    posX += posX_offset;
    servoX.write(posX);
    Serial.println(buf);
    delay(20);
  }
}

int prev_joystick = 0;
int prev_position = posX;
int joystick_to_servo_position(int joystick) {
  int tolerance = 20;
  int servo_position = prev_position;
  if (abs(joystick - prev_joystick) > tolerance){
    servo_position = map(joystick, STICK_MIN, STICK_MAX, POS_MIN, POS_MAX);
    prev_position = servo_position;
  }
  prev_joystick = joystick;
  return servo_position;
}

int toggleHighLow(int highLow) {
  return (highLow == HIGH) ? LOW : HIGH;
}
