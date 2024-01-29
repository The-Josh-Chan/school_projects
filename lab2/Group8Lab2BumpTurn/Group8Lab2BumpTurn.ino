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
int BUMPER = 12;   // was 12

// global constants
const int STOP_SPEED = 147;  // 150 to begin
const int delta = 15;        // 15 to begin
const int turnDelta = 15;
const int t = 1000;

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
  
int bumpCount = 0;
char robotState[] = "running";
int timeOne = millis();
int timeTwo = millis();
while(bumpCount < 4){
  runMotorsForward(delta,delta); 
  int timeOne = millis();
  int timeTwo = millis();
  if (digitalRead(BUMPER) == HIGH) {
    timeTwo = millis();
    // Stop robot
    runMotors(0,0); 
    digitalWrite(GRN, LOW);
    digitalWrite(RED, HIGH);
    // reverse robot
    runMotorsBack(delta, delta);
    delay(250);
    runMotors(0,0);
    delay(250);
    // Turn Function
    Turn_left(turnDelta, t);
    runMotors(0,0);
    bumpCount += 1;
  }
}
// Turn other direction
  runMotorsBack(delta, delta);
  delay(1000);
  
  // Turn_right(turnDelta, t);
  // int reverseTime =  (timeOne - timeTwo) / 2;
  // runMotorsBack(delta, delta);
  // delay(reverseTime);
  // runMotors(0,0);
  strcpy(robotState, "done");
  do{//done running
    digitalWrite(GRN, HIGH);
    digitalWrite(YLW, HIGH);
    digitalWrite(RED, HIGH);
    delay(250);
    digitalWrite(GRN, LOW);
    digitalWrite(YLW, LOW);
    digitalWrite(RED, LOW);
    delay(250);
  }while(strcmp(robotState, "done") == 0);//how program knows it's done
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

void runMotorsForward(int delta_L, int delta_R)
{
  turnOnLED(GRN);
  int pulse_L = (STOP_SPEED + delta_L)*10;    //length of pulse in msec
  int pulse_R = (STOP_SPEED + delta_R)*10;
  
  for(int i=0; i<3; i++){
    pulseOut(MOTOR_L, pulse_L);    //send pulse to left motors
    pulseOut(MOTOR_R, pulse_R);    //send pulse to right motors
  }
  digitalWrite(GRN, LOW);
}

void runMotorsBack(int delta_L, int delta_R)
{
  turnOnLED(YLW);
  int pulse_L = (STOP_SPEED - delta_L)*10;    //length of pulse in msec
  int pulse_R = (STOP_SPEED - delta_R)*10;
  
  for(int i=0; i<3; i++){
    pulseOut(MOTOR_L, pulse_L);    //send pulse to left motors
    pulseOut(MOTOR_R, pulse_R);    //send pulse to right motors
  }
  digitalWrite(YLW, LOW);
}

void pulseOut(int motor, int pulsewidth)
{
  digitalWrite(motor, HIGH);         
  delayMicroseconds(pulsewidth);  //send pulse of desired pulsewidth      
  digitalWrite(motor, LOW);
}
  
void Turn_right(int delta, int t){ 
                                  //SET T = 1000 AND DELTA = 15 FOR 90 DEGREE TURN
  turnOnLED(YLW);
  delay (500);
  runMotors(delta, -1*delta);    // run Turn for one second
  delay(1000);
  runMotors(0,0);               // Stop motors
  toggleLED(YLW);
}

void Turn_left (int delta, int t){
                                   //SET T = 1000 AND DELTA = 15 FOR 90 DEGREE TURN
  turnOnLED(YLW);
  delay (500);
  runMotors(-1*delta, delta);    // run Turn for one second
  delay(1000);
  runMotors(0,0);               // Stop motors
  toggleLED(YLW);
}
  