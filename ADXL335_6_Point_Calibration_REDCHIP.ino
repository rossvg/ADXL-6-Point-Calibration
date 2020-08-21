// ADXL 6-Point Calibration and Data Polling                May, 2018
// Author: Ross Vrana-Godwin
// Description:
/*
    This program calibrates and tracks data for the ADXL335 accelerometer.
    Data is calibrated using the 6-point method, and will only work for
    the specific chip that was measured. Calibration values are constants.
    Data will be taken for 10 seconds after the program is started by the
    user. Collected data is shown on the serial monitor where the columns
    are in order from x, y, z, and then the current elapsed time. All data
    has been converted to the g units for better readability.
    Future Work:
    Calibrate the chip automatically, be able to export values into MatLab
    or excel without copy/paste
*/



// Constants representing the pinout of ADXL335 connected to analog Arduino pins
const int xPin = A3;
const int yPin = A2;
const int zPin = A1;

// These three variabls below keep track of elapsed time
unsigned int timer = 0; // actual time elapsed
unsigned int prevCount = 0; // used in time checking if statement
unsigned int counter = 0; // used in time checking if statement

void setup() {
  Serial.begin(9600); // begin output communications

  WaitForStart(); // wait until a 2 is sent in serial monitor to begin collecting and showing data

}

void loop() { // begin main program


  // gather data from xyz pins on chip
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int z = analogRead(zPin);

  // calibration values
  float xZeroGrav = 331.5;
  float yZeroGrav = 333;
  float zZeroGrav = 334;

  // calibration values
  float xSensitivity = 67.5;
  float ySensitivity = 67;
  float zSensitivity = 66;

  // display calibrated values on serial monitor, converted to into g units
  Serial.print(((float)x - xZeroGrav) / xSensitivity);
  // print a tab between values:
  Serial.print("g\t");
  Serial.print(((float)y - yZeroGrav) / ySensitivity);
  // print a tab between values:
  Serial.print("g\t");
  Serial.print(((float)z - zZeroGrav) / zSensitivity);
  Serial.print("g\t");

  // check if time has been delayed 4 times. this will equal one second elapsed
  if ((counter - prevCount) == 4) {
    prevCount = counter;
    timer = timer + 1;
    Serial.print("TIME: ");
    Serial.print(timer); // print timer value
    Serial.print("s");
    Serial.println();
  }

  else {
    Serial.println();
  }

  pauseProgramCheck(); // user may pause the program by sending a 1 from to the serial monitor
  delay(250); // data is polled at 250ms

  counter = counter + 1;

  if (timer == 10) {
    EndProgram();
  }
}

int startProgramCheck() {
  int  incomingByte = Serial.read();
  while (incomingByte != 50) {
    incomingByte = Serial.read(); // check if a two is typed
  }
  return 2;
}

void pauseProgramCheck() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    if (incomingByte == 49) { // 1 = 49 in serial for some reason
      ClearMonitor();
      while (startProgramCheck() != 2) {
        //infinite loop until startProgramCheck returns a 2
      }
    }
    // say what you got:
    Serial.println("Resuming Program");
    //Serial.println(incomingByte, DEC);
  }
}

void ClearMonitor() {
  for (int i = 0; i < 50; i++) {
    Serial.println();
  }
}

void EndProgram() {
  Serial.println("END PROGRAM");
  while (1) {}
}

void WaitForStart() {
  Serial. println("Send a 2 in the serial monitor to begin the program");
  int  incomingByte = Serial.read();
  while (incomingByte != 50) {
    incomingByte = Serial.read(); // check if a two is typed
  }
}

