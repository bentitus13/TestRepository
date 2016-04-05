//prints the bluetooth information to the computer
void BTDebug() {
  Serial.print("Bluetooth storage: ");
  Serial.print(BTStorageSignal[0]);
  Serial.print(" ");
  Serial.print(BTStorageSignal[1]);
  Serial.print(" ");
  Serial.print(BTStorageSignal[2]);
  Serial.print(" ");
  Serial.println(BTStorageSignal[3]);
  Serial.print("Bluetooth supply: ");
  Serial.print(BTSupplySignal[0]);
  Serial.print(" ");
  Serial.print(BTSupplySignal[1]);
  Serial.print(" ");
  Serial.print(BTSupplySignal[2]);
  Serial.print(" ");
  Serial.println(BTSupplySignal[3]);
  Serial.println();
  if (radAlert == 1) {
    Serial.print("Radiation level: LOW");
  } else if (radAlert == 2) {
    Serial.print("Radiation level: HiGH");
  }
}


//if a stop command is issued, the robot will stop and listen for a start command
void BTInterruption() {
  lastState = currentState;
  currentState = BTWait;
}


//sends a signal to other arduino to retrieve the bluetooth information
//listens to the receive pin to get the bluetooth information
void getBluetooth() {
  digitalWrite(BTSend, LOW);                       //tell Bluetooth Uno to send the storage data
  delay(1);
  BTStorageSignal[0] = !digitalRead(BTSignal[0]);    //read in the storage data
  BTStorageSignal[1] = !digitalRead(BTSignal[1]);
  BTStorageSignal[2] = !digitalRead(BTSignal[2]);
  BTStorageSignal[3] = !digitalRead(BTSignal[3]);
  delay(1);
  digitalWrite(BTSend, HIGH);                        //tell Bluetooth Uno to send the supply data
  delay(1);
  BTSupplySignal[0] = digitalRead(BTSignal[0]);     //read in the supply data
  BTSupplySignal[1] = digitalRead(BTSignal[1]);
  BTSupplySignal[2] = digitalRead(BTSignal[2]);
  BTSupplySignal[3] = digitalRead(BTSignal[3]);
  delay(1);
  digitalWrite(BTSend, LOW);                       //set the BTSend pin back to low
}



//waits and polls for the start signal
void getBTStart() {
  while (digitalRead(BTStopPin)) {
    delay(1);
  }
  currentState = lastState;                         // reset the state to what it was before the stop command
}


//decides which storage tube to get
int storageDecide() {
  if (changeSides) {                    //if the robot is servicing the right side reactor
    if (BTStorageSignal[3]) {          //if the fourth storage tube is available
      return 4;
      
    } else if (BTStorageSignal[2]) {   //if the third storage tube is available
      return 3;
      
    } else if (BTStorageSignal[1]) {   //if the second storage tube is available
      return 2;
      
    } else {
      return 1;                        //the first storage tube HAS to be available
      
    }
  } else {                             //if the robot is servicing the left side reactor
    if (BTStorageSignal[0]) {          //if the first storage tube is available
      return 1;
      
    } else if (BTStorageSignal[1]) {   //if the second storage tube is available
      return 2;
      
    } else if (BTStorageSignal[2]) {   //if the third storage tube is available
      return 3;
      
    } else {
      return 4;                        //the fourth storage tube HAS to be available
    }
  }
}


//decides which supply tube to get
int supplyDecide() {
  if (changeSides) {
    if (BTSupplySignal[3]) {            //if the fourth supply tube is available
      return 4;

    } else if (BTSupplySignal[2]) {     //if the third supply tube is available
      return 3;

    } else if (BTSupplySignal[1]) {                            //the second supply tube HAS to be available
      return 2;

    } else {
      return 1;

    }
  } else {
    if (BTSupplySignal[0]) {            //if the first suply tube is available
      return 1;

    } else if (BTSupplySignal[1]) {     //if the second supply tube is available
      return 2;

    }  else if (BTSupplySignal[2]) {
      return 3;

    } else {                            //the third supply tube HAS to be available
      return 4;
    }
  }
}


//sends the latest status update to Bluetooth Uno
void sendStatusUpdate() {
  digitalWrite(BTStartPin, HIGH);
  digitalWrite(status1Pin, robotStatus & 0x01);
  digitalWrite(status2Pin, robotStatus & 0x02);
  digitalWrite(status4Pin, robotStatus & 0x04);
  digitalWrite(status8Pin, robotStatus & 0x08);
  delay(1);
  digitalWrite(BTStartPin, LOW);
}

