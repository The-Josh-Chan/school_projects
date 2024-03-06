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

// recommended initial values 
int delta  = 25;   // 25 as default for speed for speed test
int HYS    = 50;   // 50 as default for hysterisis    

// adjust stop speed and target distance as appropriate
int WALL = 1400;       // 700 (40cm) for corridor, 1200 (20cm) for wall 
int STOP_SPEED = 147;  // 147 as default, change to match that in Lab #4


// Set-up Routine
void setup() {                
  // initialize the digital led pins as outputs.
  pinMode(RED, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(GRN, OUTPUT);
  //initialize buttons and bumper pins as inputs
  pinMode(BUMPER, INPUT);
   pinMode(BUTTON_A, INPUT);
    pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);
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
  
  sensor1 = map(analogRead(SHARP1),0,1023,0,5000); // initialize sensor
  Serial.println("Program Running. Press bumper to stop");

  Serial.println("Select speed");
}

// Main Routine
void loop() {
// Select motor speed
do  {
toggleLED(YLW);
  if (digitalRead(BUTTON_A) == LOW){
    HYS = 25;
    Serial.println("Set delta = 15");
    break;
  }
  else if (digitalRead(BUTTON_B) == LOW){
    HYS = 50;
    Serial.println("Set delta = 25");
    break;
  }
  else if (digitalRead(BUTTON_C) == LOW){
    HYS = 100;
    Serial.println("Set delta = 45");
    break;
  }
}

while (true);
// start of bumper check loop
do{  
  sensor1 = map(analogRead(SHARP1),0,1023,0,5000); 

  if(sensor1 < (WALL-HYS)){          // too far from the wall
    turnOnLED(YLW);
    runMotors(0, delta); 
  }else if(sensor1 > (WALL+HYS)){    // too close to the wall
    turnOnLED(RED);
    runMotors(delta,0); 
  }else{                             // at target distance
    turnOnLED(GRN);
    runMotors(delta,delta); 
  }
}while(digitalRead(BUMPER)==LOW); // end of bumper loop
  
    runMotors(0,0); 
    delay(500);
    runMotors(-delta,-delta);
    delay(250);
    runMotors(0,0);
    while(1){                    // loop forever
      flashAllLEDs();
    }                            
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
