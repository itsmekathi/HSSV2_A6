/* Blink without delay
 *  
 *  Turns on and off a light emitting diode(LED) connected to a digital
 *  pin, without using the delay() function, This means that other code
 *  can run at the same time without being interrupted by the LED code.
 *  
 *  The Circuit:
 *  * LED attached to pin 13 to ground.
 *  
 */

 // set pin numbers:
const int ledPin = 13;  // the number of the LED pin

// variables will change:
int ledState = LOW;                     // ledState used to set the LED
unsigned long previousMillis = 0;       // will store last time LED was updated

 long interval = 1000;                  // interval at which to blink(milliseconds)
 
void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval){
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if(ledState == LOW)
    ledState = HIGH;
    else 
    ledState = LOW;


    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }

}
