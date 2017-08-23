#include <APR33A3.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "definitions.h"
#include "stringTable.h"

// Size of the command buffer
#define MAX_CMD 16 
char serialDisplayBuffer[40]; //Buffer large enough for the largest string to be displayed
//Make the input command lenght to 15 so as to accept command plus phone numbers
const byte maxCmdLength = 15;
char cmdRPROGMEMBuffer[maxCmdLength+1] ={0,0,0,0,0,0,0,0,0,0,0};
char inputCommandBuffer[maxCmdLength+1] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte k = 0,i= 0;



char message[] = "Thief has entered the house, please take immediate action";

volatile int intVal = 0;        // used in interrupt service routines


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {  

  Serial.begin(115200);    // the Arduino Serial baud rate
  A6board.begin(A6baud);   // the GPRS baud rate
  
  // ctrlZ String definition
  end_c[0] = 0x1a;
  end_c[1] = '\0';
  Serial.println("Start");

  if (A6begin() != OK) {
    Serial.println("Error");
    while (1 == 1);
  }
  SerialPrintFromPROGMEM(GSM_STARTUP_SUCCESSFULL);
  

   // If there was any data sent of serial port enter programming mode
  if(Serial.available())
  {
    SerialPrintFromPROGMEM(ENTERING_PROGRAMMING_MODE);
    SetParameters();
  }

  SerialPrintFromPROGMEM(ENTERING_MAIN_CODE);

  // Make OUT0 and OUT1 as output and make them low
  pinMode(OUT0,OUTPUT);
  pinMode(OUT1, OUTPUT);
  digitalWrite(OUT0,LOW);
  digitalWrite(OUT1,LOW);

  
  // Make the interrupt pins as input and enable the pullup resistors
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  attachInterrupt(0, setVal, RISING);
  attachInterrupt(1, resetVal, RISING);
  pinMode(STATUS_LED,OUTPUT);
  digitalWrite(STATUS_LED,HIGH);  // Output HIGH to indicate system is ready

}

void loop() {
  const byte USER_COUNT = 3, ALERT_CYCLE = 1;
  byte i =0,k=0;

  if(intVal == 1)
  {
    //digitalWrite(OUT0,HIGH);
    //digitalWrite(OUT1,HIGH);
    for( i = 0; i< ALERT_CYCLE ; i++){
      SerialPrintFromPROGMEM(IN_ALERT_CYCLE);
      Serial.println(i);
      for (k = 0; k < USER_COUNT; k++)
      {
        SerialPrintFromPROGMEM(ALERTING_USER);
        Serial.println(k);
        alertUser(k);
        Serial.println("Alerted user success");
        delay(1000);        // Delay between alerting each user
      }
      Serial.print("Value of k is ");
      Serial.println(k);
      Serial.println("Exited the user loop");
    } 
    // Set Int val to 2 to avoid re-executing the same block after alerting the user and also to avoid make the OUT0 and OUT1 low
    intVal = 2;   
   }
   delay(1000);       // Polling delay for checking each 
}

// Functions used in interrupt routines
void setVal()
{
  intVal = 1;
  digitalWrite(OUT0,HIGH);
  digitalWrite(OUT1,HIGH);
}
void resetVal()
{
  intVal= 0;
  digitalWrite(OUT0,LOW);
  digitalWrite(OUT1,LOW);
}


/***************************************************************************************************/
/* Configuration Routines to congigure the number, modify the audio etc
 *  
 */
 
void SetParameters(){
  //Be in programming mode infinitely
  while(1){
    SerialPrintFromPROGMEM(ENTER_COMMAND);
    acceptAndExecuteCommand();
    delay(1000);
  }
}

 void SerialPrintFromPROGMEM(byte cmdOffset)
{
  strcpy_P(serialDisplayBuffer,(char*)pgm_read_word(&(lcd_DisplayTable[cmdOffset])));
  Serial.println(serialDisplayBuffer);  
}


void acceptAndExecuteCommand()
{
    Serial.flush();
    clearCommandBuffer();
    // Stay here untill data is availabel in serial port
    while(!Serial.available());
    delay(100); // let the serialbuffer fill up
    clearInputCommandBuffer();
    char ch;
    for(k=0;k<maxCmdLength;k++)
    {
      if(Serial.available())
      {
        ch = Serial.read();
        if(ch == '*')break;
        inputCommandBuffer[k] = ch;  
        }
        else break;   
  }
  
//Create a logic to compare data from EEPROM and place the result
    if( (inputCommandBuffer[0] == 'A') || (inputCommandBuffer[0] == 'a'))
    {
      // Modify autdio logic block
      SerialPrintFromPROGMEM(MODIFY_AUDIO);
      modifyAudio(atoi(&inputCommandBuffer[1]));
    }
    else if( (inputCommandBuffer[0] == 'B') || (inputCommandBuffer[0] == 'b'))
    {
      // Modify EEPROM first 10 bytes
      int value = atoi(&inputCommandBuffer[1]);
      Serial.println(value);
      switch ( value)
      {
        case 0:
        // Hang up the call
        SerialPrintFromPROGMEM(HANGING_UP);
        HangUp();
        break;
        default:
        SerialPrintFromPROGMEM(INVALID_COMMAND);        
      }
      
    }
    else if ( (inputCommandBuffer[0] == 'C')||(inputCommandBuffer[0] == 'c')  )
    {
      // Implement function to call the offset user
      call(atoi(&inputCommandBuffer[1]));
    }
    else if ((inputCommandBuffer[0] == 'D') || (inputCommandBuffer[0] == 'd'))
    {
      // Implement function to test the alert user routine
      int value = atoi(&inputCommandBuffer[1]);
      Serial.println(value); 
      alertUser(value);  
    }
    else if ((inputCommandBuffer[0] == 'M')||(inputCommandBuffer[0] == 'm')) 
    {
      // implement code to modify audio
      SerialPrintFromPROGMEM(MODIFY_PHONE_NUMBER);
      modifyPhoneNumber(atoi(&inputCommandBuffer[1]));
    }
    else if ((inputCommandBuffer[0] == 'S')||(inputCommandBuffer[0] == 's')){
      // Message sending routine
      sendMessage(atoi(&inputCommandBuffer[1]));
    }
    else
    {
      SerialPrintFromPROGMEM(INVALID_COMMAND);
    }
  
}


void modifyAudio(byte numChannel)
{
   SerialPrintFromPROGMEM(MODIFY_AUDIO);
  Serial.println(numChannel);
  //Write the logic to accept key and audio input from user and 
  audio1.playAudioTillBusy(numChannel);
  SerialPrintFromPROGMEM(PRESS_START);
  SerialPrintFromPROGMEM(PRESS_END);
  char ch= getControlKey();
  if(ch == '*')
  {
    audio1.recordAudio(numChannel);
    SerialPrintFromPROGMEM(PRESS_END);
    
    // Stay here till the user sends # on serial line
    Serial.flush();
    while(getControlKey() != '#');
    audio1.stopRecording();
    SerialPrintFromPROGMEM(CONFIRMED);
  }    
  else
  {
    SerialPrintFromPROGMEM(CANCELLED);
  }
}

void modifyPhoneNumber(byte numOffset)
{
  
   SerialPrintFromPROGMEM(MODIFY_PHONE_NUMBER);
  Serial.println(numOffset);
  
  loadPhoneNumberFromEEPROM(numOffset);
  Serial.println(inputCommandBuffer);
  SerialPrintFromPROGMEM(ENTER_PHONE_NUMBER);
  //Clear the Input command Buffer
  clearInputCommandBuffer();
  char key ;

// Read the input from Serial port
  Serial.flush();
  // Wait till data is present in serial port
  while(!Serial.available());
  delay(100);
  for(k=0;k<maxCmdLength;k++)
  {
    if(Serial.available()){
      key = Serial.read();
      if(key=='#')break;
      inputCommandBuffer[k] = key;
    }
    else break;
  }

  Serial.println(inputCommandBuffer);
    
  SerialPrintFromPROGMEM(CONFIRMATION);
  if(getControlKey() =='*') 
  {
    storePhoneNumberInEEPROM(numOffset);
   SerialPrintFromPROGMEM(CONFIRMED);
  }
  else 
  {
    SerialPrintFromPROGMEM(CANCELLED);
  }
  loadPhoneNumberFromEEPROM(numOffset);
  Serial.println(inputCommandBuffer);
  }
  

//Gets the confirmation keys from Keypad
char getControlKey()
{
  char key ;
  //Stay here untill data is available on serial port
  while(!Serial.available());
  delay(10);  // let the serial buffer fill up
  key = Serial.read();
  Serial.println(key);
   return key;
}

void storePhoneNumberInEEPROM(byte offset)
{
  for (k=0;k<NUMBER_LENGTH;k++)
  {
    EEPROM.write(NUMBER_STORE_START_LOCATION+(offset*NUMBER_LENGTH)+k,inputCommandBuffer[k]);
  }
}

void loadPhoneNumberFromEEPROM(byte offset)
{
  clearInputCommandBuffer();
  for(k=0;k<NUMBER_LENGTH;k++)
  {
    inputCommandBuffer[k] = (char)EEPROM.read(NUMBER_STORE_START_LOCATION+(offset*NUMBER_LENGTH)+k);
  }
}

//Clears the Input Command Buffer
void clearInputCommandBuffer()
{
  for (k = 0; k<maxCmdLength;k++)
  {
    inputCommandBuffer[k] ='\0';
  }
}

//Clears the Input Command Buffer
void clearCommandBuffer()
{
  for (k = 0; k<maxCmdLength;k++)
  {
    cmdRPROGMEMBuffer[k] ='\0';
  }
}



//Call Demo
void call(byte offset){
  loadPhoneNumberFromEEPROM(offset);
  SerialPrintFromPROGMEM(CALLING_NUMBER);
  Serial.println(inputCommandBuffer);
  int i;
  dial(inputCommandBuffer);
  delay(2000);
}

// send Message
void sendMessage(byte offset)
{
  loadPhoneNumberFromEEPROM(offset);
  SerialPrintFromPROGMEM(SENDING_MESSAGE);
  sendSMS(inputCommandBuffer,message);
  SerialPrintFromPROGMEM(SMS_SUCCESS);
}

void HangUp()
{
  hangUp();
  SerialPrintFromPROGMEM(GSM_HANGUP_SUCCESS);
}

void alertUser(byte offset)
{
  HangUp();
  delay(HANGUP_DELAY * 1000 );
  call(offset);
  delay(CALL_SETUP_DELAY * 1000);
  delay(CALL_WAIT_DELAY * 1000);
  delay(CALL_WAIT_DELAY * 1000);
  for (i = 0; i< AUDIO_PLAY_TIMES; i++){
    audio1.playAudioTillBusy(AUDIO_CHANNEL);
    delay(250);                               // Delay between each audio play cycle
  }
  HangUp();
  delay(HANGUP_DELAY * 1000 );
  sendMessage(offset);
  delay(5000);
}



/* A6 Routines for communicating with A6 using Software serial
 */
 
byte A6waitFor(String response1, String response2, int timeOut) {
  unsigned long entry = millis();
  int count = 0;
  String reply = A6read();
  byte retVal = 99;
  do {
    reply = A6read();
    if (reply != "") {
      Serial.print((millis() - entry));
      Serial.print(" ms ");
      Serial.println(reply);
    }
  } while ((reply.indexOf(response1) + reply.indexOf(response2) == -2) && millis() - entry < timeOut );
  if ((millis() - entry) >= timeOut) {
    retVal = TIMEOUT;
  } else {
    if (reply.indexOf(response1) + reply.indexOf(response2) > -2) retVal = OK;
    else retVal = NOTOK;
  }
  //  Serial.print("retVal = ");
  //  Serial.println(retVal);
  return retVal;
}

byte A6command(String command, String response1, String response2, int timeOut, int repetitions) {
  byte returnValue = NOTOK;
  byte count = 0;
  while (count < repetitions && returnValue != OK) {
    A6board.println(command);
    Serial.print("Command: ");
    Serial.println(command);
    if (A6waitFor(response1, response2, timeOut) == OK) {
      //     Serial.println("OK");
      returnValue = OK;
    } else returnValue = NOTOK;
    count++;
  }
  return returnValue;
}


void A6input() {
  String hh;
  char buffer[100];
  while (1 == 1) {
    if (Serial.available()) {
      hh = Serial.readStringUntil('\n');
      hh.toCharArray(buffer, hh.length() + 1);
      if (hh.indexOf("ende") == 0) {
        A6board.write(end_c);
        Serial.println("ende");
      } else {
        A6board.write(buffer);
        A6board.write('\n');
      }
    }
    if (A6board.available()) {
      Serial.write(A6board.read());
    }
  }
}


bool A6begin() {
  A6board.println("AT+CREG?");
  byte hi = A6waitFor("1,", "5,", 1500);  // 1: registered, home network ; 5: registered, roaming
  while ( hi != OK) {
    A6board.println("AT+CREG?");
    hi = A6waitFor("1,", "5,", 1500);
  }

  if (A6command("AT&F0", "OK", "yy", 5000, 2) == OK) {   // Reset to factory settings
    if (A6command("ATE0", "OK", "yy", 5000, 2) == OK) {  // disable Echo
      if (A6command("AT+CMEE=2", "OK", "yy", 5000, 2) == OK) return OK;  // enable better error messages
      else return NOTOK;
    }
  }
}

void ShowSerialData()
{
  unsigned long entry = millis();
  while ( A6board.available() != 0 && millis() - entry < SERIALTIMEOUT)
    Serial.println(A6board.readStringUntil('\n'));
}

String A6read() {
  String reply = "";
  if (A6board.available())  {
    reply = A6board.readString();
  }
  return reply;
}

// Hang up the phone.
void hangUp() {
    A6command("ATH", "OK", "yy", 20000, 2);
}

///SendTextMessage()
///this function is to send a sms message
void sendSMS(String number, String text) 
{
  A6command("AT+CMGF=1\r", "OK", "yy", 2000, 2); //Because we want to send the SMS in text mode
  delay(100);
  A6command("AT+CMGS = \"" + number + "\"", ">", "yy", 20000, 2); //send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  A6board.println(text);//the content of the message
  A6board.println(end_c);//the ASCII code of the ctrl+z is 26
  A6board.println();
}

// Dial a number.
void dial(String number) {
    char buffer[50];

    logln("Dialing number...");

    sprintf(buffer, "ATD%s;", number.c_str());
    A6command(buffer, "OK", "yy", 2000,2);
}

// Overloaded function for dial routine
// with char * argunment type
void dial(char *number){
  dial((String)number);
}

// Overloaded function for sending SMS
// char * argument types
void sendSMS(char *number, char *text){
  sendSMS((String)number, (String)text);
}

