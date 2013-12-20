/*
  Home Thermostat
  Control a single relay to enable and disable central
  heating on a pre-defined schedule
 */
 
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

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(onboard_led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // Notify the user we have done a loop
  loopSignal();
}
