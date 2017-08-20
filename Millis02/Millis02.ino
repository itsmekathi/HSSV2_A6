// these variables store the flash pattern
// and the current state of the LED


int ledPin = 13;    // The number of the LED pin
int ledState = LOW;     
unsigned long previousMillis = 0;
long OnTime = 250;
long OffTime = 750;




void setup() {
  // set the digital pin as output"
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // check to see if it's time to change the state of the LED
  unsigned long currentMillis = millis();

  if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
  {
    ledState = LOW;
    previousMillis  = currentMillis;
    digitalWrite(ledPin,ledState);
  }
  else if ((ledState ==  LOW) && (currentMillis - previousMillis >= OffTime))
  {
    ledState = HIGH;
    previousMillis = currentMillis;
    digitalWrite(ledPin, ledState);
  }
}
