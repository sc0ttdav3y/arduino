/**
 * Robot with forward-facing proximity sensor 
 * 
 * By Scott Davey
 * www.coderdojorealm.com 
 * May 2018
 * 
 * The robot hardware can be built from the Jacar Electronics' 
 * 'Wall Dodging Robot' DIY kit.
 * 
 * This robot is perpetually striving to drive forward. If the robot
 * detects an obstacle in front of it, it will slow down and eventually
 * try to maneuver itself around the obstacle. 
 * 
 * The maneuver for aboiding an obstacle involves backing up, then turning 
 * slowly to the right until it finds a clear path. 
 * 
 * See 'robot_proximity_pantilt' for an upgraded robot with the abiliy
 * to turn its head.
 */

const bool DEBUG_DISABLE_MOTOR = true;

// The motor pin configuration
const int PIN_MOTOR_A_ENABLE = 6;
const int PIN_MOTOR_A_INPUT1 = 4;
const int PIN_MOTOR_A_INPUT2 = 7;
const int PIN_MOTOR_B_ENABLE = 5;
const int PIN_MOTOR_B_INPUT3 = 3;
const int PIN_MOTOR_B_INPUT4 = 2;

// The ultrasonic sensor pin configuration
const int PIN_ULTRASONIC_TRIG = 8;
const int PIN_ULTRASONIC_ECHO = 9;

// Commands
const int CMD_FORWARD = 1;
const int CMD_TURN_LEFT = 2;
const int CMD_TURN_RIGHT = 3;
const int CMD_REVERSE = 4;
const int CMD_STOP = 5;
const int CMD_SEARCH = 6;
const int CMD_AVOID = 7;
const int CMD_AVOID_LEGACY = 999;

// Wheel diretions
const int WHEEL_STOP = 0;
const int WHEEL_FORWARD = 1;
const int WHEEL_REVERSE = 2;

// Speeds
const int SPEED_SLOW = 100;
const int SPEED_MEDIUM = 125;
const int SPEED_FAST = 150;

// Distances (in cm) tor various decision-making
const int DISTANCE_CLEAR = 50;                  // At this distance, assume nothing in our path
const int DISTANCE_APPROACHING_OBSTACLE = 40;   // At this distance, acknowledge an obstacle appraching
const int DISTANCE_OBSTACLE_IMMINENT = 30;      // At this distance, take evasive action

// The time each manuever normally takes, in milliseconds
const int DEFAULT_MANEUVER_TIME = 200;


/**
 * Setup
 * 
 * Runs once each time the board powers up
 */
void setup() {
  setupUltrasonic();
  setupMotor();
}


/**
 * Loop
 * 
 * The loop function runs over and over again forever
 */
void loop() 
{
  // check for obstacles
  float distanceAhead = readUltrasonicSensorDistance();
  int command;
  int speed;

  // decide upon a course of action
  if (distanceAhead > DISTANCE_CLEAR) {
    speed = SPEED_FAST;
    command = CMD_FORWARD;
  } else if (distanceAhead > DISTANCE_APPROACHING_OBSTACLE) {
    speed = SPEED_MEDIUM;
    command = CMD_FORWARD;
  } else {
    speed = SPEED_SLOW;
    command = CMD_AVOID;    
  }

  // now drive the robot according to the command
  maneuver(command, speed, DEFAULT_MANEUVER_TIME);

}



/**
 * Drives the robot according to the requested command at the requested speed
 * 
 * @param int cmd One of the COMMAND_* constants
 * @param int speed The wheel speed
 * @param int time The time to maintain the maneuver
 */
void maneuver(int cmd, int speed, int time) 
{
  
  switch (cmd) {
    
    case CMD_FORWARD:
      moveRobot(WHEEL_FORWARD, WHEEL_FORWARD, speed, time);
      break;
      
    case CMD_AVOID:
        // Step 1: Reverse 
        moveRobot(WHEEL_REVERSE, WHEEL_REVERSE, SPEED_SLOW, 1000);

        // Step 2: Turn right until there is a clear path
        float obstacleDistance;
        while (obstacleDistance < DISTANCE_OBSTACLE_IMMINENT) {
           moveRobot(WHEEL_FORWARD, WHEEL_REVERSE, SPEED_MEDIUM, 500);  
           obstacleDistance = readUltrasonicSensorDistance();
        };
        delay(1000); // wait a second, then go again
        break;
  }
}

/**
 * Move the robot in the left/right wheel direction specified, for the given speed and time
 * 
 * @param int leftDirection The right left direction using a WHEEL_* constant
 * @param int rightDirection The right wheel direction using a WHEEL_* constant
 * @param int speed The speed turn the wheels, using a SPEED_* constant
 * @param int time The time to wait in milliseconds
 */
void moveRobot(int leftDirection, int rightDirection, int speed, int time) 
{
  analogWrite(PIN_MOTOR_A_ENABLE, speed);
  analogWrite(PIN_MOTOR_B_ENABLE, speed);
  setWheelDirection(leftDirection, PIN_MOTOR_A_INPUT2, PIN_MOTOR_A_INPUT1);
  setWheelDirection(rightDirection, PIN_MOTOR_B_INPUT4, PIN_MOTOR_B_INPUT3);
  delay(time);
}

/**
 * Sets the wheel specified by pinForward nd pinReverse to the direction specified.
 * 
 * @param int direction The direction to turn the wheel, as a WHEEL_* constant
 * @param int pinForward The pin responsible for controlling foward movement
 * @param int pinReverse The pin responsible for controlling rear movement
 */
void setWheelDirection(int direction, int pinForward, int pinReverse)
{
  if (DEBUG_DISABLE_MOTOR) {
    return;
  }
  
  if (direction == WHEEL_FORWARD) {
    digitalWrite(pinForward, HIGH);
    digitalWrite(pinReverse, LOW);
  } else if (direction == WHEEL_REVERSE) {
    digitalWrite(pinForward, LOW);
    digitalWrite(pinReverse, HIGH);
  } else  {
    digitalWrite(pinForward, LOW);
    digitalWrite(pinReverse, LOW);
  }
}

/**
 * Read the ultrasonic sensor and get a distance measurement to the wall in cm.
 * 
 * @see https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
 * 
 * @return float 
 */
float readUltrasonicSensorDistance()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse.
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  float duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH);
 
  // Convert the time into a distance: the duration is the time 
  // taken for the sound to be sent, then bounce back and be received 
  // again. The speed of sound is 34.3 centermeters per millisecond.
  float cm = (duration / 2) / 34.3;

  Serial.print(duration);
  Serial.print("ms");
  Serial.print(cm);
  Serial.print("cm");
  
  // if the sensor returned a 0 reading, report a very long distance away
  if (cm == 0) {
    cm = 1000;
    Serial.print("faking 1000 cm");
  }
  Serial.println();

  return cm;
}

/**
 * Set up the ultrasonic sensor
 * 
 * The sensor sends a trigger via an output pin and receives the echo
 * via an input pin.
 * 
 * Note that it's also possible to use two digital outputs to supply 
 * GND and 5V to the sensor. If you're doing this, also set the pin 
 * that will deliver 5V to HIGH, and the pin the will deliver GND to LOW.
 */
void setupUltrasonic() 
{
  // Uncomment these lines and set the constants at the
  // top of the file if you are providing GND and 5V from
  // the digital pins themselves.  
  //
  // Set to LOW to provide a ground
  //  pinMode(PIN_ULTRASONIC_GND, OUTPUT);
  //  digitalWrite(PIN_ULTRASONIC_GND, LOW);
  //
  // Set to HIGH to provide 5V to the sensor
  //  pinMode(PIN_ULTRASONIC_5V, OUTPUT);
  //  digitalWrite(PIN_ULTRASONIC_5V, HIGH);

  // Trigger is an output; the echo is a input
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
}

/**
 * Set up Motor
 * 
 * Configures the output pins and sets everything to off.
 */
void setupMotor() 
{
  if (DEBUG_DISABLE_MOTOR) {
    return;
  }
  
  pinMode(PIN_MOTOR_A_ENABLE, OUTPUT);
  pinMode(PIN_MOTOR_A_INPUT1, OUTPUT);
  pinMode(PIN_MOTOR_A_INPUT2, OUTPUT);
  pinMode(PIN_MOTOR_B_ENABLE, OUTPUT);
  pinMode(PIN_MOTOR_B_INPUT3, OUTPUT);
  pinMode(PIN_MOTOR_B_INPUT4, OUTPUT);

  digitalWrite(PIN_MOTOR_A_ENABLE, LOW);
  digitalWrite(PIN_MOTOR_A_INPUT1, LOW);
  digitalWrite(PIN_MOTOR_A_INPUT2, LOW);
  digitalWrite(PIN_MOTOR_B_ENABLE, LOW);
  digitalWrite(PIN_MOTOR_B_INPUT3, LOW);
  digitalWrite(PIN_MOTOR_B_INPUT4, LOW);
}


