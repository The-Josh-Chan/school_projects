/********************************************************************
  M452Lab3Sharp (v1.8)

  
  Original by H. Fernando, 24/07/2013
  Modified by Joshua Chan, Connor Russo, Matthew Birchard, 12-02-2024
  
  Reads Sharp range sensor and prints to serial monitor.
  
  Program ready, green LED flashing
  Button press to start, yellow LED on when reading
  Button press to repeat, red LED flashing 
*********************************************************************/
 
// Pin Assignments
int RED = 4;         //red LED Pin
int GRN = 5;         //green LED Pin
int YLW = 6;         //yellow LED Pin
int BUTTON = 8;      //push button
    
int SHARP1 = A1;     //sharp input pin (can't be a1)

//global variables
int result;          //A to D value from sharp
int mv;        //millivolt value for sharpvalue
int reading = 1;     //counter 

// Setup Routine
void setup() 
{                
  // initialize led pins as outputs.
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);
  
  //initialize button pins as inputs
  pinMode(BUTTON, INPUT);
  
  //initialize sharp
  pinMode(SHARP1, INPUT);

  //initialize serial printout
  Serial.begin(9600);
  Serial.println(3200);
  Serial.println(1);
  
  //Program Initalization
  Serial.println("Program loaded.");
  Serial.println("Press button to start.");
   
  do {
    toggleLED(GRN);         // Green LED flashing
  }while(digitalRead(BUTTON)== LOW);
  Serial.println("Program running.");
}

// Main Routine
void loop() {
  int count = 0;
  turnOnLED(YLW);
  delay(1000);     // 1 sec pause for setup

  do{
    result = analogRead(SHARP1);                //read the value of the sharp
    mv = map(result, 0, 1024, 0, 5000);         //convert value to millivolts
    Serial.println(mv);
    count ++;
    delay(40);
  }while(count<495 && digitalRead(BUTTON)== LOW);

  //print values to the serial monitor
  //tab format makes it easier to copy and paste values to Excel
  // Serial.print(reading);
  // Serial.print( "\t");    //tab
  // Serial.print( "result: ");
  // Serial.print( "\t");    //tab
  // Serial.print(result);                    
  // Serial.print( "\t");    //tab
  // Serial.print( "mv: ");
  // Serial.print( "\t");    //tab

    
  turnOnLED(RED);  //this turns the yellow LED off
  Serial.println("Press button again to continue.");
  delay(500);
  do{
    toggleLED(RED);
  }while(-1);
  
  reading = reading + 1;
}


//**********SUBROUTINES******************

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
  delay(250);
  digitalWrite(colour, LOW);
  delay(250); 
}
