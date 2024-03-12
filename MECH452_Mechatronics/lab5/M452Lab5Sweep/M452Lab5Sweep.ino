/********************************************************************
  M452Lab7Plot (v1.8)
  Original by B. Surgenor, 23/10/2020 (based on Lab5SharpPlot)
  - 500 point limit and red LED at end, as recommended from Lab#3
  Modified by Connor Russo, Joshua Chan, 2024/03/11
  - Move servo motor to sweep for light

  Reads Photoresistor voltage continuously and plots 500 points
  
  
  Program ready, green LED flashing
  Button press to start, yellow LED on when reading
  Flashes red after 500 points plotted
  Reset button to repeat (or close and open serial plotter)
*********************************************************************/
 
// Pin Assignments
int RED = 4;         //red LED Pin
int GRN = 5;         //green LED Pin
int YLW = 6;         //yellow LED Pin 
int BUTTON = 8;      //push button
    
int LIGHT = A0;     //photoresistor pin (can't be lowercase a0)

int servoPin = 12;
int servoAngle; //angle of the servo roughly 0-180
int pulseWidth;
#include <Servo.h>
Servo myservo;

//global variables
float result;          //A to D value from photoresistor
float mvresult;        //millivolt value for photoresistor
unsigned long time;
unsigned long start;
unsigned long time_start;
// Setup Routine
void setup() 
{           

  pinMode(servoPin, OUTPUT);     
  // initialize led pins as outputs.
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);
  
  //initialize pins as inputs
  pinMode(BUTTON, INPUT);
  pinMode(LIGHT, INPUT);

  //initialize serial printout
  Serial.begin(9600);
  Serial.println(2400);            // set desired max vertical scale
  Serial.println(0);
  
  //Program Initalization
  Serial.println("Program loaded.");
  Serial.println("Press button to start.");
  
  //initialize servo and define pin
  myservo.attach(servoPin);

  do {
    toggleLED(GRN);         // Green LED flashing
    myservo.write(180);
    delay(100);
    myservo.write(0);
  }while(digitalRead(BUTTON)== LOW);
    Serial.println("Program running.");
  delay(1000); // give user chance to get ready before plotting starts after button press
  time_start = millis();
}

// Main Loop
void loop() {
  time = millis() - time_start;
  Serial.println(time);
  do{
    myservo.write(0);
    time = millis() - time_start;
  }while(time <4000);
    // result = analogRead(LIGHT);               //read the value of photoresistor
    // mvresult = map(result, 0, 1024, 0, 5000); 
  do{
    myservo.write(90);
    time = millis() - time_start;
  }while(4000 < time < 9000);

  do{
    myservo.write(180);
    time = millis() - time_start;
  }while(9000 < time < 14000);

  do{
    myservo.write(90);
    time = millis() - time_start;
  }while(14000 < time < 19000);

  do{
    myservo.write(0);
    time = millis() - time_start;
  }while(20000 < time);




  //  turnOnLED(YLW); 
  //  start = millis();
  //  for(int ii = 0; ii < (495); ii++){
  
  //  result = analogRead(LIGHT);               //read the value of photoresistor
  //  mvresult = map(result, 0, 1024, 0, 5000);  //convert value to millivolts
 
  //  time = (millis()-start)/10;                //time is in msec, plot in msec/10
  //  if (time>2000){                            // to give 2000 for 20 sec plot
  //      time = 2000;
  //  }   
   
  // //    Serial.print(time);           // commented out for Part 1
  // //    Serial.print(" ");            // commented out for Part 1
  //     Serial.println(mvresult);
  // //    delay(40);                    // commented out for Part 1
  //  }  
  //     turnOnLED(RED);  // end of program
  // do{
  //     toggleLED(RED);
  //   }while(-1);        // repeat forever
}

//********** FUNCTIONS (subroutines) **************

//Turn on a single LED, and all other off
void turnOnLED(int COLOUR){
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW); 
  digitalWrite(COLOUR, HIGH);
}

//Toggle an LED on/off
void toggleLED(int colour){
  digitalWrite(colour, HIGH);
  delay(125);
  digitalWrite(colour, LOW);
  delay(125); 
}

// Turn servomotor by pulse width
void servoPulse(int servoPin, int myAngle)
{
  int pulseWidth = (myAngle * 10) + 275;
  digitalWrite(servoPin, HIGH);         //set servo high
  delayMicroseconds(pulseWidth);        //microsecond pause
  digitalWrite(servoPin, LOW);    
  }      //set servo low
