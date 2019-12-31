/*
"ESPboy OTTO v1" is improved version of OTTO by RomanS for ESPbot project
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



Please calibrate your OTTO with "OTTO calibration.ino" sketch
Don't forget to write calibration parameters to the EEPROM

       -------- 
      |  O  O  |
      |--------|
      |        | 
       -------- 
 YL   ||     ||   YR
RL  |---     ---|  RR


Your ESPboy OTTO has:
0. arduino pro micro chip
1. Legs for walking and dansing
2. Eyes for detecting the obstacles
3. Touch sensor for selecting modes
4. LED matrix for 5x6 smiles (supporting animation) and 8x8 scroll text and ptinting symbols
5. Microphone for detecting surrounding sounds
6. buzzer for emotions sounds
7. speaker for talking
8. tempersture/pressure/humidity sensor
9. real time clock chip with alarm
10. OLED 128x64 for displayimg telemetry data, date/time/alarm/meteo info
11. valcoder for setting up time/date/alarm
12. bluetooth module for remote controlling otto by Android App on your Phone and 433mhz module for remote controlling otto by ESPboy
13. battery with charging controller


A) 4 modes changed by touching head sensor:
  0: Waiting
  1: Dancing
  2: Walking
  3: Controlling by ESPboy

B) 20 preset moves + 1 stand
home, jump, walk[for/back], turn[lft/rgt], bend[lft/rgt], shakeLeg[lft/rgt], 
updown, swing, tiptoeSwing, jitter, ascendingTurn, moonwalker[lft/rgt],
crusaito[lft/rgt], flapping[flapping], 
NEW sit

C) 13 preset guestures
OttoHappy, OttoSuperHappy, OttoSad, OttoSleeping, OttoFart, OttoConfused,
OttoLove, OttoAngry, OttoFretful, OttoMagic, OttoWave, OttoVictory, OttoFail

D) 4 mouth animations
littleUuh, dreamMouth, adivinawi, wave, otto

E) 30 preset mouth emotions
zero, one, two, three, four, five, six, seven, eight , nine
smile, happyOpen, happyClosed, heart, bigSurprise, smallSurprise,
tongueOut, vamp1, vamp2, lineMouth, confused, diagonal, sad, 
sadOpen, sadClosed, okMouth , xMouth, interrogation, thunder, 
culito, angry

F) 19 preset sounds
S_connection, S_disconnection, S_buttonPushed, S_mode1, S_mode2, S_mode3, 
S_surprise, S_OhOoh, S_OhOoh2, S_cuddly, S_sleeping, S_happy, S_superHappy, 
S_happy_short, S_sad, S_confused, S_fart1, S_fart2, S_fart3


G) Lot of voices from Talkie lib
spt_CONNECTED, spt_COURSE, spt_DO, spt_DONE, spt_EXACTLY, spt_FINE, spt_FINISH, spt_GO,
spt_GOOD, spt_GOOD_WORK, spt_GOODBYE, spt_HAVE, spt_HELLO, spt_HELP, spt_HOME, spt_LOOKS, spt_I,
spt_AM, spt_LIKE, spt_IT, spt_NEGATIVE, spt_NICE_TRY, spt_NO, spt_ON, spt_OFF, spt_PLEASE,
spt_POSITION, spt_POSITIVE, spt_PROBLEM, spt_RIGHT, spt_LEFT, spt_BACK, spt_YES, spt_YOU,
spt_SORRY, spt_START, spt_STOP, spt_THAT_IS_INCORRECT, spt_THAT_IS_RIGHT, spt_THAT_IS_INCORRECT,
spt_THAT_IS_TURN, spt_WHAT_WAS_THAT, spt_WHAT_UNDERSTAND
*/


#include "ESPboyOtto.h" 
#include <Talkie.h>
#include <SoftwareWire.h> 
#include <RtcDS3231.h>
#include <SerialCommand.h>


enum ottoStates {ottoStateWaiting = 0,ottoStateDancing, ottoStateWalking, ottoStateControlling} ottoState;

OTTO Otto; 
Talkie voice;
SoftwareWire myWire(PIN_SDA, PIN_SCL);
RtcDS3231<SoftwareWire> Rtc(myWire);
RtcDateTime RtcNow;
RtcTemperature temp;

static uint8_t OTTO_STATE = ottoStateWaiting;
static double waitingtime;
static char timestring[40];

void setup() {
  Serial.begin(115200);

//RTC init and set if first run
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid() && !Rtc.LastError()) Rtc.SetDateTime(compiled);
  if (!Rtc.GetIsRunning()) Rtc.SetIsRunning(true);
  RtcNow = Rtc.GetDateTime();
  if (RtcNow < compiled)  Rtc.SetDateTime(compiled);
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo, PIN_Button); 
  Otto.home();
  Otto.putMouth(M_smile);
  Otto.sing(S_connection);
  voice.doNotUseUseInvertedOutput();
  delay(500);
  voice.say(spt_CONNECTED);
  delay(500);
  voice.say(spt_HELLO);
  
  getdatetime();
  Otto.writeTextMatrix (timestring, 20);
  delay(500);

  itoa (Otto.getBatteryLevel(), timestring, 10);
  strcat(timestring, " BAT   ");
  Otto.writeTextMatrix (timestring, 20);

  temp = Rtc.GetTemperature();
  itoa((int8_t)temp.AsFloatDegC(), timestring, 10);
  strcat(timestring, "C TEMP ");
  Otto.writeTextMatrix (timestring, 20);

  Otto.putMouth(M_smile);
  Otto.sing(S_happy);
  waitingtime = millis()+5000;
}



void loop() {
 uint8_t randomDance = 0;
 uint8_t randomSteps = 0;
 static uint16_t moveSpeed, moveHight = movingAmplitude;
 int8_t side;
 
  if (Otto.getButton()){ 
    OTTO_STATE++; 
    if (OTTO_STATE > ottoStateControlling) OTTO_STATE = ottoStateWaiting;
 
    if (OTTO_STATE == ottoStateWaiting) {
      Otto.intensityMatrix(10);
      Otto.home();
      playRandomAnim();
      Otto.sing(S_mode1);
      sayPhrase(random (0,12));
      Otto.writeTextMatrix ((char *)" RESTING ", 20);
      Otto.intensityMatrix(1);
      Otto.putMouth(M_smile);
      waitingtime = millis()+5000;
      
    }
    if (OTTO_STATE == ottoStateDancing) {
      Otto.intensityMatrix(10);
      Otto.home();
      playRandomAnim();
      Otto.sing(S_mode2);
      sayPhrase(random (0,12));
      Otto.writeTextMatrix ((char *)" DANCING ", 20);
    }
    if (OTTO_STATE == ottoStateWalking) {
      Otto.intensityMatrix(10);
      Otto.home();
      playRandomAnim();
      Otto.sing(S_mode3);
      sayPhrase(random (0,12));
      Otto.writeTextMatrix ((char *)" WALKING ", 20);
    }
    if (OTTO_STATE > ottoStateWalking) {
      Otto.intensityMatrix(10);
      Otto.home();
      playRandomAnim();
      Otto.sing(S_happy_short);
      sayPhrase(random (0,12));
      Otto.writeTextMatrix ((char *)" OTTO ", 20);
      Otto.putMouth(M_heart);
      delay(1000);
      Otto.intensityMatrix(1);
      delay(1000);
      Otto.intensityMatrix(10);
      delay(1000);
      Otto.intensityMatrix(1);
      delay(1000);
    }
  }

  
  switch (OTTO_STATE) {
    
    case ottoStateWaiting:
      if (waitingtime - millis() < 0){
        randomDance = random(5,22);
        if ((randomDance > 14) && (randomDance < 19))
          moveSpeed = 3200;
        else 
          moveSpeed = 2000;
        Otto.putMouth(random(10,31));
        Otto.move(random(5,22), moveSpeed, 15);
        Otto.home();
        Otto.intensityMatrix(1);
        Otto.putMouth(M_smallSurprise);
        waitingtime = millis()+5000+random(15000);
      }

      if (Otto.getDistance() < 15) {
        Otto.intensityMatrix(10);
        Otto.putMouth(M_angry);
        Otto.sing(S_confused);
        voice.say(spt_WHAT_WAS_THAT);
        Otto.move(random(5,22), 1400, 25);
        Otto.home();
        Otto.intensityMatrix(1);
        Otto.putMouth(M_smallSurprise);
        waitingtime += 5000;
      }
      break;
      
    case ottoStateDancing:
      randomDance = random(5, 22);
      if ((randomDance > 14) && (randomDance < 19)) {
        randomSteps = 1; 
        moveSpeed = 1600;
      }
      else {
        randomSteps = random(2, 4);
        moveSpeed = 1000;
      }
      Otto.putMouth(random(10,30));
      for (uint8_t i = 0; i < randomSteps; i++) 
        Otto.move(randomDance, moveSpeed, moveHight);
      break;

    case ottoStateWalking:
      Otto.putMouth(M_smile);
      if (Otto.getDistance() < 15) {
        Otto.putMouth(M_bigSurprise);
        Otto.sing(S_surprise);
        Otto.jump(5, 500);
        Otto.sing(S_cuddly);
        Otto.putMouth(M_lineMouth);
        for (uint8_t i = 0; i < 3; i++) 
          Otto.walk(1, 1300, -1); 
        side = 1;
        if (random(100) > 50){
          side = -1;   
        }
        Otto.putMouth(M_happyClosed);
        Otto.sing(S_happy_short);
        Otto.turn(5, 1000, side);
        Otto.putMouth(M_smile);
        Otto.sing(S_happy);
      }
      else
        Otto.walk(1, 1000, 1); //walk straight
      break;

    case ottoStateControlling:
      if (Otto.getDistance() < 15) {
        gettime(1);
        Otto.writeTextMatrix (timestring, 30);  
        delay(1000);            
        Otto.putMouth(M_smallSurprise);
      }
      break;
   delay (100);
  }  
}

void getdatetime(){
  timestring[0] = 0;
  getdate();
  strcat(timestring, "  ");
  gettime(0);
}


void getdate(){
  char day_[3], month_[3], year_[5];
  RtcNow = Rtc.GetDateTime();
  itoa(RtcNow.Day(), day_, 10);
  itoa(RtcNow.Month(), month_, 10);
  itoa(RtcNow.Year(), year_, 10);
  if(RtcNow.Day()<10) strcat(timestring, "0");
  strcat(timestring, day_);
  strcat(timestring, " ");
  if(RtcNow.Month()<10) strcat(timestring, "0");
  strcat(timestring, month_);
  strcat(timestring, " ");
  strcat(timestring, year_);
}


void gettime(uint8_t mode){
 char hour_[3], minute_[3];
  if (mode) timestring[0]=0;
  RtcNow = Rtc.GetDateTime();
  itoa(RtcNow.Hour(), hour_, 10);
  itoa(RtcNow.Minute(), minute_, 10);
  if(RtcNow.Hour()<10) strcat(timestring, "0");
  strcat(timestring, hour_);
  strcat(timestring, ":");
  if(RtcNow.Minute()<10) strcat(timestring, "0");
  strcat(timestring, minute_);
}


void playRandomAnim(){
  uint8_t animationNo = random(4);
  static uint8_t frames[]={8,4,6,10};
  for(int i=0; i<2; i++)
    for (int j=0; j<frames[animationNo]; j++){
      Otto.putAnimationMouth(animationNo,j);
      delay(100);
  }
}


void sayPhrase(uint8_t phraseNo){
 switch (phraseNo){
  case 0: voice.say(spt_EXACTLY);
  break;
  case 1: voice.say(spt_FINE);
  break;
  case 2: voice.say(spt_GO);
  break;
  case 3: voice.say(spt_GOOD);
  break;
  case 4: voice.say(spt_GOOD_WORK);
  break;
  case 5: voice.say(spt_NICE_TRY);
  break;
  case 6: voice.say(spt_POSITIVE);
  break;
  case 7: voice.say(spt_YES);
  break;
  case 8: voice.say(spt_THAT_IS_RIGHT);
  break;
  case 9: voice.say(spt_UNDERSTAND);
  break;
  case 10: voice.say(spt_START);
  break;
  case 11: voice.say(spt_PLEASE);
 }
}
