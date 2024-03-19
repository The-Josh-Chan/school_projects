/********************************************************************
  M452Lab7Plot (v1.8)
  Original by B. Surgenor, 23/10/2020 (based on Lab5SharpPlot)
  - 500 point limit and red LED at end, as recommended from Lab#3
  Modified by Connor Russo, Joshua Chan, Matt Birchard 2024/03/18
  - Move Lynxbot to search for light
  - Move lynxbot to respectfully kill the light

  Reads Photoresistor voltage continuously and plots 500 points
  
  
  Program ready, green LED flashing
  Button press to start, yellow LED on when reading
  Flashes red after 500 points plotted
  Reset button to repeat (or close and open serial plotter)
*********************************************************************/
 
// Pin Assignments
int RED = 4;         //red LED Pin
int GRN = 5;         //green LED Pin
int YLW = 6;        //yellow LED Pin 

int BUTTON_A = 7;
int BUTTON_B = 8;
int BUTTON_C = 9;

int MOTOR_L = 10;  // left motor signal
int MOTOR_R = 11;  // right motor signal
    
int LIGHT = A0;     //photoresistor pin (can't be lowercase a0)

int SHARP1 = A1;   // sharp input pin
int sensor1;       // sharp sensor reading

int delta  = 25;   // 25 as default for speed for speed test
int delta_turn = 10; // 10 for sweep speed

int delay_inc = 500; // SET TO 5 DEGREE TURN
int delay_90 = 5000; // SET TO 90 DEGREE TURN TIME

// adjust stop speed and target distance as appropriate
int WALL = 1400;       // 700 (40cm) for corridor, 1200 (20cm) for wall 
int STOP_SPEED = 147;  // 147 as default, change to match that in Lab #4

//global variables
float result;          //A to D value from photoresistor
float mvresult;        //millivolt value for photoresistor
unsigned long time;
unsigned long start;
unsigned long time_start;

// Setup Routine
void setup()
{              
  // initialize led pins as outputs.
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);
  
  // initialize buttons and bumper pins as inputs
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_C, INPUT);

  // initialize motor control pins as outputs
  pinMode(MOTOR_L, OUTPUT);
  pinMode(MOTOR_R, OUTPUT);

  //initialize pins as inputs
  pinMode(LIGHT, INPUT);

  //initialize serial printout
  Serial.begin(9600);

  runMotors(0,0);  // check motors are stopped
  
  //Program Initalization
  Serial.println("Program loaded.");
  Serial.println("Press button B to start.");

  do {
    toggleLED(GRN);         // Green LED flashing
  } while(digitalRead(BUTTON_B)== HIGH);
    
  sensor1 = map(analogRead(SHARP1),0,1023,0,5000); // initialize sensor
  Serial.println("Program running.");
  time_start = millis();
}

// Main Loop
void loop() {
  // Select distance from target
  // Button A = 2m
  // Button C = 3m

  // First sweep
  // declare function for sweep increment
  // Turn 90 left 
  // sweep 180 right
  sweep(delta_turn);

  // get highest light value increment position
  // turn to light direction by number of increment
  // move to direction of light for 1 meter
  // pause of 2 seconds
  // sweep again for highest light
  // go for highest light
  // when sharp is 20cm from base of lamp, stop bot pause for 2 seconds
  // bot turn 180 deg
  // bot back up and takeout lamp

}

//********** FUNCTIONS (subroutines) **************


void pulseOut(int motor, int pulsewidth){
  digitalWrite(motor, HIGH);         
  delayMicroseconds(pulsewidth);  //send pulse of desired pulsewidth      
  digitalWrite(motor, LOW);
}

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

void runMotors(int delta_L, int delta_R){
  int pulse_L = (STOP_SPEED + delta_L)*10;  //determines length of pulse in microsec
  int pulse_R = (STOP_SPEED + delta_R)*10;
  for(int i=0; i<3; i++){
    pulseOut(MOTOR_L, pulse_L);    //send pulse to left motors
    pulseOut(MOTOR_R, pulse_R);    //send pulse to right motors
  }
}

void runMotorSweep(int delta_L, int delta_R){
  runMotors(delta_L, delta_R);
  // Change delay to 5 degree turn
  delay(delay_inc);
}

void sweep(int delta){
  runMotors(-delta, delta);
  // Change delay to get 90 degrees to the left
  delay(delay_90);

  int max_light = map(analogRead(LIGHT), 0, 1024, 0, 5000);
  int increment_count = 0;
  for(int i = 0; i<36; i++){
    runMotorSweep(delta, -delta);
    result = analogRead(LIGHT);               //read the value of photoresistor
    mvresult = map(result, 0, 1024, 0, 5000);
    if (mvresult>max_light){
        max_light=mvresult;
        increment_count += 1;
  }
  int found_light = 36-increment_count;
  for(int j = 0; j < found_light; j ++){
    runMotorSweep(-delta, delta);
    }
  }
}



