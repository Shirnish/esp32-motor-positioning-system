//Converts text into a long integer
#include <stdlib.h>
//Checks for conversion range errors
#include <errno.h>

//Button related assignments
const int homeSwitchPin = 14;
bool lastHomeSwitchPressed = false;
bool stableHomeSwitchPressed = false;
unsigned long lastHomeSwitchChangeMs = 0;
const unsigned long homeDebounceMs = 25;

//Serial Input Variables
String inputLine = "";
bool discardingLine = false;
const unsigned int maxCommandLength = 32;

//Motion variables
long targetPositionSteps = 0;
bool isMoving = false;
unsigned long stepIntervalMs = 5;
unsigned long lastStepMs = 0;
//Homing
bool isHoming = false;
unsigned long homingStartMs = 0;
const unsigned long homingTimeoutMs = 5000;
unsigned long homingStepsTaken = 0;
const unsigned long maxHomingSteps = 256;
unsigned long lastHomingStepMs = 0;
const unsigned long homingStepIntervalMs = 10;
const bool homingForward = false;
bool isHomed = false;



//Motor output pins in IN1 to IN4 order
int outPorts[] = { 1, 2, 42, 41 };

//Keeps track of our place in the coil 0 - 3
int phaseIndex = 0;

//Keeps track of issued steps, not measured shaft position
long commandedPositionSteps = 0;

//Bounds for steps
const long minPositionSteps = 0;
const long maxPositionSteps = 1024;


//Sets up serial, our switch and the motor outputs
void setup() {
  Serial.begin(115200);
  pinMode(homeSwitchPin, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    pinMode(outPorts[i], OUTPUT);
    digitalWrite(outPorts[i], LOW);
  }
  Serial.println("Ready");
}

//Accepts switch changes after the reading stays stable for 25 ms
void updateHomeSwitch(){
  bool pressed = isHomeSwitchPressed();
  unsigned long now = millis();
if(pressed != lastHomeSwitchPressed){
    lastHomeSwitchPressed = pressed;
  lastHomeSwitchChangeMs = now;
  }
    if(now - lastHomeSwitchChangeMs < homeDebounceMs){
      return;
    }
 if (pressed == stableHomeSwitchPressed) {
    return;
  }
stableHomeSwitchPressed = pressed;

if (stableHomeSwitchPressed) {
    Serial.println("Home switch is pressed");
    if(isHoming){
      finishHoming();
    } else{
      stopMotion();
      Serial.println(commandedPositionSteps);
    }
  } else {
    Serial.println("Home switch is unpressed");
  }
}


//Our switch connects the input to ground when pressed
bool isHomeSwitchPressed(){
  return digitalRead(homeSwitchPin) == LOW;
}

//Earlier blocking version, kept for reference and not used by our command path
void movingSteps(int steps, bool forward, unsigned long stepDelay) {
  for (int i = 0; i < steps; i++) {
    stepOnce(forward);
    delay(stepDelay);
    digitalWrite(outPorts[phaseIndex], LOW);
  }
}




//Checks the request and schedules a target for updateMotion
void moveTo(long targetSteps, unsigned long stepDelay) {
  if (isMoving || isHoming) {
    Serial.println("Busy");
    return;
  } else if((!isHomed)){
    Serial.println("Home First");
    Serial.println(commandedPositionSteps);
    return;
    
  }else if (targetSteps == commandedPositionSteps) {
    Serial.println("No Movement");
    Serial.println(commandedPositionSteps);
    return;
  } else {
    if (targetSteps < minPositionSteps || targetSteps > maxPositionSteps) {
      Serial.println("Not Within target Range");
      return;
    } else {
      Serial.println("Within target Range");
    }
    targetPositionSteps = targetSteps;
    stepIntervalMs = stepDelay;
    lastStepMs = millis();
    isMoving = true;
    Serial.println("Movement scheduled");
  }
}



//Changes the active coil and updates our count by one step
void stepOnce(bool forward) {
  digitalWrite(outPorts[phaseIndex], LOW);

  if (forward) {
    phaseIndex++;
    if (phaseIndex >= 4) {
      phaseIndex = 0;
    }
  } else {
    phaseIndex--;
    if (phaseIndex < 0) {
      phaseIndex = 3;
    }
  }

  digitalWrite(outPorts[phaseIndex], HIGH);

  if (forward) {
    commandedPositionSteps++;
  } else {
    commandedPositionSteps--;
  }
}

//Takes one step when its time is due so the other updates can keep running
void updateMotion() {
  unsigned long now = millis();
  if (isMoving == false) {
    return;
  }
  if (now - lastStepMs < stepIntervalMs) {
    return;
  }
  lastStepMs = now;
  if (commandedPositionSteps == targetPositionSteps) {
    digitalWrite(outPorts[phaseIndex], LOW);
    isMoving = false;
    Serial.println(commandedPositionSteps);
    return;
  } else {
    stepOnce(targetPositionSteps > commandedPositionSteps);
  }
}


//Checks for digits with an optional minus sign at the start
bool isInteger(String command) {
  if (command.length() == 0) {
    return false;
  }
  int start = 0;
  if (command.charAt(0) == '-') {
    start = 1;
  }
  if (start == command.length()) {
    return false;
  }
  for (int i = start; i < command.length(); i++) {
    char c = command.charAt(i);
    if (c < '0' || c > '9') {
      return false;
    }
  }
  return true;
}


//Handles named commands first, then checks and converts numeric targets
void handleCommand(String command) {
  command.trim();

  Serial.print("Received: ");
  Serial.println(command);

  if(command == "STOP"){
    stopMotion();
    Serial.println("Stopped");
    Serial.println(commandedPositionSteps);
    return;
  }
  
  if (command == "HOME") {
  startHoming();
  return;
}
if ( command == "STATUS"){
  printStatus();
  return;
}

  if (!isInteger(command)) {
    Serial.println("Invalid Command");
    return;
  }

  //Clears the previous error before checking this conversion
  errno = 0;
  long targetSteps = strtol(command.c_str(), nullptr, 10);
  if (errno == ERANGE) {
    Serial.println("Integer too large");
    return;
  }
  moveTo(targetSteps, 5);
}

//Reads one available character without waiting for the rest of the command
void updateSerial(){
if (Serial.available() == 0) {
  return;
}

char receivedChar = Serial.read();

//Only handles a command after its newline arrives
if (receivedChar == '\n') {
  if (discardingLine) {
    Serial.println("Command too long");
  } else {
    handleCommand(inputLine);
  }

  inputLine = "";
  discardingLine = false;
  return;
}
  
  if(discardingLine){
    return;

  }
  //Discards an overlong line until its newline so it cannot become a partial command
  if( inputLine.length()>= maxCommandLength){
    discardingLine = true;
    return;
  } else {
    inputLine += receivedChar;
  }
}

//Cancels motion and turns off the active coil without resetting our count
void stopMotion(){
  isMoving = false;
  isHoming = false;
  targetPositionSteps = commandedPositionSteps;
  digitalWrite(outPorts[phaseIndex], LOW);
}


//Searches for the switch with a separate step interval and attempt limits
void updateHoming(){
  if(isHoming == false){
    return;
  }
  unsigned long now = millis();
  if(now - homingStartMs >= homingTimeoutMs){
    stopMotion();
    Serial.println("Homing timed out");
    return;
  }
  if(homingStepsTaken >= maxHomingSteps){
    stopMotion();
    Serial.println("Homing step limit reached");
    return;
  }
  //Pauses search steps on the raw press while the debounce check finishes
  bool pressed = isHomeSwitchPressed();
if(pressed){
  return;
}
if(now - lastHomingStepMs < homingStepIntervalMs){
  return;
} else {
  lastHomingStepMs = now;
  stepOnce(homingForward);
  homingStepsTaken++;
}


}

//Starts a new search only when idle and the switch is released
void startHoming(){
  if(isMoving || isHoming){
    Serial.println("Busy");
    return;
  }
  if(isHomeSwitchPressed() || stableHomeSwitchPressed){
    Serial.println("Release Home switch first");
    return;
  } else {
    isHomed = false;
  homingStepsTaken = 0;
  homingStartMs = millis();
  lastHomingStepMs = homingStartMs;
  isHoming = true;
  Serial.println("Homing requested");
}
}

//Stops the search and assigns zero when the home press is accepted
void finishHoming(){
  if(isHoming == false){
    return;
  }
  stopMotion();
    commandedPositionSteps = 0;
    targetPositionSteps = 0;
    isHomed = true;
    Serial.println("Homing Complete");
    Serial.println(commandedPositionSteps);
}

//Prints our current count, home reference and mode on one line
void printStatus(){
  Serial.print("STATUS position=");
  Serial.print(commandedPositionSteps);
  Serial.print(" homed=");
   Serial.print(isHomed);
    Serial.print(" mode=");
if(isHoming){
    Serial.println("HOMING");
} else if(isMoving){
  Serial.println("MOVING");
}else{
  Serial.println("IDLE");
}
}

//Keeps motion, serial input and the home switch updated
void loop() {
  updateMotion();
   updateSerial();
   updateHomeSwitch();
   updateHoming();
}
