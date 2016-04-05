//gets a position value from the line sensors
int readSensors() {
  return qtr.readLine(sensors, QTR_EMITTERS_ON);
}


//reads the sensors and returns the values to the sensors[] array
void readAllSensors() {
  qtr.read(sensors, QTR_EMITTERS_ON);
}


//ISR for the front button
void frontBumper() {
  digitalWrite(53, LOW);
  //needs additional logic for what to do in
  //certain states for which incrementing is
  botStop();
  lastState = currentState;
  switch (currentState) {

    case GOTO_REACTOR1:
      //the robot is currently going to the first reactor for the first time
      currentState = GET_SPENT_ROD1;
      //the robot is now going to pick up the fuel rod
      break;

    case GOTO_STORAGE1:
      //the robot is currently navigating to the first storage tube
      currentState = DISPENSE_SPENT_ROD1;
      //the robot needs to dispense the spent fuel rod
      break;

    //the robot is navigating to the first supply tube
    case NAV_SUPPLY1_AHEAD:
      currentState = GET_NEW_ROD1;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY1_RIGHT1:
      currentState = GET_NEW_ROD1;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY1_RIGHT2:
      currentState = GET_NEW_ROD1;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY1_LEFT1:
      currentState = GET_NEW_ROD1;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY1_LEFT2:
      currentState = GET_NEW_ROD1;
      //the robot needs to retrieve the new fuel rod
      break;

    case GOTO_REACTOR1_AGAIN:
      //the robot is going back to the first reactor
      currentState = DISPENSE_NEW_ROD1;
      //the robot needs to dispense a new rod
      break;

    /*****************************************************************************************/

    case GOTO_REACTOR2:
      //the robot is currently going to the first reactor for the first time
      currentState = GET_SPENT_ROD2;
      //the robot is now going to pick up the fuel rod
      break;

    case GOTO_STORAGE2:
      //the robot is currently navigating to the first storage tube
      currentState = DISPENSE_SPENT_ROD2;
      //the robot needs to dispense the spent fuel rod
      break;

    //the robot is navigating to the first supply tube
    case NAV_SUPPLY2_AHEAD:
      currentState = GET_NEW_ROD2;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY2_RIGHT1:
      currentState = GET_NEW_ROD2;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY2_RIGHT2:
      currentState = GET_NEW_ROD2;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY2_LEFT1:
      currentState = GET_NEW_ROD2;
      //the robot needs to retrieve the new fuel rod
      break;
      
    case NAV_SUPPLY2_LEFT2:
      currentState = GET_NEW_ROD2;
      //the robot needs to retrieve the new fuel rod
      break;

    case GOTO_REACTOR2_AGAIN:
      //the robot is going back to the first reactor
      currentState = DISPENSE_NEW_ROD2;
      //the robot needs to dispense a new rod
      break;
  }
}


//decides which direction to turn after dropping off a fuel rod
//0 is left, 1 is straight, 2 is right
int decideTurn() {
  int num = storageRodPosition - supplyRodPosition;
  if (changeSides) {
    switch (num) {
      //don't need to move to a new Rail
      case 0:
        return NAV_SUPPLY2_AHEAD;
        break;

      //need to move 1 rail to the right
      case 1:
        return NAV_SUPPLY2_RIGHT1;
        break;

      //need to move 2 rails to the right
      case 2:
        return NAV_SUPPLY2_RIGHT2;
        break;

      //need to move 1 rail to the left
      case -1:
        return NAV_SUPPLY2_LEFT1;
        break;

      //need to move 2 rails to the left
      case -2:
        return NAV_SUPPLY2_LEFT2;
        break;

      default:
        return NAV_SUPPLY2_AHEAD;
        break;

    }
  } else {
    switch (num) {
      //don't need to move to a new Rail
      case 0:
        return NAV_SUPPLY1_AHEAD;
        break;

      //need to move 1 rail to the right
      case 1:
        return NAV_SUPPLY1_RIGHT1;
        break;

      //need to move 2 rails to the right
      case 2:
        return NAV_SUPPLY1_RIGHT2;
        break;

      //need to move 1 rail to the left
      case -1:
        return NAV_SUPPLY1_LEFT1;
        break;

      //need to move 2 rails to the left
      case -2:
        return NAV_SUPPLY1_LEFT2;
        break;

      default:
        return NAV_SUPPLY1_AHEAD;
        break;
    }
  }
}



//polls the start button and sets the flag high if the button is pressed
void startButton() {
  if (!digitalRead(startButtonPin)) {
    lastState = currentState;
    currentState = INITIALIZE_BLUETOOTH;
  } else {
    delay(5);
  }
}


//Counts the ticks of the right side encoder
void rightEncoderISR() {
  rightCount++;
}


//counts the ticks of the left side encoder
void leftEncoderISR() {
  leftCount++;
}


//resets the ticks counted by the encoders
void resetEncoders() {
  rightCount = 0;
  leftCount = 0;
}


//takes in an int and sends out a
void robotStatusUpdate(int botStatus) {
  robotStatus = botStatus;
  sendStatusUpdate();
  switch (botStatus) {
    case 3:
      digitalWrite(musicPin1, HIGH);
      digitalWrite(musicPin2, HIGH);
      digitalWrite(53, LOW);
    case 4:
      digitalWrite(musicPin1, LOW);
      digitalWrite(53, HIGH);
      break;
    case 5:
      digitalWrite(musicPin2, LOW);
      digitalWrite(53, HIGH);
      break;
  }
}

