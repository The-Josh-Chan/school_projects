/*************************************************************
  M452Lab2UpDown (v1.8)
  Created by: H. Fernando, 17/07/2013
  Revised by: B. Surgenor, 27/09/2017 and then 09/08/2020
  
  This program allows the user to test different motor speeds
  - Press B Button to start the program (and toggle increment)
  - Press A Button to increase motor speed
  - Press C Button to decrease motor speed
  - Press bumper to restart the program
 ***************************************************************/
 
// pin assignments
int RED = 4;
int GRN = 5;
int YLW = 6;
int BUTTON_A = 7;
int BUTTON_B = 8;
int BUTTON_C = 9;

int MOTOR_L = 10;  // left motor PWM signal
int MOTOR_R = 11;  // right motor PWM signal
int BUMPER =  13;

// global constants
const int STOP_SPEED = 155;           // 155 to begin, find value for your robot 
const int INIT_SPEED = 70;           // 155 to begin, ranges from 70 to 230

//global variables
int delta = INIT_SPEED - STOP_SPEED; 
int increment = 1;                    // use PBB to toggle between 1 and 5 when running 

// Set-up Routine
void setup() {                
  // initialize the digital LED pins as outputs.
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
  
  //stop motors
  runMotors(0,0); 
  
  //setup serial debug
  Serial.begin(9600);
}

// Main routine
void loop() {
  
  Serial.println("Program ready. Press Button B to start");
  do {
    toggleLED(GRN);         //motors stopped, Green LED flashing   
  } while(digitalRead(BUTTON_B)== HIGH);
  Serial.println("Program Running.");
  delay(1000);
  
  do {
    if (digitalRead(BUTTON_A)== LOW){   //press button A to increase speed by 'increment'
      delta = delta + increment;
    }
    if (digitalRead(BUTTON_C)== LOW){   //press button C to reduce speed by 'increment'
      delta = delta - increment;
      
    }
    if (digitalRead(BUTTON_B)== LOW){   //press button B to change increment value
      if(increment == 5)
      {
        increment = 1;
        Serial.println("Increment is now 1");
        delay(1000);
      }
      else
      {
        increment = 5;
        Serial.println("Increment is now 5");
        delay(1000);
       }
    }
       
    Serial.print("Speed: ");            //print current speed to screen
    Serial.println(STOP_SPEED + delta);
    
    runMotors(delta, delta);            //run motors at current speed
    
    delay(500);                        //pause before repeating (set to 1000 for demo)
     
   }while(digitalRead(BUMPER) == LOW);  //run until bumper is pressed
  
  delta = INIT_SPEED - STOP_SPEED;      //reset value of delta to initial speed
  runMotors(0,0);                       //stop motors 
  delay(500);
  
}

//**********SUBROUTINES******************

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
  int pulse_L = (STOP_SPEED + delta_L)*10;    //determines the length of pulse in microseconds
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
