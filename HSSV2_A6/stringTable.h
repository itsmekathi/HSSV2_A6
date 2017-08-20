// LCD Display Strings stored in PROGRAM Memory
#define ENTER_COMMAND 0
#define MODIFY_AUDIO 1
#define MODIFY_PHONE_NUMBER 2
#define PRESS_START 3
#define PRESS_END 4
#define LCD_INITIALIZING 5
#define READY 6
#define INVALID_COMMAND 7
#define CANCELLED 8
#define CONFIRMED 9
#define CONFIRMATION 10
#define ENTER_PHONE_NUMBER 11
#define CALLING_NUMBER 12
#define HANGING_UP 13 
#define SENDING_MESSAGE 14
#define ASK_SERIAL_DATA 15
#define ENTERING_PROGRAMMING_MODE 16
#define SYSTEM_READY 17
#define ENTERING_MAIN_CODE 18
#define LCD_STARTUP_MESSAGE 19
#define GSM_STARTUP_ERROR 20
#define GSM_STARTUP_SUCCESSFULL 21
#define IN_ALERT_CYCLE 22
#define ALERTING_USER 23
#define CALL_FAILURE 24
#define SMS_FAILURE 25
#define CALL_SUCCESS 26
#define SMS_SUCCESS 27
#define GSM_HANGUP_SUCCESS 28
#define GSM_HANGUP_FAILURE 29



//Store LCD Strings that are supposed to be displayed in PROGRAM Memory so as to save RAM Space
const char lcdEnterCmd[] PROGMEM = {"Enter Command"};
const char lcdModifyAudio[] PROGMEM = {"Modify Audio "};
const char lcdModifyPhoneNumber[] PROGMEM = {"Modify phone Number "};
const char lcdPressStarToStart[] PROGMEM = {"Press * to START"};
const char lcdPressHashToEnd[] PROGMEM = {"Press # to END"};
const char lcdInitializing[] PROGMEM = {"Initializing"};
const char lcdReady[] PROGMEM ={"Ready"};
const char lcdInvalidCommand[] PROGMEM ={"Invalid Command"};
const char lcdCancelled[] PROGMEM = {"Cancelled.."};
const char lcdConfirmed[] PROGMEM = {"Confirmed.."};
const char lcdConfirmation[] PROGMEM = {"* for Y & #for N"};
const char lcdEnterNumber[] PROGMEM = {"Enter phone NO:"};
const char lcdCallingNumber[] PROGMEM = {"Calling Number: "};
const char lcdHangingUp[] PROGMEM = {"Hanging up :"};
const char lcdSendingMessage[] PROGMEM = {"Messaging Number :"};
const char lcdAskSerialData[] PROGMEM ={"Enter data to go to Programming mode"};
const char lcdEnteringProgrammingMode[] PROGMEM ={"Entering programming Mode"};
const char lcdSystemReasy[] PROGMEM ={"System Ready"};
const char lcdEnteringMainCode[] PROGMEM = {"Entering main code"};
const char lcdStartUpMessage[] PROGMEM = {"Aayud MS HSS V 2.0"};
const char lcdGSMStartUpError[] PROGMEM = {"GSM Initialization Error"};
const char lcdGSMStartUpSuccess[] PROGMEM = {"GSM Initialization Successfull"};
const char lcdInAlertCycle[] PROGMEM ={"In alert cycle :"};
const char lcdAlertingUser[] PROGMEM = {"Alerting User :"};
const char lcdCallFailure[] PROGMEM ={"Call Failure "};
const char lcdSMSFailure[] PROGMEM = {"SMS Send Failure "};
const char lcdCallSuccess[] PROGMEM = {"Call Successfull "};
const char lcdSMSSuccess[] PROGMEM = {"SMS Successfull "};
const char lcdGSMHangUpSuccess[] PROGMEM = {"GSM Hangup Successfull"};
const char lcdGSMHangUpFailure[] PROGMEM = {"GSM Hangup Failure"};





//Set up a table to refer your string
//Note : Add in the same order as the #define is used to reference the same
const char* const lcd_DisplayTable[] PROGMEM = {lcdEnterCmd,lcdModifyAudio,lcdModifyPhoneNumber, lcdPressStarToStart,lcdPressHashToEnd
,lcdInitializing,lcdReady, lcdInvalidCommand,lcdCancelled,lcdConfirmed, lcdConfirmation, lcdEnterNumber, lcdCallingNumber,lcdHangingUp,lcdSendingMessage, lcdAskSerialData
,lcdEnteringProgrammingMode, lcdSystemReasy, lcdEnteringMainCode, lcdStartUpMessage, lcdGSMStartUpError,lcdGSMStartUpSuccess, lcdInAlertCycle, lcdAlertingUser, lcdCallFailure,
lcdSMSFailure, lcdCallSuccess, lcdSMSSuccess , lcdGSMHangUpSuccess,lcdGSMHangUpFailure  };


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX//

