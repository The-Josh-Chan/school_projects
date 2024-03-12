/*************************************************
  StackSearch(v1.4)

  Original by H Fernando, 24/6/2013
  Modified by B Surgenor, 13/9/2019



  Program ready, green LED flashing
  Button press to start, yellow LED on
  Servo searches CCW (yellow on) and then CW (red on)
  Stops when Pot angle matched (yellow or red flashing)
  Resumes after button press (yellow or red on, depends on direction)
  Continues search in the direction of the last search

 ************************************************/

// Pin Assignments
int RED = 4;           //red LED Pin
int YLW = 5;           //yellow LED Pin
int GRN = 6;           //green LED Pin

int BUTTON = 8;        //pushbbutton
int servoPin = 12;     //servo pin
int potPin = A2;       //potentiometer input pin

int potAngle;
int potVal;
int servoAngle;

// Setup routine
void setup() {

  // Initialize button pins as inputs
  pinMode(BUTTON, INPUT);

  // Initialize servo and potentiometer
  pinMode(servoPin, OUTPUT);
  pinMode(potPin, INPUT);

  // initialize led pins as outputs.
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);

  // Initialize serial printout
  Serial.begin(9600);
  Serial.println("Program ready, press button to start.");
}

// Main routine
void loop() {

  // Wait for button press
  do {
    blinkLED(GRN); // blink green LED
  } while (digitalRead(BUTTON) == LOW);

  // Read potentiometer and convert to an angle
  potVal = analogRead(potPin);
  potAngle = map(potVal, 0, 1023, 0, 180);

  do {
    // Sweep servo CCW
    for (servoAngle = 0; servoAngle <= 180; servoAngle++) {

      // Check potAngle and wait for button press if equal
      if (servoAngle == potAngle) {
        do
          blinkLED(YLW);   // blink yellow LED
        while (digitalRead(BUTTON) == LOW);
      }

      // Read potentiometer and convert to an angle
      potVal = analogRead(potPin);
      potAngle = map(potVal, 101, 802, 180, 0);

      // Increment position of servo
      servoPulse(servoPin, servoAngle);
      delay(15);
      turnOnLED(YLW);
    }

    // Sweep servo CW
    for (servoAngle = 180; servoAngle >= 0; servoAngle--) {

      // Check potAngle and wait for button press if equal
      if (servoAngle == potAngle) {
        do
          blinkLED(RED); // flash red LED
        while (digitalRead(BUTTON) == LOW);
      }

      // Read potentiometer and convert to an angle
      potVal = analogRead(potPin);
      potAngle = map(potVal, 101, 802, 180, 0);

      // Decrement position of servo
      servoPulse(servoPin, servoAngle);
      delay(15);
      turnOnLED(RED);
    }
  } while (1);
}

//********** FUNCTIONS (subroutines)  ******************

// Turn on an LED and all others off
void turnOnLED(int colour) {
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
}

// Blink a single LED
void blinkLED(int colour) {
  digitalWrite(colour, HIGH);
  delay(200);
  digitalWrite(colour, LOW);
  delay(200);
}

// Turn servomotor by pulse width
void servoPulse(int servoPin, int myAngle)
{
  int pulseWidth = (myAngle * 10) + 275;
  digitalWrite(servoPin, HIGH);         //set servo high
  delayMicroseconds(pulseWidth);        //microsecond pause
  digitalWrite(servoPin, LOW);          //set servo low
}
