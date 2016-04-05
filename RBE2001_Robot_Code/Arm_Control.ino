//takes in the reactor core if a fuel rod is not loaded
//activates the intake motor for a duration
//sets the fuelRodLoaded to true
void intake(int durat) {
  if (!fuelRodLoaded) {
    intakeMotor.write(in.maxF);
    delay(durat);
    intakeStop();
  }
  fuelRodLoaded = true;
}


//takes in the reactor core
void intake() {
  intakeMotor.write(in.maxF);
}


//spits out the reactor core if a fuel rod is loaded
//activates the intake motor for a duration
//sets the fuel rod to unloaded
void outtake(int durat) {
  if (fuelRodLoaded) {
    intakeMotor.write(in.maxR);
    delay(durat);
    intakeStop();
  }
  fuelRodLoaded = false;
}


//stops the intake motor
void intakeStop() {
  intakeMotor.write(in.stopped);
}


//delay for the iteration of the arm
int armDelay = 4;


//raises the front arm all the way if the arm is raised
//sets the arm as raised
void raiseArm() {
  if (!armRaised) {
    for (int i = arm.maxF; i > arm.maxR; i -= 5) {
      if (i > 180) {
        armMotor.write(180);
      }
      else if (i < 0) {
        armMotor.write(0);
      }
      else {
        armMotor.write(i);
        delay(armDelay);
      }
    }
  }
  armRaised = true;
}


//lowers the front arm of the robot all the way if lowered
//sets the arm as lowered
void lowerArm() {
  if (armRaised) {
    for (int i = arm.maxR; i < arm.maxF; i += 5) {
      if (i > 180) {
        armMotor.write(180);
      }
      else if (i < 0) {
        armMotor.write(0);
      }
      else {
        armMotor.write(i);
        delay(armDelay);
      }
    }
  }
  armRaised = false;
}


//holds the arm up if the arm is raised
//holds the arm down if the arm is lowered
void holdArm() {
  if (armRaised == true) {
    armMotor.write(arm.maxR);
  }
  else if (armRaised == false) {
    //do nothing and let gravity hold the arm down
    armMotor.write(arm.maxF);
  }
}





