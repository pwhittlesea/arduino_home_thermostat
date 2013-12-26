/*
 Home Thermostat
 Control a single relay to enable and disable central
 heating on a pre-defined schedule
 */

// include the LCD library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Const.
const int ONBOARD_LED = 13;
const int RELAY_ONE = 6;
const int RELAY_TWO = 5;
const int PRG_0_SIZE = 7;
const int PRG_1_SIZE = 24;
const int CHAR_DEG = 0;

// Vars.
int day = 0;
int hour = 0;
int temp = 21;

// Custom Characters
byte charDegree[8] = {
  B00010,
  B00101,
  B00010,
};

// Heating planner (TODO make user configurable)
int program[PRG_0_SIZE][PRG_1_SIZE] = {
  // Monday
  {
    0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Tuesday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Wednesday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Thursday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Friday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Saturday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
  // Sunday
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 }
  ,
};

/*
  Flash the onboard LED once.
 */
void loopSignal() {
  digitalWrite(ONBOARD_LED, HIGH);
  delay(500);
  digitalWrite(ONBOARD_LED, LOW);
  delay(500);
}

/*
 Check if the heating should be on.
 */
boolean shouldHeatingBeOn() {
  if (hour == 24) {
    day++;
    hour = 0;
  }
  if (day == 7) {
    day = 0;
  }
  return program[day][hour];
}

/*
 Set the state of the screen and the relay to the state specified.
 */
void setHeatingState(boolean on) {
  if (on) {
    updateScreen(5, 0, " On");
    digitalWrite(RELAY_ONE, LOW);
  }
  else {
    updateScreen(5, 0, "Off");
    digitalWrite(RELAY_ONE, HIGH);
  }
}

void nextChangeAt(boolean state, int* change) {
  for (int x = hour; x < 24; x++)  {
    if (program[day][x] != state) {
      change[0] = x;
      change[1] = !state;
      return;
    }
  }
  for (int x = 0; x < PRG_0_SIZE; x++) {
    int* thisDay = program[(day + x + 1) % 7];
    for (int y = 0; y < 24; y++) {
      if (thisDay[y] != state) {
        change[0] = y;
        change[1] = !state;
        return;
      }
    }
  };
  change[0] = 99;
  change[1] = !state;
}

/*
 Update the LCD with a string.
 */
void updateScreen(int x, int y, String text) {
  lcd.setCursor(x, y);
  lcd.print(text);
}

/*
 Update the LCD with a custom char.
 */
void updateScreen(int x, int y, byte text) {
  lcd.setCursor(x, y);
  lcd.write(text);
}

// the setup routine runs once when you press reset:
void setup() {
  // Set up the LCD's number of columns and rows and turn on the display:
  lcd.begin(16, 2);
  lcd.display();

  updateScreen(0, 0, "Initialising...");
  delay(1000);

  // Do all the important stuff for setup here

  // initialize the digital pins as outputs.
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(RELAY_ONE, OUTPUT);
  //pinMode(, OUTPUT);

  // Set the heating to be off by default
  setHeatingState(false);

  // Init chars
  lcd.createChar(CHAR_DEG, charDegree);

  // Initial screen state
  updateScreen(0, 0, "Now: --     -- C");
  updateScreen(14, 0, byte(CHAR_DEG));
  updateScreen(0, 1, "Next:   --- @ --");
}

// the loop routine runs over and over again forever:
void loop() {
  updateScreen(12, 0, String(temp));

  boolean heating = shouldHeatingBeOn();
  setHeatingState(heating);

  int change[2];
  nextChangeAt(heating, change);
  String nextHour = String(change[0]);
  if (change[0] < 10) {
    nextHour = "0" + nextHour;
  }
  updateScreen(14, 1, nextHour);
  updateScreen(8, 1, (change[1]) ? " On" : "Off");

  // Notify the user we have done a loop
  loopSignal();

  hour++;
}
