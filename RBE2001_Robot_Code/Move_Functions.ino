//makes the robot face the other direction
//inverts the rail number
void aboutFace() {
  char isTurnDone = false;
  resetEncoders();
  int currentLeftTicks = leftCount;
  int currentRightTicks = rightCount;
  while ((leftCount - currentLeftTicks < 90) && (rightCount - currentRightTicks < 90)) {
    motor1.write(M1.maxF);
    motor2.write(M2.maxR);
    motor3.write(M3.maxR);
    motor4.write(M4.maxF);
    //  char isTurnDone = false;
    //  int currentLeftCount = leftCount;
    //
    //  //pivot backwards until the two ends of the line sensor board are on the black line
    //  while (leftCount - currentLeftCount < 125) {
    //    botPivotBackwardsLeft();
    //  }
    //  botStop();
    //
    //  int currentRightCount = rightCount;
    //
    //  //pivot forwards until the line is back in the middle of the line sensor
    //  while (rightCount - currentRightCount < 100) {
    //    botPivotRight();
  }
  while (!isTurnDone) {
    motor1.write(M1.halfF);
    motor2.write(M2.halfR);
    motor3.write(M3.halfR);
    motor4.write(M4.halfF);
    int leftSide = 0;
    int rightSide = 0;
    readAllSensors();
    for (int i = 0; i < 3; i++) {
      leftSide += sensors[i];
      rightSide += sensors[i + 5];
    }
    int middle = sensors[3] + sensors[4];
    if (middle > leftSide && middle > rightSide) {
      isTurnDone = true;
    }
  }
  botStop();

  //invert currentRail
  currentRail = (-1) * currentRail;
}


//moves the bot backward for unspecified amount of time
void botBackward() {
  motor1.write(M1.maxR);
  motor2.write(M2.maxR);
  motor3.write(M3.maxR);
  motor4.write(M4.maxR);
}


//moves the bot forward for unspecified amount of time
void botForward(int ticks) {
  resetEncoders();
  int currentLeftTicks = leftCount;
  int currentRightTicks = rightCount;
  while ((leftCount - currentLeftTicks < ticks) && (rightCount - currentRightTicks < ticks)) {
    motor1.write(M1.maxF);
    motor2.write(M2.maxF);
    motor3.write(M3.maxF);
    motor4.write(M4.maxF);
  }
}


//stops the robot
void botStop() {
  motor1.write(M1.stopped);
  motor2.write(M2.stopped);
  motor3.write(M3.stopped);
  motor4.write(M4.stopped);
}


//stops the robot
void botAllStop() {
  motor1.write(M1.stopped);
  motor2.write(M2.stopped);
  motor3.write(M3.stopped);
  motor4.write(M4.stopped);
  intakeMotor.write(in.stopped);
  holdArm();
}


//turns the bot left indefinitely
void botTurnLeft() {
  motor1.write(M1.maxR);
  motor2.write(M2.maxF);
  motor3.write(M3.maxF);
  motor4.write(M4.maxR);
}


//turns the robot to the right indefinitely
void botTurnRight() {
  motor1.write(M1.maxF);
  motor2.write(M2.maxR);
  motor3.write(M3.maxR);
  motor4.write(M4.maxF);
}


//turns the robot a little bit to the left
void botTurnSlightLeft() {
  motor1.write(M1.halfF);
  motor2.write(M2.maxF);
  motor3.write(M3.maxF);
  motor4.write(M4.halfF);
}


//turns the robot a little bit to the right
void botTurnSlightRight() {
  motor1.write(M1.maxF);
  motor2.write(M2.halfF);
  motor3.write(M3.halfF);
  motor4.write(M4.maxF);
}


//pivots about M2
void botPivotRight() {
  motor1.write(M1.maxF);
  motor2.write(M2.stopped);
  motor3.write(M3.stopped);
  motor4.write(M4.maxF);
}


//pivots about M1
void botPivotLeft() {
  motor1.write(M1.stopped);
  motor2.write(M2.maxF);
  motor3.write(M3.maxF);
  motor4.write(M4.stopped);
}


//pivots about M2
void botPivotBackwardsRight() {
  motor1.write(M1.maxR);
  motor2.write(M2.stopped);
  motor3.write(M3.stopped);
  motor4.write(M4.maxR);
}


//pivots about M1
void botPivotBackwardsLeft() {
  motor1.write(M1.stopped);
  motor2.write(M2.maxR);
  motor3.write(M3.maxR);
  motor4.write(M4.stopped);
}


//moves the robot a direction for a duration
//direction 0 moves bot forward for duration
//direction 1 moves bot backward for duration
/*
  THIS SHOULD ONLY BE USED FOR A SHORT WHILE AND NEVER TO MOVE ACROSS ZONES OR ONTO NEW RAILS
*/
void deadReckon(int direc, int ticks) {
  resetEncoders();
  int currentLeftTicks = leftCount;
  int currentRightTicks = rightCount;
  switch (direc) {
    case 0:
      while ((leftCount - currentLeftTicks < ticks) && (rightCount - currentRightTicks < ticks)) {
        motor1.write(M1.maxF);
        motor2.write(M2.maxF);
        motor3.write(M3.maxF);
        motor4.write(M4.maxF);
      }
      botStop();
      break;

    case 1:
      while ((leftCount - currentLeftTicks < ticks) && (rightCount - currentRightTicks < ticks)) {
        motor1.write(M1.maxR);
        motor2.write(M2.maxR);
        motor3.write(M3.maxR);
        motor4.write(M4.maxR);
      }
      botStop();
      break;

    default:
      botStop();
      break;
  }
}


//turns the robot 90 degrees to the left
void botTurnLeft90() {
  char isTurnDone = false;
  resetEncoders();
  int currentLeftTicks = leftCount;
  int currentRightTicks = rightCount;
  while ((leftCount - currentLeftTicks < 45) && (rightCount - currentRightTicks < 45)) {
    motor1.write(M1.maxR);
    motor2.write(M2.maxF);
    motor3.write(M3.maxF);
    motor4.write(M4.maxR - 5);
  }
  
  while (!isTurnDone) {
    motor1.write(M1.halfR);
    motor2.write(M2.halfF);
    motor3.write(M3.halfF);
    motor4.write(M4.halfR);
    int leftSide = 0;
    int rightSide = 0;
    readAllSensors();
    int middle = sensors[3] + sensors[4];
    for (int i = 0; i < 3; i++) {
      leftSide += sensors[i];
      rightSide += sensors[i + 5];
    }
    if (middle > leftSide && middle > rightSide) {
      isTurnDone = true;
    }
  }

  botStop();

  //This code isn't being implemented currently due to a bug somewhere
  //decide what to set the new currentRail and currentZone to
  switch (currentRail) {
    //on rail 5 going negative turning left onto: rail 1, 2, 3, 4
    case -5:
      switch (currentZone) {
        //in zone 0 turning left onto rail 1
        case 0:
          currentRail = -1;
          break;

        //in zone 1 turning left onto rail 2
        case 1:
          currentRail = -2;
          break;

        //in zone 2 turning left onto rail 3
        case 2:
          currentRail = -3;
          break;

        //in zone 3 turning left onto rail 4
        case 3:
          currentRail = -4;
          break;

        default :
          //shouldn't get here
          break;

      }
      //currentRail != 5 and we are going negative on a side rail so now currentZone should be -5
      currentZone = -5;
      break;

    //on rail 4 going negative turning left onto rail 5
    case -4:
      currentRail = 5;
      currentZone = 4;
      break;

    //on rail 3 going negative turning left onto rail 5
    case -3:
      currentRail = 5;
      currentZone = 3;
      break;

    //on rail 2 going negative turning left onto rail 5
    case -2:
      currentRail = 5;
      currentZone = 2;
      break;

    //on rail 1 going negative turning left onto rail 5
    case -1:
      currentRail = 5;
      currentZone = 1;
      break;

    //on rail 1 going positive turning left onto rail 5
    case 1:
      currentRail = -5;
      currentZone = 0;
      break;

    //on rail 2 going positive turning left onto rail 5
    case 2:
      currentRail = -5;
      currentZone = 1;
      break;

    //on rail 3 going positive turning left onto rail 5
    case 3:
      currentRail = -5;
      currentZone = 2;
      break;

    //on rail 4 going positive turning left onto rail 5
    case 4:
      currentRail = -5;
      currentZone = 3;
      break;

    //on rail 5 going positive turning left onto: rail 1, 2, 3, 4
    case 5:
      switch (currentZone) {
        //in zone 1 turning left onto rail 1
        case 1:
          currentRail = 1;
          break;

        //in zone 2 turning left onto rail 2
        case 2:
          currentRail = 2;
          break;

        //in zone 3 turning left onto rail 3
        case 3:
          currentRail = 3;
          break;

        //in zone 4 turning left onto rail 4
        case 4:
          currentRail = 4;
          break;

        default :
          //shouldn't get here
          break;

      }
      //currentRail != 5 and we are going positive on a side rail so now currentZone should be 5
      currentZone = 5;
      break;

    default:
      break;
  }
}


//turns the robot 90 degrees to the right
void botTurnRight90() {
  char isTurnDone = false;
  resetEncoders();
  int currentLeftTicks = leftCount;
  int currentRightTicks = rightCount;
  while ((leftCount - currentLeftTicks < 45) && (rightCount - currentRightTicks < 45)) {
    motor1.write(M1.maxF);
    motor2.write(M2.maxR);
    motor3.write(M3.maxR);
    motor4.write(M4.maxF);
    delay(1);
  }
  
  while (!isTurnDone) {
    motor1.write(M1.halfF);
    motor2.write(M2.halfR);
    motor3.write(M3.halfR);
    motor4.write(M4.halfF);
    int leftSide = 0;
    int rightSide = 0;
    readAllSensors();
    int middle = sensors[3] + sensors[4];
    for (int i = 0; i < 3; i++) {
      leftSide += sensors[i];
      rightSide += sensors[i + 5];
    }
    if (middle > leftSide && middle > rightSide) {
      isTurnDone = true;
    }
  }

  botStop();

  //This code isn't being implemented currently due to a bug somewhere
  //decide what to set the new currentRail and currentZone to
  switch (currentRail) {
    //on rail 5 going negative turning right onto: rail 1, 2, 3, 4
    case -5:
      switch (currentZone) {
        //in zone 0 turning right onto rail 1
        case 0:
          currentRail = 1;
          break;

        //in zone 1 turning right onto rail 2
        case 1:
          currentRail = 2;
          break;

        //in zone 2 turning right onto rail 3
        case 2:
          currentRail = 3;
          break;

        //in zone 3 turning right onto rail 4
        case 3:
          currentRail = 4;
          break;

        default :
          //shouldn't get here
          break;

      }
      //currentRail != 5 and we are going positive on a side rail so now currentZone should be 5
      currentZone = 5;
      break;

    //on rail 4 going negative turning left onto rail 5
    case -4:
      currentRail = -5;
      currentZone = 4;
      break;

    //on rail 3 going negative turning left onto rail 5
    case -3:
      currentRail = -5;
      currentZone = 3;
      break;

    //on rail 2 going negative turning left onto rail 5
    case -2:
      currentRail = -5;
      currentZone = 2;
      break;

    //on rail 1 going negative turning left onto rail 5
    case -1:
      currentRail = -5;
      currentZone = 1;
      break;

    //on rail 1 going positive turning left onto rail 5
    case 1:
      currentRail = 5;
      currentZone = 0;
      break;

    //on rail 2 going positive turning left onto rail 5
    case 2:
      currentRail = 5;
      currentZone = 1;
      break;

    //on rail 3 going positive turning left onto rail 5
    case 3:
      currentRail = 5;
      currentZone = 2;
      break;

    //on rail 4 going positive turning left onto rail 5
    case 4:
      currentRail = 5;
      currentZone = 3;
      break;

    //on rail 5 going positive turning left onto: rail 1, 2, 3, 4
    case 5:
      switch (currentZone) {
        //in zone 1 turning right onto rail 4
        case 1:
          currentRail = -1;
          break;

        //in zone 2 turning right onto rail 4
        case 2:
          currentRail = -2;
          break;

        //in zone 3 turning right onto rail 4
        case 3:
          currentRail = -3;
          break;

        //in zone 4 turning right onto rail 4
        case 4:
          currentRail = -4;
          break;

        default :
          //shouldn't get here
          break;

      }
      //currentRail != 5 and we are going negative on a side rail so now currentZone should be -5
      currentZone = -5;
      break;
    }
}


//drives forward while following a line indefinitely
void lineFollowF() {
  int leftSide = 0;
  int rightSide = 0;
  readAllSensors();
  for (int i = 0; i < 3; i++) {
    leftSide += sensors[i];
    rightSide += sensors[i + 5];
  }
  int middle = sensors[3] + sensors[4];
  //turns left
  if ((leftSide > middle) && (rightSide < 500)) {
    //the left side sees a line
    motor1.write(M1.halfF);
    motor1.write(M2.halfF - 25);
    motor1.write(M3.halfF - 25);
    motor1.write(M4.halfF);
  }
  //turns right
  if ((rightSide > middle) && (leftSide < 500)) {
    //the right side sees a line
    motor1.write(M1.halfF + 25);
    motor2.write(M2.halfF);
    motor3.write(M3.halfF);
    motor4.write(M4.halfF + 25);
  }
  //goes straight
  if (middle > (leftSide + rightSide)) {
    motor1.write(M1.halfF + 5);
    motor2.write(M2.halfF - 5);
    motor3.write(M3.halfF - 5);
    motor4.write(M4.halfF + 5);
  }
}


//drive to next storage/supply
void driveToNext(int targetIntersect)
{
  int intersect = 0;
  readAllSensors();
  while (intersect < targetIntersect) {
    lineFollowF();
    if (sensors[0] > 1500 && sensors[7] > 1500) {
      intersect++;
      if (intersect < targetIntersect) {
        deadReckon(0, backupDelay / 2);
      }
    }
  }
  deadReckon(0, backupDelay / 2);
  botStop();
}


//drives forward a number of lines while following a line
void lineForward(int lines) {
  int linesDone = 0;
  int linesCounted = 0;
  int lastSensor0, lastSensor7;

  //line follow until the front bumper is pressed
  if (lines == 0) {
    while (currentState != lastState) {
      lineFollowF();
    }
  } else {

    while (linesDone != lines) {
      lineFollowF();

      //if both ends detect a line
      int diff0 = lastSensor0 - sensors[0];
      int diff7 = lastSensor7 - sensors[7];
      if ((diff0 < 50) && (diff7 < 50)) {
        if ((sensors[0] < 50) && (sensors[7] < 0)) {
          linesDone++;
          linesCounted++;
        }
      }

      //This code isn't being implemented currently due to a bug somewhere
      //if we have a new line update to read
      if (linesCounted) {
        //set the new currentZone
        switch (currentRail) {
          /**************On main horizontal going negative*************/
          case -5:
            switch (currentZone) {
              case 4:
                //something's wrong
                break;

              case 3:
                switch (linesCounted) {
                  case 1:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 2:
                switch (linesCounted) {
                  case 1:
                    currentZone = 3;
                    break;

                  case 2:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 1:
                switch (linesCounted) {
                  case 1:
                    currentZone = 2;
                    break;

                  case 2:
                    currentZone = 3;
                    break;

                  case 3:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 0:
                switch (linesCounted) {
                  case 1:
                    currentZone = 1;
                    break;

                  case 2:
                    currentZone = 2;
                    break;

                  case 3:
                    currentZone = 3;
                    break;

                  case 4:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;
            }
            break;

          /**************On main horizontal going positive*************/
          case 5:
            switch (currentZone) {
              case 4:
                //something's wrong
                break;
              case 3:
                switch (linesDone == 1) {
                  case 1:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 2:
                switch (linesCounted) {
                  case 1:
                    currentZone = 3;
                    break;

                  case 2:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 1:
                switch (linesCounted) {
                  case 1:
                    currentZone = 2;
                    break;

                  case 2:
                    currentZone = 3;
                    break;

                  case 3:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;

              case 0:
                switch (linesCounted) {
                  case 1:
                    currentZone = 1;
                    break;

                  case 2:
                    currentZone = 2;
                    break;

                  case 3:
                    currentZone = 3;
                    break;

                  case 4:
                    currentZone = 4;
                    break;

                  default:
                    break;
                }
                break;
            }
            break;

          default:
            break;
        }
        linesCounted = 0;
      }

      lastSensor0 = sensors[0];
      lastSensor7 = sensors[7];
      delay(5);
    }
  }
}


//line follows from a zone to a rail
//takes in an int from -4 to +4 excluding 0
void driveTo(int toRail) {
  int railDiff = toRail - currentRail;
  int railZonediff = toRail - currentZone;
  if (toRail == 0) {
    lineFollowF();
  } else if (railDiff == (toRail * 2)) {
    //robot is on same rail facing wrong way
    aboutFace();
    lineForward(0);

  } else if (railDiff == 0) {
    //robot is on same rail facing right way
    lineForward(0);

  } else if ((currentRail != 5) && (currentRail != -5)) {
    //robot is on the wrong rail
    goToRail5(toRail);

  } else {
    //the robot is on rail 5 and needs to get to the right rail
    int zoneDiff = abs(toRail) - abs(currentZone);
    if (zoneDiff <= 0) {
      zoneDiff = abs(zoneDiff - 1);
    }

    if (railDiff < 0) {
      if (abs(railDiff) > abs(currentRail)) {
        //facing LEFT, turning LEFT
        lineForward(zoneDiff);
        botTurnLeft90();

      } else {
        //facing RIGHT, turning LEFT
        lineForward(zoneDiff);
        botTurnLeft90();

      }
    } else {
      if (abs(railDiff) > abs(currentRail)) {
        //facing LEFT, turning RIGHT\
        lineForward(zoneDiff);
        botTurnRight90();

      } else {
        //facing RIGHT, turning RIGHT
        lineForward(zoneDiff);
        botTurnRight90();

      }
    }
  }
  currentRail = toRail;
  botStop();

}


//This code isn't being implemented currently due to a bug somewhere
//goes to Rail 5 from anywhere
void goToRail5(int rail5) {
  if (currentRail == 5) {
    //congratulations, you're there

  } else if (currentZone != 5) {
    //what even, how did you do this

  } else {

    int diff = 5 - currentRail;
    if (currentZone > 0) {
      //above rail 5

      if (currentRail > 0) {
        //facing wrong direction
        aboutFace();
        lineForward(1);

      } else if (currentRail < 0) {
        //facing right way
        lineForward(1);

      }

      if (rail5 > 0) {
        botTurnRight90();
      } else {
        botTurnLeft90();
      }

    } else if (currentZone < 0) {
      //below Rail 5

      if (currentRail < 0) {
        //facing wrong direction
        aboutFace();
        lineForward(1);

      } else if (currentRail > 0) {
        //facing right way
        lineForward(1);
      }
      if (rail5 > 0) {
        botTurnLeft90();
      } else {
        botTurnRight90();
      }
    }
  }
}


//back up until the robot finds a line
void botFindLineReverse() {
  int line = readSensors();
  if (line < 3000) {
    botBackward();
  } else if (line < 4000) {
    botBackward();
  } else {
    botBackward();
  }
  botStop();
}

