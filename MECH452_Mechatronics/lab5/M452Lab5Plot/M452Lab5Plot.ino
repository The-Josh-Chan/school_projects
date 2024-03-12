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

//global variables
float result;          //A to D value from photoresistor
float mvresult;        //millivolt value for photoresistor
unsigned long time;
unsigned long start;

// Setup Routine
void setup() 
{                
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
   
  do {
    toggleLED(GRN);         // Green LED flashing
  }while(digitalRead(BUTTON)== LOW);
    Serial.println("Program running.");
    
  delay(1000); // give user chance to get ready before plotting starts after button press
}

// Main Loop
void loop() {
  
   turnOnLED(YLW); 
   start = millis();
   for(int ii = 0; ii < (495); ii++){
  
   result = analogRead(LIGHT);               //read the value of photoresistor
   mvresult = map(result, 0, 1024, 0, 5000);  //convert value to millivolts
 
   time = (millis()-start)/10;                //time is in msec, plot in msec/10
   if (time>2000){                            // to give 2000 for 20 sec plot
       time = 2000;
   }   
   
  //    Serial.print(time);           // commented out for Part 1
  //    Serial.print(" ");            // commented out for Part 1
      Serial.println(mvresult);
  //    delay(40);                    // commented out for Part 1
   }  
      turnOnLED(RED);  // end of program
  do{
      toggleLED(RED);
    }while(-1);        // repeat forever
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
