/*
ESPboy OTTO v1 improved by RomanS for ESPbot project
www.ESPboy.com


--------------------------------------------------------
It includes with all respect parts of following code:
--------------------------------------------------------
OTTO_DIY project 
https://www.ottodiy.com

ZOWI project
http://zowi.bq.com

Talkie library
Copyright 2011 Peter Knight
This code is released under GPLv2 license.

Zowi Battery Reader Library
@version 20150831
@author Raul de Pablos Martin

MaxMatrix
Version 1.0 Feb 2013
Copyright 2013 Oscar Kin-Chung Au

Oscillator.pde
Generate sinusoidal oscillations in the servos
(c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
GPL license

"SerialCommand.h" by Steven Cogswell http://awtfy.com
--------------------------------------------------------
*/

#include <Arduino.h>
#include "ESPboyOtto.h"

void OTTO::init(uint8_t YL, uint8_t YR, uint8_t RL, uint8_t RR, bool load_calibration, uint8_t NoiseSensor, uint8_t Buzzer, uint8_t USTrigger, uint8_t USEcho, uint8_t pinButt) {
  servo_pins[0] = YL;
  servo_pins[1] = YR;
  servo_pins[2] = RL;
  servo_pins[3] = RR;

  attachServos();
  isOttoResting=false;

  if (load_calibration) {
    for (int i = 0; i < 4; i++) {
      int servo_trim = EEPROM.read(i);
      if (servo_trim > 128) servo_trim -= 256;
      servo[i].SetTrim(servo_trim);
    }
  }
  
  for (int i = 0; i < 4; i++) servo_position[i] = 90;
  
  pinBuzzer = Buzzer; //Buzzer init
  pinMode(pinBuzzer,OUTPUT);
  pinNoiseSensor = NoiseSensor; //Noise sensor init
  pinMode(pinNoiseSensor,INPUT);

  pinBattery = PIN_battery;
  pinMode(pinBattery,INPUT);
  
  pinUltrasonicEcho = USEcho;
  pinMode(pinUltrasonicEcho,INPUT);
  pinUltrasonicTrigger = USTrigger;
  pinMode( pinUltrasonicTrigger , OUTPUT );

  pinButton = pinButt;
  pinMode(pinButton,INPUT);
  
  initMatrix(); //LED Matrix init
}



//-- ATTACH & DETACH FUNCTIONS ----------------------------------//
void OTTO::attachServos(){
    servo[0].attach(servo_pins[0]);
    servo[1].attach(servo_pins[1]);
    servo[2].attach(servo_pins[2]);
    servo[3].attach(servo_pins[3]);
}

void OTTO::detachServos(){
    servo[0].detach();
    servo[1].detach();
    servo[2].detach();
    servo[3].detach();
}


//-- OSCILLATORS TRIMS ------------------------------------------//
void OTTO::setTrims(int YL, int YR, int RL, int RR) {
  servo[0].SetTrim(YL);
  servo[1].SetTrim(YR);
  servo[2].SetTrim(RL);
  servo[3].SetTrim(RR);
}


void OTTO::saveTrimsOnEEPROM() {
  for (int i = 0; i < 4; i++){ 
      EEPROM.write(i, servo[i].getTrim());
  } 
      
}


//-- BASIC MOTION FUNCTIONS -------------------------------------//
void OTTO::_moveServos(int time, int  servo_target[]) {

  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }

  if(time>10){
    for (int i = 0; i < 4; i++) increment[i] = ((servo_target[i]) - servo_position[i]) / (time / 10.0);
    final_time =  millis() + time;

    for (int iteration = 1; millis() < final_time; iteration++) {
      partial_time = millis() + 10;
      for (int i = 0; i < 4; i++) servo[i].SetPosition(servo_position[i] + (iteration * increment[i]));
      while (millis() < partial_time); //pause
    }
  }
  else{
    for (int i = 0; i < 4; i++) servo[i].SetPosition(servo_target[i]);
  }
  for (int i = 0; i < 4; i++) servo_position[i] = servo_target[i];
}

void OTTO::_moveSingle(int position, int servo_number) {
if (position > 180) position = 90;
if (position < 0) position = 90;
  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }
int servoNumber = servo_number;
if (servoNumber == 0){
  servo[0].SetPosition(position);
}
if (servoNumber == 1){
  servo[1].SetPosition(position);
}
if (servoNumber == 2){
  servo[2].SetPosition(position);
}
if (servoNumber == 3){
  servo[3].SetPosition(position);
}
}

void OTTO::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle=1){

  for (int i=0; i<4; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()){
     for (int i=0; i<4; i++){
        servo[i].refresh();
     }
  }
}


void OTTO::_execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0){
  attachServos();
  if(getRestState()==true){
        setRestState(false);
  }
  int cycles=(int)steps;    
  //-- Execute complete cycles
  if (cycles >= 1) 
    for(int i = 0; i < cycles; i++) 
      oscillateServos(A,O, T, phase_diff);
  //-- Execute the final not complete cycle    
  oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}



//-- HOME = Otto at rest position -------------------------------//
void OTTO::home(){

  if(isOttoResting==false){ //Go to rest position only if necessary

    int homes[4]={90, 90, 90, 90}; //All the servos at rest position
    _moveServos(500,homes);   //Move the servos in half a second

    detachServos();
    isOttoResting=true;
  }
}


bool OTTO::getRestState(){
    return isOttoResting;
}


void OTTO::setRestState(bool state){
    isOttoResting = state;
}


//-- PREDETERMINED MOTION SEQUENCES -----------------------------//
void OTTO::move(uint8_t moveId, uint16_t moveSpeed, uint16_t moveSize) {
  switch (moveId) {
    case 0:
      home();
      break;
    case 1:
      walk(1, moveSpeed, 1);
      break;
    case 2: 
      walk(1, moveSpeed, -1);
      break;
    case 3: 
      turn(1, moveSpeed, 1);
      break;
    case 4:
      turn(1, moveSpeed, -1);
      break;
    case 5: 
      updown(1, moveSpeed, moveSize);
      break;
    case 6:
      moonwalker(1, moveSpeed, moveSize, 1);
      break;
    case 7:
      moonwalker(1, moveSpeed, moveSize, -1);
      break;
    case 8: 
      swing(1, moveSpeed, moveSize);
      break;
    case 9: 
      crusaito(1, moveSpeed, moveSize, 1);
      break;
    case 10:
      crusaito(1, moveSpeed, moveSize, -1);
      break;
    case 11:
      jump(1, moveSpeed);
      break;
    case 12:
      flapping(1, moveSpeed, moveSize, 1);
      break;
    case 13: 
      flapping(1, moveSpeed, moveSize, -1);
      break;
    case 14: 
      tiptoeSwing(1, moveSpeed, moveSize);
      break;
    case 15: 
      bend(1, moveSpeed, 1);
      break;
    case 16:
      bend(1, moveSpeed, -1);
      break;
    case 17:
      shakeLeg(1, moveSpeed, 1);
      break;
    case 18:
      shakeLeg(1, moveSpeed, -1);
      break;
    case 19:
      jitter(1, moveSpeed, moveSize);
      break;
    case 20:
      ascendingTurn(1, moveSpeed, moveSize);
      break;
    case 21:
      sit(1, moveSpeed);
  }
}


//---------------------------------------------------------
//-- Otto movement: Jump
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//---------------------------------------------------------
void OTTO::jump(float steps, int T){

  int up[]={90,90,150,30};
  _moveServos(T,up);
  int down[]={90,90,90,90};
  _moveServos(T,down);
}


//---------------------------------------------------------
//-- Otto movement: Sit
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//---------------------------------------------------------
void OTTO::sit(float steps, int T){

  int down[]={90,90,0,180};
  _moveServos(T,down);
  delay(1000);
  int up[]={90,90,90,90};
  _moveServos(T,up);
}


//---------------------------------------------------------
//-- Otto gait: Walking  (forward or backward)    
//--  Parameters:
//--    * steps:  Number of steps
//--    * T : Period
//--    * Dir: Direction: FORWARD / BACKWARD
//---------------------------------------------------------
void OTTO::walk(float steps, int T, int dir){

  //-- Oscillator parameters for walking
  //-- Hip sevos are in phase
  //-- Feet servos are in phase
  //-- Hip and feet are 90 degrees out of phase
  //--      -90 : Walk forward
  //--       90 : Walk backward
  //-- Feet servos also have the same offset (for tiptoe a little bit)
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);  
}


//---------------------------------------------------------
//-- Otto gait: Turning (left or right)
//--  Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void OTTO::turn(float steps, int T, int dir){

  //-- Same coordination than for walking (see Otto::walk)
  //-- The Amplitudes of the hip's oscillators are not igual
  //-- When the right hip servo amplitude is higher, the steps taken by
  //--   the right leg are bigger than the left. So, the robot describes an 
  //--   left arc
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)}; 
    
  if (dir == LEFT) {  
    A[0] = 30; //-- Left hip servo
    A[1] = 10; //-- Right hip servo
  }
  else {
    A[0] = 10;
    A[1] = 30;
  }
    
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto gait: Lateral bend
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    dir: RIGHT=Right bend LEFT=Left bend
//---------------------------------------------------------
void OTTO::bend (int steps, int T, int dir){

  //Parameters of all the movements. Default: Left bend
  int bend1[4]={90, 90, 62, 35}; 
  int bend2[4]={90, 90, 62, 105};
  int homes[4]={90, 90, 90, 90};

  //Time of one bend, constrained in order to avoid movements too fast.
  //T=max(T, 600);

  //Changes in the parameters if right direction is chosen 
  if(dir==-1)
  {
    bend1[2]=180-35;
    bend1[3]=180-60;  //Not 65. Otto is unbalanced
    bend2[2]=180-105;
    bend2[3]=180-60;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=800; 

  //Bend movement
  for (int i=0;i<steps;i++)
  {
    _moveServos(T2/2,bend1);
    _moveServos(T2/2,bend2);
    delay(T*0.8);
    _moveServos(500,homes);
  }

}


//---------------------------------------------------------
//-- Otto gait: Shake a leg
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//---------------------------------------------------------
void OTTO::shakeLeg (int steps,int T,int dir){

  //This variable change the amount of shakes
  int numberLegMoves=2;

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[4]={90, 90, 58, 0};   
  int shake_leg2[4]={90, 90, 58, 120};
  int shake_leg3[4]={90, 90, 58, 60};
  int homes[4]={90, 90, 90, 90};

  //Changes in the parameters if left leg is chosen
  if(dir==-1)      
  {
    shake_leg1[2]=180-0;
    shake_leg1[3]=180-50;
    shake_leg2[2]=180-120;
    shake_leg2[3]=180-50;
    shake_leg3[2]=180-60;
    shake_leg3[3]=180-50;
  }
  
  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=1000;    
  //Time of one shake, constrained in order to avoid movements too fast.            
  T=T-T2;
  T=max(T,200*numberLegMoves);  

  for (int j=0; j<steps;j++)
  {
  //Bend movement
  _moveServos(T2/2,shake_leg1);
  _moveServos(T2/2,shake_leg2);
  
    //Shake movement
    for (int i=0;i<numberLegMoves;i++)
    {
    _moveServos(T/(2*numberLegMoves),shake_leg3);
    _moveServos(T/(2*numberLegMoves),shake_leg2);
    }
    _moveServos(500,homes); //Return to home position
  }
  
  delay(T);
}


//---------------------------------------------------------
//-- Otto movement: up & down
//--  Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL / MEDIUM / BIG 
//--              (or a number in degrees 0 - 90)
//---------------------------------------------------------
void OTTO::updown(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto movement: swinging side to side
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void OTTO::swing(float steps, int T, int h){
  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2, -h/2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto movement: swinging side to side without touching the floor with the heel
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void OTTO::tiptoeSwing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto gait: Jitter 
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter 
//--    h: height (Values between 5 - 25)   
//---------------------------------------------------------
void OTTO::jitter(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(25,h);
  int A[4]= {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto gait: Ascending & turn (Jitter while up&down)
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15) 
//---------------------------------------------------------
void OTTO::ascendingTurn(float steps, int T, int h){

  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(13,h);
  int A[4]= {h, h, h, h};
  int O[4] = {0, 0, h+4, -h+4};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto gait: Moonwalker. Otto moves like Michael Jackson
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical valures between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void OTTO::moonwalker(float steps, int T, int h, int dir){

  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Otto's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Otto, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly
 
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2+2, -h/2 -2};
  int phi = -dir * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//----------------------------------------------------------
//-- Otto gait: Crusaito. A mixture between moonwalker and walk
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//-----------------------------------------------------------
void OTTO::crusaito(float steps, int T, int h, int dir){

  int A[4]= {25, 25, h, h};
  int O[4] = {0, 0, h/2+ 4, -h/2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//---------------------------------------------------------
//-- Otto gait: Flapping
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FOREWARD, BACKWARD
//---------------------------------------------------------
void OTTO::flapping(float steps, int T, int h, int dir){

  int A[4]= {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}



//-- SENSORS FUNCTIONS  -----------------------------------------//

unsigned int OTTO::getDistance(){
  long dist=0;
  for (uint8_t i=0; i<3; i++)
    dist+=measureDistance();
  return dist/3;
}


unsigned int OTTO::measureDistance(){
  unsigned int duration;
  digitalWrite(pinUltrasonicTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinUltrasonicTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinUltrasonicTrigger, LOW);
  duration = pulseIn(pinUltrasonicEcho, HIGH);
  return duration / 58; 
}


int OTTO::getNoise(){
  int noiseLevel = 0;
  int noiseReadings = 0;
  int numReadings = 2;  
    noiseLevel = analogRead(pinNoiseSensor);
    for(int i=0; i<numReadings; i++){
        noiseReadings += analogRead(pinNoiseSensor);
        delay(4); // delay in between reads for stability
    }
    noiseLevel = noiseReadings / numReadings;
    return noiseLevel;
}



//-- Button
uint8_t OTTO::getButton(){
   if (analogRead (PIN_Button)>buttonSense) return (1);
   else return(0);
}



//-- Otto getBatteryLevel: return battery voltage percent
uint8_t OTTO::getBatteryLevel(){
    double batteryReadings = 0;
    int batteryLevel;
    for(int i=0; i<5; i++){
        batteryReadings += map (analogRead(pinBattery), 800, 1024, 0, 100); 
        delay(1);
    }
    batteryLevel = batteryReadings / 5;
    return batteryLevel;
}



//-- ANIMATIONS -------------------------------------------------//
void OTTO::putAnimationMouth(uint32_t aniMouth, uint8_t index){
  putMouth5x6(pgm_read_dword(&Gesturetable[aniMouth][index]));
}


//-- LED MATRIX AND MOUTH ---------------------------------------//
void OTTO::initMatrix(){ 
  pinMode(PIN_Matrix_Dta,  OUTPUT);
  pinMode(PIN_Matrix_CLK, OUTPUT);
  pinMode(PIN_Matrix_CS,  OUTPUT);
  digitalWrite(PIN_Matrix_CLK, HIGH); 
  setCommandMatrix(max7219_reg_scanLimit, 0x07);      
  setCommandMatrix(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  setCommandMatrix(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  setCommandMatrix(max7219_reg_displayTest, 0x00); // no display test
  clearMouth();  // empty registers, turn all LEDs off
  intensityMatrix(0x0f); //max brightness
}


void OTTO::intensityMatrix(int intensity){
  setCommandMatrix(max7219_reg_intensity, intensity);
}


void OTTO::setCommandMatrix(uint8_t command, uint8_t value){
  digitalWrite(PIN_Matrix_CS, LOW);    
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, command);
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, value);
  digitalWrite(PIN_Matrix_CS, LOW);
  digitalWrite(PIN_Matrix_CS, HIGH);
}


void OTTO::setDotMatrix(int8_t col, int8_t row){
  if(col<8 && row<8 && col>=0 && row>0){
    bitWrite(bufColMatrix[col], row, 1);
    digitalWrite(PIN_Matrix_CS, LOW);  
    shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, col+1);
    shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, bufColMatrix[col]);
    digitalWrite(PIN_Matrix_CS, LOW);
    digitalWrite(PIN_Matrix_CS, HIGH);
  }
}


void OTTO::setColumnMatrix(uint8_t col, uint8_t value){
  digitalWrite(PIN_Matrix_CS, LOW);    
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, col+1);
  shiftOut(PIN_Matrix_Dta, PIN_Matrix_CLK, MSBFIRST, value);
  digitalWrite(PIN_Matrix_CS, LOW);
  digitalWrite(PIN_Matrix_CS, HIGH);
  bufColMatrix[col] = value;
}


void OTTO::putMouth(uint8_t mouth){
  uint32_t value = pgm_read_dword(&Mouthtable[mouth]);
  uint8_t bufMouth[5];
 
  for(uint8_t i=0; i<5; i++){
    bufMouth[i] = value & 63;
    value >>= 6;
  }
  clearMouth();
 for(uint8_t i=0; i<5; i++)
   for(uint8_t j=0; j<6; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(6-j,5-i+2);
}


void OTTO::putMouth5x6(uint32_t mouth){
  uint32_t value = mouth;
  uint8_t bufMouth[5];
  for(uint8_t i=0; i<5; i++){
    bufMouth[i] = value & 63;
    value >>= 6;
  }
 clearMouth();
 for(uint8_t i=0; i<5; i++)
   for(uint8_t j=0; j<6; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(6-j,5-i+1);
}


void OTTO::putMouth8x8(uint64_t mouth){
  uint64_t value = mouth;
  uint8_t bufMouth[8];
  for(uint8_t i=0; i<8; i++){
    bufMouth[i] = value & 255;
    value >>= 8;
  }
 clearMouth();
 for(uint8_t i=0; i<8; i++)
   for(uint8_t j=0; j<8; j++)
     if((bufMouth[i])&(1<<j)) setDotMatrix(8-j,8-i);
}


void OTTO::clearMouth(){
  for (int i=0; i<8; i++){ 
    setColumnMatrix(i,0);
    bufColMatrix[i]=0;
  }
}


void OTTO::writeTextMatrix(char *str, uint16_t scrollspeed){
 uint8_t charNo = 0;
 char currentChar;
 char printChar;
  while (currentChar = str[charNo++]){
    if ((currentChar < 48) || (currentChar > 91))
       printChar = 44;
    else 
       printChar = currentChar-48;
    sendCharMatrix (printChar, scrollspeed);
  }
}



void OTTO::sendCharMatrix (uint8_t printChar, uint16_t scrollspeed){
 uint8_t bufSymbol[8];
  memcpy_P (&bufSymbol, &Character_font_6x8[printChar*6], 6);

  clearMouth();
  for(int8_t a=6; a>-7; a--){
    for(uint8_t i=0; i<6; i++)
      for (uint8_t j=0; j<8; j++)
        if ((bufSymbol[i]>>j) & 1) setDotMatrix(i+1+a, 8-j);
  delay(scrollspeed);
  clearMouth();   
  }
}


//-- SOUNDS -----------------------------------------------------//
void OTTO::_tone (float noteFrequency, long noteDuration, int silentDuration){
      if(silentDuration==0){silentDuration=1;}
      TimerFreeTone(OTTO::pinBuzzer, noteFrequency, noteDuration);      
      delay(silentDuration);     //
}



void OTTO::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration){
  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);
  if(silentDuration==0){silentDuration=1;}
  if(initFrequency < finalFrequency){
      for (int i=initFrequency; i<finalFrequency; i=i*prop) {
          _tone(i, noteDuration, silentDuration);
      }
  } else{
      for (int i=initFrequency; i>finalFrequency; i=i/prop) {
          _tone(i, noteDuration, silentDuration);
      }
  }
}


void OTTO::sing(int songName){
  switch(songName){

    case S_connection:
      _tone(note_E5,50,30);
      _tone(note_E6,55,25);
      _tone(note_A6,60,10);
    break;

    case S_disconnection:
      _tone(note_E5,50,30);
      _tone(note_A6,55,25);
      _tone(note_E6,50,10);
    break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
    break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
    break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
    break;

    case S_mode3:
      _tone(note_E6,50,100); //D6
      _tone(note_G6,50,80);  //E6
      _tone(note_D7,300,0);  //G6
    break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
    break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
           _tone(note_B5,5,10);
      }
    break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          _tone(note_C6,10,10);
      }
    break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
    break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
    break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
    break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
    break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
    break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
    break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2); 
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
    break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
    break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
    break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
    break;

  }
}



//-- GESTURES ---------------------------------------------------//
void OTTO::playGesture(int gesture){
 int gesturePOSITION[4];
  
  switch(gesture){

    case G_OttoHappy: 
        _tone(note_E5,50,30);
        putMouth(M_smile);
        sing(S_happy_short);
        swing(1,800,20); 
        sing(S_happy_short);

        home();
        putMouth(M_happyOpen);
    break;


    case G_OttoSuperHappy:
        putMouth(M_happyOpen);
        sing(S_happy);
        putMouth(M_happyClosed);
        tiptoeSwing(1,500,20);
        putMouth(M_happyOpen);
        sing(S_superHappy);
        putMouth(M_happyClosed);
        tiptoeSwing(1,500,20); 

        home();  
        putMouth(M_happyOpen);
    break;


    case G_OttoSad: 
        putMouth(M_sad);
        gesturePOSITION[0] = 110;//int sadPos[6]=      {110, 70, 20, 160};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 20;
          gesturePOSITION[3] = 160;
        _moveServos(700, gesturePOSITION);     
        bendTones(880, 830, 1.02, 20, 200);
        putMouth(M_sadClosed);
        bendTones(830, 790, 1.02, 20, 200);  
        putMouth(M_sadOpen);
        bendTones(790, 740, 1.02, 20, 200);
        putMouth(M_sadClosed);
        bendTones(740, 700, 1.02, 20, 200);
        putMouth(M_sadOpen);
        bendTones(700, 669, 1.02, 20, 200);
        putMouth(M_sad);
        delay(500);

        home();
        delay(300);
        putMouth(M_happyOpen);
    break;


    case G_OttoSleeping:
    gesturePOSITION[0] = 100;//int bedPos[6]=      {100, 80, 60, 120};
        gesturePOSITION[1] = 80;
         gesturePOSITION[2] = 60;
          gesturePOSITION[3] = 120;
        _moveServos(700, gesturePOSITION);     
        for(int i=0; i<4;i++){
          putAnimationMouth(A_dreamMouth,0);
          bendTones (100, 200, 1.04, 10, 10);
          putAnimationMouth(A_dreamMouth,1);
          bendTones (200, 300, 1.04, 10, 10);  
          putAnimationMouth(A_dreamMouth,2);
          bendTones (300, 500, 1.04, 10, 10);   
          delay(500);
          putAnimationMouth(A_dreamMouth,1);
          bendTones (400, 250, 1.04, 10, 1); 
          putAnimationMouth(A_dreamMouth,0);
          bendTones (250, 100, 1.04, 10, 1); 
          delay(500);
        } 

        putMouth(M_lineMouth);
        sing(S_cuddly);

        home();  
        putMouth(M_happyOpen);
    break;


    case G_OttoFart:
    gesturePOSITION[0] = 90;// int fartPos_1[6]=   {90, 90, 145, 122};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 145;
          gesturePOSITION[3] = 122;
        _moveServos(500,gesturePOSITION);
        delay(300);     
        putMouth(M_lineMouth);
        sing(S_fart1);  
        putMouth(M_tongueOut);
        delay(250);
        gesturePOSITION[0] = 90;// int fartPos_2[6]=   {90, 90, 80, 122};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 80;
          gesturePOSITION[3] = 122;
        _moveServos(500,gesturePOSITION);
        delay(300);
        putMouth(M_lineMouth);
        sing(S_fart2); 
        putMouth(M_tongueOut);
        delay(250);
        gesturePOSITION[0] = 90;// int fartPos_3[6]=   {90, 90, 145, 80};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 145;
          gesturePOSITION[3] = 80;
        _moveServos(500,gesturePOSITION);
        delay(300);
        putMouth(M_lineMouth);
        sing(S_fart3);
        putMouth(M_tongueOut);    
        delay(300);

        home(); 
        delay(500); 
        putMouth(M_happyOpen);
    break;


    case G_OttoConfused:
    gesturePOSITION[0] = 110;//int confusedPos[6]= {110, 70, 90, 90};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(300, gesturePOSITION); 
        putMouth(M_confused);
        sing(S_confused);
        delay(500);

        home();  
        putMouth(M_happyOpen);
    break;


    case G_OttoLove:
        putMouth(M_heart);
        sing(S_cuddly);
        crusaito(2,1500,15,1);

        home(); 
        sing(S_happy_short);  
        putMouth(M_happyOpen);
    break;


    case G_OttoAngry: 
    gesturePOSITION[0] = 90;//int angryPos[6]=    {90, 90, 70, 110};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 70;
          gesturePOSITION[3] = 110;
        _moveServos(300, gesturePOSITION); 
        putMouth(M_angry);

        _tone(note_A5,100,30);
        bendTones(note_A5, note_D6, 1.02, 7, 4);
        bendTones(note_D6, note_G6, 1.02, 10, 1);
        bendTones(note_G6, note_A5, 1.02, 10, 1);
        delay(15);
        bendTones(note_A5, note_E5, 1.02, 20, 4);
        delay(400);
        gesturePOSITION[0] = 110;//int headLeft[6]=    {110, 110, 90, 90};
        gesturePOSITION[1] = 110;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(200, gesturePOSITION); 
        bendTones(note_A5, note_D6, 1.02, 20, 4);
        gesturePOSITION[0] = 70;//int headRight[6]=   {70, 70, 90, 90};
        gesturePOSITION[1] = 70;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 90;
        _moveServos(200, gesturePOSITION); 
        bendTones(note_A5, note_E5, 1.02, 20, 4);
        home();  
        putMouth(M_happyOpen);
    break;


    case G_OttoFretful: 
        putMouth(M_angry);
        bendTones(note_A5, note_D6, 1.02, 20, 4);
        bendTones(note_A5, note_E5, 1.02, 20, 4);
        delay(300);
        putMouth(M_lineMouth);

        for(int i=0; i<4; i++){
          gesturePOSITION[0] = 90;//int fretfulPos[6]=  {90, 90, 90, 110};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 90;
          gesturePOSITION[3] = 110;
          _moveServos(100, gesturePOSITION);   
          home();
        }
        putMouth(M_angry);
        delay(500);
        home();  
        putMouth(M_happyOpen);
    break;


    case G_OttoMagic:
        //Initial note frecuency = 400
        //Final note frecuency = 1000 
        // Reproduce the animation four times
        for(int i = 0; i<4; i++){ 
          int noteM = 400; 
            for(int index = 0; index<6; index++){
              putAnimationMouth(A_adivinawi,index);
              bendTones(noteM, noteM+100, 1.04, 10, 10);    //400 -> 1000 
              noteM+=100;
            }
            clearMouth();
            bendTones(noteM-100, noteM+100, 1.04, 10, 10);  //900 -> 1100
            for(int index = 0; index<6; index++){
              putAnimationMouth(A_adivinawi,index);
              bendTones(noteM, noteM+100, 1.04, 10, 10);    //1000 -> 400 
              noteM-=100;
            }
        } 
        delay(300);
        putMouth(M_happyOpen);
    break;


    case G_OttoWave:
        // Reproduce the animation four times
        for(int i = 0; i<2; i++){ 
            int noteW = 500; 
            for(int index = 0; index<10; index++){
              putAnimationMouth(A_wave,index);
              bendTones(noteW, noteW+100, 1.02, 10, 10); 
              noteW+=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(A_wave,index);
              bendTones(noteW, noteW+100, 1.02, 10, 10); 
              noteW+=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(A_wave,index);
              bendTones(noteW, noteW-100, 1.02, 10, 10); 
              noteW-=101;
            }
            for(int index = 0; index<10; index++){
              putAnimationMouth(A_wave,index);
              bendTones(noteW, noteW-100, 1.02, 10, 10); 
              noteW-=101;
            }
        }    
        clearMouth();
        delay(100);
        putMouth(M_happyOpen);
    break;

    case G_OttoVictory:
        putMouth(M_smallSurprise);
        //final pos   = {90,90,150,30}
        for (int i = 0; i < 60; ++i){
          int pos[]={90,90,90+i,90-i};  
          _moveServos(10,pos);
          _tone(1600+i*20,15,1);
        }
        putMouth(M_bigSurprise);
        //final pos   = {90,90,90,90}
        for (int i = 0; i < 60; ++i){
          int pos[]={90,90,150-i,30+i};  
          _moveServos(10,pos);
          _tone(2800+i*20,15,1);
        }
        putMouth(M_happyOpen);
        //SUPER HAPPY
        //-----
        tiptoeSwing(1,500,20);
        sing(S_superHappy);
        putMouth(M_happyClosed);
        tiptoeSwing(1,500,20); 
        //-----
        home();
        clearMouth();
        putMouth(M_happyOpen);
    break;

    case G_OttoFail:
        putMouth(M_sadOpen);
         gesturePOSITION[0] = 90;//int bendPos_1[6]=   {90, 90, 70, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 70;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(900,200,1);
        putMouth(M_sadClosed);
        gesturePOSITION[0] = 90;//int bendPos_2[6]=   {90, 90, 55, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 55;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(600,200,1);
        putMouth(M_confused);
        gesturePOSITION[0] = 90;//int bendPos_3[6]=   {90, 90, 42, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 42;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        _tone(300,200,1);
        gesturePOSITION[0] = 90;//int bendPos_4[6]=   {90, 90, 34, 35};
        gesturePOSITION[1] = 90;
         gesturePOSITION[2] = 34;
          gesturePOSITION[3] = 35;
        _moveServos(300,gesturePOSITION);
        putMouth(M_xMouth);

        detachServos();
        _tone(150,2200,1);
        
        delay(600);
        clearMouth();
        putMouth(M_happyOpen);
        home();

    break;

  }
} 
