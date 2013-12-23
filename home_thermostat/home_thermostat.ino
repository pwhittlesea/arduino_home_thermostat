/*
 Home Thermostat
 Control a single relay to enable and disable central
 heating on a pre-defined schedule
 */

// include the LCD library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin 13 is the onboard LED:
int onboard_led = 13;

/*
  Flash the onboard LED once.
 */
void loopSignal() {
  digitalWrite(onboard_led, HIGH); 
  delay(500);
  digitalWrite(onboard_led, LOW);  
  delay(500);
}

/*
 Update the LCD.
 */
void updateScreen(int x, int y, String text) {
  lcd.setCursor(x, y);
  lcd.print(text);
}

// the setup routine runs once when you press reset:
void setup() {                
  // Set up the LCD's number of columns and rows and turn on the display:
  lcd.begin(16, 2);
  lcd.display();

  updateScreen(0, 0, "Initialising... ");
  delay(1000);

  // Do all the important stuff for setup here

  // initialize the digital pin as an output.
  pinMode(onboard_led, OUTPUT);

  // Current time
  updateScreen(0, 1, "      T:");

  updateScreen(0, 0, "Done...         ");
}

// the loop routine runs over and over again forever:
void loop() {
  // print the number of seconds since reset:
  String milli = String(millis()/1000);
  updateScreen(16 - milli.length(), 1, milli);

  // Notify the user we have done a loop
  loopSignal();
}




