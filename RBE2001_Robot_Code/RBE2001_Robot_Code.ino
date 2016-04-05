#include <QTRSensors.h>
#include <Servo.h>
#include <Encoder.h>

boolean test = false;
boolean testtest = false;
int testState = 4;

/*
  Field setup

      (rails)    1   2   3   4
                   (Storage)
  (Top Left)     T   T   T   T    (Top Right)   (+ zone)
                 |   |   |   |
  [R]----|-------|---|---|---|----|-------[R] (rail) 5
                 |   |   |   |
  (Bottom Left)  T   T   T   T  (Bottom Right)  (- zone)
                   (SUPPLY)
   (zones)   0     1   2   3    4
*/

/*Motors are set up like so
      Front
     M1* *M2
 *   *    *
 *   *    *
     M4* *M3
      Back
*/

// Defining Pin Numbers
// 0
// 1
// 2
#define frontBumperPin 3
// 4 is weird
#define armMotorPin 5
#define motor4Pin 6
#define intakePin 7
#define motor3Pin 8
#define motor2Pin 9
// 10 is broken
#define motor1Pin 11
// 12
// THRU
// 17
#define BTInterrupt 18
#define LEFT_ENCODER_PIN 19
#define RIGHT_ENCODER_PIN 20
// THRU
// 26
#define startButtonPin 27
// 28
#define LEDPin 29
#define QTRSensor0Pin 30
#define QTRSensor1Pin 31
#define QTRSensor2Pin 32
#define QTRSensor3Pin 33
#define QTRSensor4Pin 34
#define QTRSensor5Pin 35
#define QTRSensor6Pin 36
#define QTRSensor7Pin 37
// 38
#define BTSignal1 39
#define BTSignal2 40
#define BTSignal4 41
#define BTSignal8 42
#define BTSend 43
#define BTStartPin 44
#define BTStopPin 45
#define status1Pin 46
#define status2Pin 47
#define status4Pin 48
#define status8Pin 49
// 50
// THRU
#define musicPin1 51
#define musicPin2 52
#define debugLED 53

//define the states that will control the robots actions
#define NO_STATE 0                    // Initial condition for lastState
#define WAIT_FOR_START_BUTTON 19      // Wait for the start button to be pressed
#define INITIALIZE_BLUETOOTH 20       // Starts the bluetooth send/receive
#define BTWait 16                     // Wait for the start signal
/********************************************************************************/
#define GOTO_REACTOR1 1               // Line follow forward indefinitely with intake running
#define GET_SPENT_ROD1 2               // Stops and runs intake then raises arm
#define GOTO_STORAGE1 3                // Update bluetooth, turn around, drive to desired line, turn and drive until limit switch is hit
#define DISPENSE_SPENT_ROD1 4                // Dispense the fuel rod, back up, turn around, drive until a horizontal line is found
#define NAV_DECIDE_DIRECTION1 5        // Decide which way to turn
#define NAV_SUPPLY1_AHEAD 6            // Drive forward indefinitely
#define NAV_SUPPLY1_RIGHT1 7           // Navigate to supply tube that is ONE position to the RIGHT
#define NAV_SUPPLY1_RIGHT2 8           // Navigate to supply tube that is TWO positions to the RIGHT
#define NAV_SUPPLY1_LEFT1 9            // Navigate to supply tube that is ONE positions to the LEFT
#define NAV_SUPPLY1_LEFT2 10           // Navigate to supply tube that is TWO positions to the LEFT
#define GET_NEW_ROD1 11                // Retrieve a new fuel rod
#define GOTO_REACTOR1_AGAIN 12        // Go to the first reactor with the new fuel rod
#define DISPENSE_NEW_ROD1 13           // Place a new fuel rod in reactor
/********************************************************************************/
#define GOTO_REACTOR2 15               // Line follow forward indefinitely with intake running
#define GET_SPENT_ROD2 17               // Stops and runs intake then raises arm
#define GOTO_STORAGE2 18               // Update bluetooth, turn around, drive to desired line, turn and drive until limit switch is hit
#define DISPENSE_SPENT_ROD2 21                // Dispense the fuel rod, back up, turn around, drive until a horizontal line is found
#define NAV_DECIDE_DIRECTION2 22        // Decide which way to turn
#define NAV_SUPPLY2_AHEAD 23            // Drive forward indefinitely
#define NAV_SUPPLY2_RIGHT1 24           // Navigate to supply tube that is ONE position to the RIGHT
#define NAV_SUPPLY2_RIGHT2 25           // Navigate to supply tube that is TWO positions to the RIGHT
#define NAV_SUPPLY2_LEFT1 26            // Navigate to supply tube that is ONE positions to the LEFT
#define NAV_SUPPLY2_LEFT2 27           // Navigate to supply tube that is TWO positions to the LEFT
#define GET_NEW_ROD2 28                // Retrieve a new fuel rod
#define GOTO_REACTOR2_AGAIN 29        // Go to the first reactor with the new fuel rod
#define DISPENSE_NEW_ROD2 30           // Place a new fuel rod in reactor


//define the robot statuses
//0 not used
#define ROBOT_STOPPED 1               // The robot is stopped
#define ROBOT_MOVING_AUTONOMOUS 2     // The robot is moving in autonomous
#define GRIPPER_NO_ROD 3              // There is no rod in the gripper
#define GRIPPER_SPENT_ROD 4           // There is a spent rod in the gripper
#define GRIPPER_NEW_ROD 5             // There is a new rod in the gripper
#define OPERATION_GRIP_ATTEMPT 6      // The robot is attempting to grip a rod
#define OPERATION_GRIP_RELEASE 7      // The robot is attempting to release a rod
#define OPERATION_DRIVETO_REACTOR 8   // The robot is navigating to a reactor 
#define OPERATION_DRIVETO_STORAGE 9   // The robot is navigating to a storage tube
#define OPERATION_DRIVETO_SUPPLY 10   // The robot is navigating to a supply tube
#define OPERATION_IDLE 11             // The robot is idling
//12-15 not used

volatile int currentState;
volatile int lastState;

//defining values for max forward speed, max reverse speed, and stop
int motorMaxF = 60;
int motorMaxR = 120;
int motorStop = 90;

//defining values for half forward speed and half reverse speed
int motorHalfF = ((motorStop - motorMaxF) / 2) + motorMaxF;
int motorHalfR = ((motorMaxR - motorStop) / 2) + motorStop;

//defines a type to store the motor pin, max forward speed,
//max reverse speed, and stop
typedef struct {
  int pin;
  int maxF;
  int maxR;
  int stopped;
  int halfF;
  int halfR;
} MotorData;

//setting values for the motors
const MotorData M1 = {motor1Pin, motorMaxR, motorMaxF, motorStop, motorHalfR, motorHalfF};
const MotorData M2 = {motor2Pin, motorMaxF - 2, motorMaxR + 2, motorStop, motorHalfF, motorHalfR};
const MotorData M3 = {motor3Pin, motorMaxF - 2, motorMaxR + 2, motorStop, motorHalfF, motorHalfR};
const MotorData M4 = {motor4Pin, motorMaxR, motorMaxF, motorStop, motorHalfR, motorHalfF};
const MotorData in = {intakePin, 0, 180, motorStop, 45, 135};
//for the arm, maxF is raised, maxR is lowered, stopped is lowered
const MotorData arm = {armMotorPin, 180, 10, 180, 10, 180};

const int backupDelay = 45;

//Temporary time values
const int turn90 = 500;           //how long it takes for the bot to turn 90 degrees
const int armRaise = 500;         //how long it takes for the bot to raise the arm

//const int armPotLow = 140;      // default 140
//const int armPotHigh = 283;     // default 283

boolean changeSides = false;      //false means the robot is servicing the left side reactor, true means right side
char robotStatus;                 //robot status for sending Bluetooth updates to be sent to Bluetooth Uno

Servo motor1, motor2, motor3, motor4, intakeMotor, armMotor;

volatile int leftCount, rightCount;

//creating a line sensor object
//pins are {ARRAY_OF_PIN_NUMBERS[0-7], NUMBER_OF_SENSORS, TIMEOUT, LED_PIN}
QTRSensorsRC qtr((unsigned char[]) {
  QTRSensor0Pin, QTRSensor1Pin, QTRSensor2Pin, QTRSensor3Pin,
                 QTRSensor4Pin, QTRSensor5Pin, QTRSensor6Pin, QTRSensor7Pin
},
8, 2500, LEDPin);
unsigned int sensors[8];  //output array to hold the current value of each sensor

//Bluetooth communications
const int BTBitsIn = 4;                 // number of lines used for bluetooth communication
boolean BTStorageSignal[BTBitsIn];      // array to hold the storage tube positions
boolean BTSupplySignal[BTBitsIn];       // array to hold the supply tube positions
int BTSignal[BTBitsIn] = {BTSignal1, BTSignal2, BTSignal4, BTSignal8};
int radAlert = 0;                       // 0 means no radiation, 1 means spent fuel rod, 2 means new fuel rod

//int of the rod to get
int storageRodPosition = 1;
int supplyRodPosition = 1;

/*field navigation values
  currenRail takes values from -5 to +5 excluding 0
  5 is the main horizontal rail, 1-4 are supply-storage lines 1-4
  polarity defines direction. + is pointing towards storage tubes
  - is pointint to supply tubes

  currentZone takes values from -4 to 5 including 0
  zones 0-4 are the spaces between/around the lines
  0 is from the reactor on the left side of the field to Rail 1
  4 is from the reactor on the right side of the field to Rail 4
  Polarity defines whether the robot is above or below Rail 5
  when the robot is not on Rail 5, the zone is 5
*/
volatile int currentRail = 5;
volatile int currentZone = 0;

//arm states
boolean fuelRodLoaded = false;
boolean armRaised = false;

boolean startButtonFlag = false;
boolean calibrationFlag = false;

int pos;

/*************************************************************************/

void setup() {
  currentState = WAIT_FOR_START_BUTTON;          // Waiting to have the GO button pressed
  lastState = NO_STATE;                          // There was no previous state
  Serial.begin(9600);

  //attach the motors
  motor1.attach(M1.pin, 1000, 2000);
  motor2.attach(M2.pin, 1000, 2000);
  motor3.attach(M3.pin, 1000, 2000);
  motor4.attach(M4.pin, 1000, 2000);
  intakeMotor.attach(in.pin, 1000, 2000);
  armMotor.attach(arm.pin, 1000, 2000);

  // declare inputs
  pinMode(frontBumperPin, INPUT_PULLUP);
  pinMode(startButtonPin, INPUT_PULLUP);

  for (int j = 0; j < BTBitsIn; j++) {
    pinMode(BTSignal[j], INPUT);
    Serial.println(BTSignal[j]);
  }

  // declare outputs and write them low
  pinMode(BTStartPin, OUTPUT);
  digitalWrite(BTStartPin, LOW);
  pinMode(BTSend, OUTPUT);
  digitalWrite(BTSend, LOW);
  pinMode(BTStopPin, INPUT);
  digitalWrite(BTStopPin, LOW);
  pinMode(53, OUTPUT);
  digitalWrite(53, LOW);

  //attach the external interrupts
  attachInterrupt(digitalPinToInterrupt(frontBumperPin), frontBumper, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTInterrupt), BTInterruption, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), rightEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), leftEncoderISR, RISING);


  botAllStop();
  delay(50);
}


/**********************************************************************************/

void loop() {

  if (test == false) {
    switch (currentState) {

      //polls for the start button
      case WAIT_FOR_START_BUTTON:
        if (!digitalRead(startButtonPin)) {
          lastState = currentState;
          currentState = INITIALIZE_BLUETOOTH;
        }
        break;

      //send the GO signal to the bluetooth module
      case INITIALIZE_BLUETOOTH:
        sendStatusUpdate();
        lastState = currentState;
        currentState = GOTO_REACTOR1;
        break;

      //line follow forward indefinitely with intake runnning
      case GOTO_REACTOR1:
        intake();
        robotStatusUpdate(OPERATION_DRIVETO_REACTOR);
        driveToNext(1);     //
        while (currentState == GOTO_REACTOR1) {
          lineFollowF();
        }
        lastState = currentState;
        break;  // the state is updated when the limit switch is activated

      //stop, pick up the fuel rod, and raise the arm
      case GET_SPENT_ROD1:
        digitalWrite(53, LOW);
        delay(10);
        botStop();
        robotStatusUpdate(ROBOT_STOPPED);
        intake(1700);           // run the intake for 1.7 seconds
        robotStatusUpdate(OPERATION_GRIP_ATTEMPT);
        robotStatusUpdate(GRIPPER_SPENT_ROD);
        raiseArm();
        lastState = currentState;
        currentState = GOTO_STORAGE1;
        break;

      //update Bluetooth, turn around and drive until get to desired line
      //turn and drive indefinitely
      case GOTO_STORAGE1:
        getBluetooth();
        storageRodPosition = storageDecide();
        supplyRodPosition = supplyDecide();
        deadReckon(1, backupDelay);                      // reverse slightly to allow for a full rotation of the robot
        robotStatusUpdate(9);                    // driving to storage tube
        aboutFace();                             // Turn around 180 degrees
        intake(250);
        driveToNext(storageRodPosition);
        botStop();
        botTurnLeft90();
        deadReckon(0, 50);
        while (currentState == GOTO_STORAGE1) {
          lineFollowF();
        }
        lastState = currentState;
        break;                                  // the state is updated when the limit switch is activated

      //dispense the fuel rod, back up and turn around
      //drive until hit a line
      case DISPENSE_SPENT_ROD1:
        outtake(2100);
        robotStatusUpdate(3);                        // no rod in intake
        delay(10);
        deadReckon(1, backupDelay);
        robotStatusUpdate(10);                       // driving to supply tube
        aboutFace();
        deadReckon(1, backupDelay / 3);
        lastState = currentState;
        currentState = NAV_DECIDE_DIRECTION1;
        break;

      //decide which way to turn
      //jumps to the approrpiate case to deal with the current situation
      case NAV_DECIDE_DIRECTION1:
        currentState = decideTurn();
        driveToNext(1);
        break;

      /***************************************************************************/

      //drive forward indefinitely
      case NAV_SUPPLY1_AHEAD:
        while (currentState == NAV_SUPPLY1_AHEAD) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the right 1 position
      case NAV_SUPPLY1_RIGHT1:
        botTurnRight90();
        driveToNext(1);
        botTurnLeft90();
        while (currentState == NAV_SUPPLY1_RIGHT1) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the right 2 positions
      case NAV_SUPPLY1_RIGHT2:
        botTurnRight90();
        driveToNext(2);
        botTurnLeft90();
        while (currentState == NAV_SUPPLY1_RIGHT2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the left 1 position
      case NAV_SUPPLY1_LEFT1:
        botTurnLeft90();
        driveToNext(1);
        botTurnRight90();
        while (currentState == NAV_SUPPLY1_LEFT1) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the left 2 positions
      case NAV_SUPPLY1_LEFT2:
        botTurnLeft90();
        driveToNext(2);
        botTurnRight90();
        while (currentState == NAV_SUPPLY1_LEFT2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      /******************************************************************/

      //retrieve a new fuel rod and turn around
      case GET_NEW_ROD1:
        digitalWrite(53, LOW);
        intake(2000);
        robotStatusUpdate(5);         // new rod in intake
        deadReckon(1, backupDelay);
        robotStatusUpdate(8);         // driving to reactor
        aboutFace();
        deadReckon(1, backupDelay / 3);
        lastState = currentState;
        currentState = GOTO_REACTOR1_AGAIN;
        break;

      //bring the new fuel rod to reactor
      case GOTO_REACTOR1_AGAIN:
        driveToNext(1);
        botTurnLeft90();
        lowerArm();
        while (currentState == GOTO_REACTOR1_AGAIN) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //place new fuel rod in reactor
      case DISPENSE_NEW_ROD1:
        deadReckon(1, 1);
        outtake(2100);
        robotStatusUpdate(3);        // no rod in intake
        delay(10);
        deadReckon(1, backupDelay);
        aboutFace();
        robotStatusUpdate(8);        // driving to reactor
        lastState = currentState;
        break;

      /************************************************************************/

      //get other fuel rod
      case GOTO_REACTOR2:
        intake();
        changeSides = true;
        deadReckon(0, 50);
        driveToNext(5);
        while (currentState == GOTO_REACTOR2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //pick up fuel rod 2 and raise arm
      case GET_SPENT_ROD2:
        botStop();
        intake(2000);
        raiseArm();
        lastState = currentState;
        currentState = GOTO_STORAGE2;
        break;

      //navigate to storage tube
      case GOTO_STORAGE2:
        getBluetooth();
        storageRodPosition = storageDecide();
        supplyRodPosition = supplyDecide();
        deadReckon(1, backupDelay);
        aboutFace();
        driveTo(storageRodPosition);
        botStop();
        botTurnRight90();
        while (currentState == GOTO_STORAGE2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      case DISPENSE_SPENT_ROD2:
        outtake(2100);
        digitalWrite(53, LOW);
        robotStatusUpdate(3);                        // no rod in intake
        delay(10);
        deadReckon(1, backupDelay);
        robotStatusUpdate(10);                       // driving to supply tube
        aboutFace();
        deadReckon(1, backupDelay);
        driveToNext(1);
        lastState = currentState;
        currentState = NAV_DECIDE_DIRECTION2;
        break;

      //decide which way to turn
      //jumps to the approrpiate case to deal with the current situation
      case NAV_DECIDE_DIRECTION2:
        currentState = decideTurn();
        break;

      /***************************************************************************/

      //drive forward indefinitely
      case NAV_SUPPLY2_AHEAD:
        while (currentState == NAV_SUPPLY2_AHEAD) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the right 1 position
      case NAV_SUPPLY2_RIGHT1:
        driveToNext(1);
        botTurnRight90();
        driveToNext(1);
        botTurnLeft90();
        while (currentState == NAV_SUPPLY2_RIGHT1) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the right 2 positions
      case NAV_SUPPLY2_RIGHT2:
        driveToNext(1);
        botTurnRight90();
        driveToNext(2);
        botTurnLeft90();
        while (currentState == NAV_SUPPLY2_RIGHT2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the left 1 position
      case NAV_SUPPLY2_LEFT1:
        driveToNext(1);
        botTurnLeft90();
        driveToNext(1);
        botTurnRight90();
        while (currentState == NAV_SUPPLY2_LEFT1) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //navigate to supply tube to the left 2 positions
      case NAV_SUPPLY2_LEFT2:
        driveToNext(1);
        botTurnLeft90();
        driveToNext(2);
        botTurnRight90();
        while (currentState == NAV_SUPPLY2_LEFT2) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      case GET_NEW_ROD2:
        intake(2000);
        robotStatusUpdate(5);         // new rod in intake
        deadReckon(1, backupDelay);
        robotStatusUpdate(8);         // driving to reactor
        aboutFace();
        lastState = currentState;
        currentState = GOTO_REACTOR2_AGAIN;
        break;

      //bring the new fuel rod to reactor
      case GOTO_REACTOR2_AGAIN:
        driveToNext(1);
        botTurnRight90();
        lowerArm();
        while (currentState == GOTO_REACTOR1_AGAIN) {
          lineFollowF();
        }
        lastState = currentState;
        break;

      //place new fuel rod in reactor
      case DISPENSE_NEW_ROD2:
        deadReckon(1, 3);
        outtake(2100);
        robotStatusUpdate(3);        // no rod in intake
        delay(10);
        deadReckon(1, backupDelay);
        aboutFace();
        deadReckon(1, backupDelay);
        robotStatusUpdate(8);        // driving to reactor
        lastState = currentState;
        break;

      //wait for the start signal
      case BTWait:
        getBTStart();
        break;
    }
  }
  else { // Test code: will run if test = true
    switch (testState) {
      case 1: // Turn around
        aboutFace();
        delay(1000000);
        break;

      case 2: // Output rod vertically
        fuelRodLoaded = true;
        outtake(2000);
        delay(1000000);
        break;
      case 3:
        lineFollowF();
        break;
      case 4:
        if (digitalRead(startButtonPin) == LOW) {
          testState = 7;
        }
        break;

      case 5:
        readAllSensors();
        for (unsigned char i = 0; i < 8; i++)
        {
          Serial.print(sensors[i]);
          Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
        }
        Serial.println();
        break;

      case 6:
        getBluetooth();
        BTDebug();
        Serial.print("Storage Decide: ");
        Serial.println(storageDecide());
        Serial.print("Supply Decide: ");
        Serial.println(supplyDecide());
        Serial.println();
        delay(1000);
        break;

      case 7:
        getBluetooth();
        BTDebug();
        delay(1000);
        break;
    }
  }

}











