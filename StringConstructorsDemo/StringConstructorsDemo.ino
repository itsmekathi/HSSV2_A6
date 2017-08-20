/* String constructors
 *  Examples of how to create Strings from other data types
 *  
 */
void setup() {
  // Open Serial communication and wait for port to open
  Serial.begin(115200);
  while(!Serial){;}

  Serial.println("\n\nString Constructors: ");
  Serial.println();
}

void loop() {

  unsigned long enterMillis = millis();
  
  // Using a constant String:
  String stringOne = "Hello String";
  Serial.println(stringOne);

  // converting a constant char into a String
  stringOne = String('a');
  Serial.println(stringOne);    // prints "a"

  // converting a constant string into a String object
  String stringTwo = String("This is a String");
  Serial.println(stringTwo);

  // concatenating two strings:
  stringOne = String(stringTwo + " with more");
  Serial.println(stringOne);

  // using a constant integer:
  stringOne = String(13);
  Serial.println(stringOne);

  // Using an int and a base:
  stringOne = String(45, DEC);
  Serial.println(stringOne);

  // using an int and a base (binary)
  stringOne = String(255,BIN);
  Serial.println(stringOne);

  // using a long and a base:
  stringOne = String(millis() , DEC);
  Serial.println(stringOne);

  // using float and the right decimal places:
  stringOne = String(5.968,3);
  Serial.println(stringOne);

  // using a float and less decimal places to use rounding:
  stringOne = String(5.698,2);
  Serial.println(stringOne);

  char myName[] = "KATHIRAVAN";
  stringOne = (String)myName;
  Serial.println(myName);

  Serial.print("Total program execution time is: ");
  Serial.print(millis() - enterMillis);
  Serial.println(" ms");

  // do nothing while true:
  while(true);

}
