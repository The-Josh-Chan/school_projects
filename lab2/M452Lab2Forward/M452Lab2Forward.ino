/*****************************************************************************
  M452Lab2Forward (v1.8)
  
  Created by: H. Fernando, 17/07/2013
  Revised by: B. Surgenor, 27/09/2019 (LED logic)
  
  This program is for calibrating the speed of the LynxBot.  Sample numbers:
  delta = 15, 30, 50 with distance over 1 sec = 32, 72, 110 cm @ 12.8 v unloaded
  Remember to take each measurement 3 times in a row, checking forward and back. 
  
  Press PBB to start when green LED flashes. Forward 1 sec (green on), pauses 
  (yellow on, take your measurement) and then reverses for 1 sec (red on). 
  Solid green briefly, then back to flashing green, press PBB to repeat.
 *******************************************************************************/
 
// pin assignments
int RED = 4;
int GRN = 5;
int YLW = 6;

int BUTTON_A = 7;
int BUTTON_B = 8;
int BUTTON_C = 9;

int MOTOR_L = 10;  // left motor PWM signal
int MOTOR_R = 11;  // right motor PWM signal
int BUMPER = 13;   // was 12

// global constants
const int STOP_SPEED = 147;  // 150 to begin
const int delta = 60;        // 15 to begin

// Set-up Routine
void setup() {                
  // initialize the digital led pins as outputs.
  pinMode(RED, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(GRN, OUTPUT);

  pinMode(BUMPER, INPUT);  // initialize inputs
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  
  pinMode(MOTOR_L, OUTPUT); // initialize motor outputs
  pinMode(MOTOR_R, OUTPUT);
  
  Serial.begin(9600); // set up serial
  
  runMotors(0,0);  //stop motors
  
  //Print out Values
  Serial.println("Program Loaded.");
  Serial.print("Forward Speed: "); 
  Serial.println(STOP_SPEED + delta);
  Serial.print("Reverse Speed: "); 
  Serial.println(STOP_SPEED - delta); 
}

// Main Routine
void loop() {
  
  Serial.println("Press Button B to go forward.");
  
  do {
    toggleLED(GRN);         //motors stopped, Green LED flashing
  } while(digitalRead(BUTTON_B)== HIGH);
  
  delay(1000);

  runMotors(0,0);            //stop motors for 0.5 second
  delay(500);
  
  turnOnLED(GRN); 
  runMotors(delta, delta);    // run forward for 1 sec
  delay(1000);
  
  turnOnLED(YLW);
  runMotors(0,0);            // stop motors for 5 sec to take measurement
  delay(5000);

  turnOnLED(RED);
  runMotors(-delta, -delta); // reverse motors for 1 sec
  delay(1000);
  
  turnOnLED(GRN);
  runMotors(0,0);            //stop motors for 2 sec
  delay(2000);
}

//**********SUBROUTINES (functions) ******************

//Turn on a single LED, and all other off
void turnOnLED(int colour){
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
}

//Toggle an LED on/off
void toggleLED(int colour){
  digitalWrite(colour, HIGH);
  delay(250);
  digitalWrite(colour, LOW);
  delay(250); 
}

void runMotors(int delta_L, int delta_R)
{
  int pulse_L = (STOP_SPEED + delta_L)*10;    //length of pulse in msec
  int pulse_R = (STOP_SPEED + delta_R)*10;
  
  for(int i=0; i<3; i++){
    pulseOut(MOTOR_L, pulse_L);    //send pulse to left motors
    pulseOut(MOTOR_R, pulse_R);    //send pulse to right motors
  }
}

void pulseOut(int motor, int pulsewidth)
{
  digitalWrite(motor, HIGH);         
  delayMicroseconds(pulsewidth);  //send pulse of desired pulsewidth      
  digitalWrite(motor, LOW);
}
  
