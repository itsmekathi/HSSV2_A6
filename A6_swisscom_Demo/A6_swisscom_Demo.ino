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

void setup() {
  A6board.begin(A6baud);   // the GPRS baud rate
  Serial.begin(115200);    // the GPRS baud rate
  // ctrlZ String definition
  end_c[0] = 0x1a;
  end_c[1] = '\0';
  Serial.println("Start");

  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  delay(5000);
  digitalWrite(RST, LOW);
  delay(500);
  if (A6begin() != OK) {
    Serial.println("Error");
    while (1 == 1);
  }
}

void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  //if input 'd' in the terminal, it will execute DialVoiceCall(), etc.
  // GetSignalQuality();
  // A6command("AT+CIPSTATUS", "OK", "yy", 10000, 2);
  Serial.println("Waiting for command");
  if (Serial.available())
    switch (Serial.read())
    {
      case 's':
        Serial.println("Send SMS to Kathi");
        sendSMS((String)KATHI_PHNO,(String) SMS_TEXT);
        break;
      case 'c':
        Serial.println("Make voice call to Kathi");
        dial((String)KATHI_PHNO);
        break;
      case 't':
        Serial.println("Send SMS to Amma");
        sendSMS((String)AMMA_PHNO,(String)SMS_TEXT);
        break;
      case 'u':
        Serial.println("Make voice call to Amma");
        dial((String)AMMA_PHNO);
        break;
      case 'h':
        Serial.println("Call hangUp");
        hangUp();
        break;
      default:
        Serial.println("You have entered an invalid command");
    }
  while ( A6board.available())
    Serial.write( A6board.read());

  delay(2000);
}

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



///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  A6command("AT+CMGF=1\r", "OK", "yy", 2000, 2); //Because we want to send the SMS in text mode
  delay(100);
  A6command("AT+CMGS = \"+919738460098\"", ">", "yy", 20000, 2); //send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  A6board.println("GSM test message!");//the content of the message
  A6board.println(end_c);//the ASCII code of the ctrl+z is 26
  A6board.println();
}

///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  A6command("AT+SNFS=0", "OK", "yy", 20000, 2);
  A6command("ATD+919738460098;", "OK", "yy", 20000, 2); //dial the number
}


// Hang up the phone.
void hangUp() {
    A6command("ATH", "OK", "yy", 20000, 2);
}


void  sendSMS(String number, String text) 
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
