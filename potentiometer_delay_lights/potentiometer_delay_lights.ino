/**
 * Use a potentiometer to control the cycle speed of three lights.
 * 
 * By Scott Davey
 * www.coderdojorealm.com 
 * April 2018
 * 
 * Instructions;
 * 
 * - connect three LEDs to the DIGITAL pins 11, 12 and 13
 * - connect the potentiometer's middle pin to ANALOG 0
 * - connect the potentiometer's left pin to the 3.3V pin
 * - connect the potentiometer's right pin to GND
 * 
 * When it powers up, the LEDs will cycle at the delay speficied by 
 * the potentiometer. Turn the knob to speed up or slow down the 
 * cycle.
 * 
 * Open the Arduino IDE's Serial Monitor (Tools > Serial Monitor) to
 * view the raw potentiometer value and the calculated delay.
 */

// The digital pins used for the three LEDs
const int led1 = 13;
const int led2 = 12;
const int led3 = 11;

// The analog port with the potentiometer
const int pot1 = 0;

// The maximum and minimum delay 
const int delayMin = 100;
const int delayMax = 1500;

// the (measured) lowest and highest number from the potentiometer
//const float rawPotMin = 0;
//const float rawPotMax = 664;

// the (measured) lowest and highest number from the light-sensitive potentiometer
const float rawPotMin = 100;
const float rawPotMax = 662;

/**
 * Setup
 * 
 * Runs once each time the board powers up
 */
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(pot1, INPUT);

  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  
  Serial.begin(9600);
  Serial.println("Potentiometer light delay");
}

/**
 * Loop
 * 
 * The loop function runs over and over again forever
 */
void loop() {

  // the delay (in microseconds) between cycling each light
  int d;

  d = get_speed();
  digitalWrite(led1, HIGH); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(d);               
  
  d = get_speed();
  digitalWrite(led1, LOW); 
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  delay(d);               

  d = get_speed();
  digitalWrite(led1, LOW); 
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(d);               
}


/**
 * Reads the potentiometer and returns the calculated speed in ms
 * 
 * @return int
 */
int get_speed() {

  float rawPot = analogRead(pot1);

  float percentage = (rawPot - rawPotMin) / (rawPotMax - rawPotMin);

  // calculate as a number between min and max delay
  int d = delayMin + (percentage * delayMax);
  
  Serial.print("Raw pot = ");
  Serial.print(rawPot, 0);
  Serial.print("; computed delay = ");
  Serial.print(d);
  Serial.println(" ms");

  return d;
}



