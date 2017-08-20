#include <SoftwareSerial.h>

#define OK 1
#define NOTOK 2
#define TIMEOUT 3
#define RST 2


#define log(msg) Serial.print(msg)
#define logln(msg) Serial.println(msg)


SoftwareSerial A6board (5,6);
#define A6baud 115200
#define SERIALTIMEOUT 3000

char KATHI_PHNO[] =   "+919738460098";
char AMMA_PHNO[]  =   "+918197429387";
char SMS_TEXT[]   =   "GSM test message!";

char end_c[2];

// APR33A3  busy,rec,m0,m1,m2,m3
APR33A3 audio1(A0,A2,A1,A3,A4,A5);




//************************************************************************************************************************
//////////////////////////////   DEFINITIONS     /////////////////////////////////////////////////////////////////////////

#define STATUS_LED 13
#define OUT0 9
#define OUT1 10
#define AUDIO_PLAY_TIMES 3          // Defines the number of time the audio will be played through the phone
#define AFTER_CALL_DELAY 10000      // 10 seconds delay after you send the call command
#define AUDIO_CHANNEL 0             // Defines the channel on which the audio is stored
#define CALL_SETUP_DELAY 10         // Delay required after sending ATD command to GSM
#define CALL_WAIT_DELAY 5           // Delay after the ring so that user can pick up the phone    
#define RETRY_CALL_COUNT 3          // Retry call after particular count
#define RETRY_CALL_DELAY 5          // Rerty call after each failure
#define HANGUP_DELAY 5              // Dealy after call hangup


#define NUMBER_STORE_START_LOCATION 10
#define NUMBER_LENGTH 13


