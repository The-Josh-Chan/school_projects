/*******************************************************
M452Lab1ServoMotor (v1.6)

Original by B. Surgenor, 10/06/2019
Revised by B. Surgenor, 13/09/2021
- switch YLW and GRN (for Lab #5 alignment)


Based on Evan's Arduino Programming Library, originally from:
http://playground.arduino.cc/uploads/Main/arduino_notebook_v1-1.pdf

Program ready, green LED flashing
Button press to start, yellow LED on
Servo rotates 0 to 180, then 180 to 0, pause, red LED on
Then return to flashing green LED
*******************************************************/

// Pin Assignments
int RED = 4;           //red LED Pin
int GRN = 5;           //yellow LED Pin
int YLW = 6;           //green LED Pin
int BUTTON = 8;        //pushbutton

int servoPin = 12;    //servo connected to digital pin 12
int myAngle;          //angle of the servo roughly 0-180
int pulseWidth;       //servoPulse function variable
#include <Servo.h> //servo library call
Servo myservo; // create myservo object for library Servo
int potPin = 3;

// Set-up Routine
void setup() {
  
  pinMode(servoPin, OUTPUT);  //sets servoPin output
  Serial.begin(9600);
  
// initialize led pins as outputs.
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);
  
//initialize button pin as input
  pinMode(BUTTON, INPUT);
//initialize servo and define pin
myservo.attach(servoPin);
}

// Main Routine
void loop(){
  
// Wait for button press (and flash green while waiting)
  Serial.println("Press button to start.");
  do{
       digitalWrite(GRN, HIGH);
       delay(125);
       digitalWrite(GRN, LOW);
       delay(125);
    }while(digitalRead(BUTTON) == LOW);
    
  Serial.println("Servo sweeping.");
  turnOnLED(YLW);     // yellow indicates running
  
    while(true){
    int potVal = analogRead(potPin); // analog range assumes 5 v = 1023
    //myAngle = map(potVal,125,800,180,0);
    myAngle = map (potVal,0,1023,0,180);
    myservo.write(myAngle);
    delay(15);
        
}
}

//**************** FUNCTIONS (subroutines) *************

void servoPulse(int servoPin, int myAngle)
{
  pulseWidth = (myAngle * 5) + 1000; //nominal is 5 and 1000
  digitalWrite(servoPin, HIGH);  //set servo high
  delayMicroseconds(pulseWidth);  //microsecond pause
  digitalWrite(servoPin, LOW);  //set servo low
}

void turnOnLED(int COLOUR)
{
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(COLOUR, HIGH);
}
