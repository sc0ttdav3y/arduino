/**
 * Use a light-dependent resistor to show the light level via a series of LEDs
 * 
 * By Scott Davey
 * www.coderdojorealm.com 
 * April 2018
 * 
 * - Connect 5 LEDs to DIGITAL pins 9 through 13
 * - Connect an LDR to the 3.3v, GND and ANALOG pin 0
 * 
 * The lights will show the level of the light as a meter between 
 * 0 (low light) and 5 (bright light). 
 * 
 * Set debug = 1 and connect to the Serial Monitor to see the calculations 
 * behind the project.
 */

// The digital pins used for the five LEDs
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;
const int led4 = 12;
const int led5 = 13;

// an array of the leds
const int leds[] = { led1, led2, led3, led4, led5 };
const int ledCount = 5;

// The analog port with the LDR
const int ldr1 = 0;

// the (measured) lowest and highest number from the light-sensitive resistor
const float rawLdrMin = 30;
const float rawLdrMax = 662;

// debugging: set to 1 to enable debugging
const int debug = 0;

/**
 * Setup
 * 
 * Runs once each time the board powers up
 */
void setup() 
{
  // initialize the led inputs
  for (int i = 0; i < ledCount; i++) {
    pinMode(leds[i], OUTPUT);  
    digitalWrite(leds[i], LOW); 
  }

  // initialise the LDR input
  pinMode(ldr1, INPUT);

  // initialise the serial monitor
  if (debug) {
    Serial.begin(9600);
    Serial.println("Potentiometer light delay");
  }
}

/**
 * Loop
 * 
 * The loop function runs over and over again forever
 */
void loop() 
{
  // Get the light percentage and convert it to 
  // a number between 1 and 5.
  float lightPercentage = get_percentage();
  int outOf5 = lightPercentage / 20;

  if (debug) {
    Serial.print("; => ");
    Serial.print(outOf5);
    Serial.print(" / 5");
  }
  
  // Loop over each LED and turn it HIGH if its index is 
  // equal or lower than outOf5, otherwise set it to LOW.
  for (int i = 0; i < ledCount; i++) {
    if (i < outOf5) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }

  if (debug) {
    Serial.println();
  }
  
}


/**
 * Reads the LDR and returns the value as a percentage
 * 
 * @return float
 */
float get_percentage() {

  float rawLdr = analogRead(ldr1);

  float percentage = ((rawLdr - rawLdrMin) / (rawLdrMax - rawLdrMin)) * 100;

  if (debug) {
    Serial.print("Raw LDR = ");
    Serial.print(rawLdr, 0);
    Serial.print("; Percentage = ");
    Serial.print(percentage);
    Serial.print("%");
  }
  
  return percentage;
}



