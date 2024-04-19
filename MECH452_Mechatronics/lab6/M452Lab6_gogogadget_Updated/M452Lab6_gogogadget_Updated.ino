/********************************************************************
  M452Lab4OnOff (v2.0)
  
  Original by H. Fernando, 24/09/2013  
  Revised by B. Surgenor, 17/09/2016, 01/10/2020, 30/09/2021
  
  This is an ON/OFF wall following algorithm for a robot equipped
  with a single Sharp on the left front corner of the robot. 
   
 ***********************************************************************/
// Pin Assignments
int RED = 4;
int GRN = 5;
int YLW = 6;

int BUTTON_A = 7;
int BUTTON_B = 8;
int BUTTON_C = 9;
int MOTOR_L = 10;  // left motor signal
int MOTOR_R = 11;  // right motor signal
int BUMPER = 12;

int SHARP1 = A1;   // sharp input pin
int sensor1;       // sharp sensor reading

int LIGHT = A0; // photoresistor pin 

// recommended initial values 
int delta  = 25;   // 25 as default for speed for speed test
int HYS    = 50;   // 50 as default for hysterisis
int delta_turn = 25; // 10 as turning speed for motors    

// adjust stop speed and target distance as appropriate
int WALL = 700;       // 700 (40cm) for corridor, 1200 (20cm) for wall 
int STOP_SPEED = 147;  // 147 as default, change to match that in Lab #4

//delay variables
int start_dist_delay = 0;
int fwd_delay = 0;

// Global Variables
float result;
float mvresult;
int start_distance;
int run_delay = 5000;
bool light_terminated;

// Set-up Routine
void setup() {                
  // initialize the digital led pins as outputs.
  pinMode(RED, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(GRN, OUTPUT);
  //initialize buttons, light, and bumper pins as inputs
  pinMode(BUMPER, INPUT);
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
  pinMode(LIGHT, INPUT);
  //initialize motor control pins as outputs
  pinMode(MOTOR_L, OUTPUT);
  pinMode(MOTOR_R, OUTPUT);
  
  //setup serial debug
  Serial.begin(9600);

  runMotors(0,0);  // check motors are stopped
  
  Serial.println("Press Button B to start.");
  do {
    toggleLED(GRN);         //motors stopped, Green LED flashing
  } while(digitalRead(BUTTON_B)== HIGH);

  do  {
    toggleLED(YLW);
    Serial.println("Ready to go");
  } while(digitalRead(BUTTON_B) == LOW);
  
  light_terminated = false;
  sensor1 = map(analogRead(SHARP1),0,1023,0,5000); // initialize sensor
  Serial.println("Program Running. Press bumper to stop");
}

// Main Routine
void loop() {
  delay (1000);
  do{
    int turn_90 = 1200;
    runMotors(10,10);
    delay(50);
    start_dist_delay=sweepForLight(delta_turn, turn_90);
    fwd_delay=start_dist(start_dist_delay);
    runMotors(0,0);
    delay(2000);
    turn_90 = 900;
    runMotors(delta,delta);
    delay(fwd_delay);
    runMotors(0,0);
    start_dist_delay=sweepForLight(delta_turn, turn_90);
    
    do{
      runMotors(delta, delta);
    }while((map(analogRead(LIGHT), 0, 1024, 0, 5000)<3810) or (analogRead(SHARP1) > 900));// Stop Motors once light is reach
    //while(map(analogRead(SHARP1),0,1023,0,5000) < WALL); // Wall should be set to 40cm away from the wall behind the light
    runMotors(0,0);
    delay(1000);
    // Turn 180 deg
    runMotors(-delta_turn, delta_turn);
    delay(1450);
    runMotors(0,0);
    runMotors(-delta,-delta);
    delay(400);
    runMotors(0,0);
    delay(1000);
    light_terminated = true;
  }while(light_terminated != true);
  
  do{
    toggleLED(RED);
    runMotors(0,0);
  }while(light_terminated = true);
}

//**********FUNCTIONS (subroutines)******************

// left and right motor commands
void runMotors(int delta_L, int delta_R){
  int pulse_L = (STOP_SPEED + delta_L)*10;  //determines length of pulse in microsec
  int pulse_R = (STOP_SPEED + delta_R)*10;
  for(int i=0; i<3; i++){
    pulseOut(MOTOR_L, pulse_L);    //send pulse to left motors
    pulseOut(MOTOR_R, pulse_R);    //send pulse to right motors
  }
}

int sweepForLight(int deltaTurn, int turn_time){
  delay(50);
  runMotors(-deltaTurn, deltaTurn);
  delay(turn_time); // Set time for desired turn length
  runMotors(0,0); // Stop initial turn to left
  delay(2000);
  result = analogRead(LIGHT);
  mvresult = map(result, 0, 1024, 0, 5000);
  int max_light = mvresult;
//  int increment_count = 0;
  for(int i = 0; i<60; i++){
    runMotors(deltaTurn, -deltaTurn);
    delay(turn_time/18);
    runMotors(0,0);
    delay(100);
    // Read current light situation
    result = analogRead(LIGHT);
    mvresult = map(result, 0, 1024, 0, 5000);
    if (mvresult>max_light){
        max_light=mvresult;
    }
  }
  runMotors(0,0);
  delay(1000);
    result = analogRead(LIGHT);
    int current_read = map(result, 0, 1024, 0, 5000);
  do{
    runMotors(-17, 17);
    result = analogRead(LIGHT);
    current_read = map(result, 0, 1024, 0, 5000);
  }while(current_read < max_light-9);
  
  // Turn Motors off for next step after sweeping for light
  runMotors(0,0);
  delay(2000);
  return max_light;
}

int start_dist(int max_light){
  int fwd_delay=0;
  if (max_light > 1750){//if true starting dist is 2m
    fwd_delay = 900;//calibrate this
  }
  else{//if false starting dist is 3m
    fwd_delay = 2300;//calibrate this
  }
  return fwd_delay;
}

// single motor pulsewidth command
void pulseOut(int motor, int pulsewidth){
  digitalWrite(motor, HIGH);         
  delayMicroseconds(pulsewidth);  //send pulse of desired pulsewidth      
  digitalWrite(motor, LOW);
}

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

//flash all LEDs
void flashAllLEDs(){
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  delay(250);
  digitalWrite(GRN, HIGH);
  digitalWrite(YLW, HIGH);
  digitalWrite(RED, HIGH);
  delay(250);
}
