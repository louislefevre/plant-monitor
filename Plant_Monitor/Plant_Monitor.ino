#include <LiquidCrystal.h>

// Digital Pin Variables
const int pumpRelay1 = 2;
const int pumpRelay2 = 3;
const int pumpRelay3 = 4;
const int pumpRelay4 = 5;
const int lightRelay = 6;
const int lightButton = 7;
const int rs = 8;
const int en = 9;
const int d4 = 10;
const int d5 = 11;
const int d6 = 12;
const int d7 = 13;

// Analog Pin Variables
const int waterSensor = A0;
const int lightSensor = A1;
const int moistureSensor1 = A2;
const int moistureSensor2 = A3;
const int moistureSensor3 = A4;
const int moistureSensor4 = A5;

// Analog Reading Values Variables
int waterLevel = 0;
int lightLevel = 0;
int moistureLevel1 = 0;
int moistureLevel2 = 0;
int moistureLevel3 = 0;
int moistureLevel4 = 0;

// Minimum Conditional Variables
int waterMin = 10; // Higher value more water, lower value less water
int lightMin = 300; // Higher value more light, lower value less light
int moistureMin = 450; // Higher value less water, lower value more water

// Light Control Variables
bool manualLight = false;
int buttonState = 0;
int lastButtonState = 0;
int startPressed = 0;
int endPressed = 0;
int timeHold = 0;
int timeReleased = 0;
int timeMin = 3000;

// Manual Light Variables
int lightState = LOW;
int previous = LOW;
int buttonStateAuto = 0;
unsigned long buttonTime = 0;
const long debounce = 1000;

// Loop Delay Variables
unsigned long currentTime = 0;
const long delayTime = 1000;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  /* Initializes the various processes required for the program to run.
   *
   * Initializes Serial so data can be read, and also sets up the LCD character display.
   * Inputs and outputs are then initialized, with a short delay to ensure relays are
   * given enough time to reset.
   */
  Serial.begin(9600);
  lcd.begin(16, 2);
  initializeInput();
  initializeOutput();
  delay(1000);
}

void initializeInput()
{
  /* Sets the analog pins to recieve input.
   *
   * Initializes the input pins so that they can recieve analog data from the
   * various sensors.
   */
  pinMode(waterSensor, INPUT);
  pinMode(lightSensor, INPUT);
  pinMode(lightButton, INPUT);
  pinMode(moistureSensor1, INPUT);
  pinMode(moistureSensor2, INPUT);
  pinMode(moistureSensor3, INPUT);
  pinMode(moistureSensor4, INPUT);
}

void initializeOutput()
{
  /* Turns off all the relays and then sets their pins as output.
   *
   * To ensure relays are inactive at reset, they are turned off at setup.
   * Single relays are turned off with 'LOW' and turned on with 'HIGH'
   * Multi-relays (2, 4 and 8) are turned off with 'HIGH' and turned on with 'LOW'.
   * The corresponding pins are then set as outputs.
   */
  digitalWrite(lightRelay, LOW);
  digitalWrite(pumpRelay1, HIGH);
  digitalWrite(pumpRelay2, HIGH);
  digitalWrite(pumpRelay3, HIGH);
  digitalWrite(pumpRelay4, HIGH);

  pinMode(lightRelay, OUTPUT);
  pinMode(pumpRelay1, OUTPUT);
  pinMode(pumpRelay2, OUTPUT);
  pinMode(pumpRelay3, OUTPUT);
  pinMode(pumpRelay4, OUTPUT);
}

void collectReadings()
{
  /* Reads the data from the analog inputs, and assigns them to global variables.
   *
   * The global variables previously initialized are assigned values through reading
   * the data being received by the various sensors.
   */
  waterLevel = analogRead(waterSensor);
  lightLevel = analogRead(lightSensor);
  moistureLevel1 = analogRead(moistureSensor1);
  moistureLevel2 = analogRead(moistureSensor2);
  moistureLevel3 = analogRead(moistureSensor3);
  moistureLevel4 = analogRead(moistureSensor4);
}

void printReadings()
{
  /* Prints the analog data to Serial.
   *
   * The data from the analog readings is converted into strings, and then printed to
   * the console. This is mainly used for debugging purposes and seeing data in its
   * rawest form.
   */
  Serial.println("WATER SENSOR LEVEL: " + String(waterLevel));
  Serial.println("LIGHT SENSOR LEVEL: " + String(lightLevel));
  Serial.println("MOISTURE SENSOR 1 LEVEL: " + String(moistureLevel1));
  Serial.println("MOISTURE SENSOR 2 LEVEL: " + String(moistureLevel2));
  Serial.println("MOISTURE SENSOR 3 LEVEL: " + String(moistureLevel3));
  Serial.println("MOISTURE SENSOR 4 LEVEL: " + String(moistureLevel4));
  Serial.println("-------------------------");
}

void lcdDiplay()
{
  /* Displays moisture and light data on the LCD, as well as low water warnings.
   *
   * Averages the moisture readings and converts them to a percentage. Due to the awkward
   * upper/lower averages of the readings, some extra work has to be done here. The same
   * goes for the light percentage.
   * In order to prevent readings appearing as below 0% or above 100%, checks are done
   * to ensure values are capped.
   * The LCD screen is then cleared to prevent any characters accidentally carrying over.
   * The following conditional then shows a warning to refill the water supply, should
   * it fall below minimum levels. If the water level is satisfactory, the moisture
   * and light percentages will be displayed instead.
   */
  int moistureAvg = ((moistureLevel1 + moistureLevel2 + moistureLevel3 + moistureLevel4) / 4) - 300;
  int lightPercent = lightLevel / 7;
  int moisturePercent = (300 - moistureAvg) / 3.5;

  if (lightPercent > 100)
    lightPercent = 100;
  else if (lightPercent < 0)
    lightPercent = 0;

  if (moisturePercent > 100)
    moisturePercent = 100;
  else if (moisturePercent < 0)
    moisturePercent = 0;

  lcd.clear();

  if (waterLevel < waterMin)
  {
    lcd.setCursor(0, 0);
    lcd.print("----WARNING!----");
    lcd.setCursor(0, 1);
    lcd.print("  REFILL WATER  ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Moisture: " + String(moisturePercent) + "%");
    lcd.setCursor(0, 1);
    lcd.print("Light: " + String(lightPercent) + "%");
  }
}

void activatePump(int waterPump, int flowTime)
{
  /* Turns on a single water pump for a specific period.
   *
   * Activates a water pump for a few seconds, then turns it off again. The delay is how
   * long the water will run for, and can be adjusted for each individual plant.
   */
  digitalWrite(waterPump, LOW);
  delay(flowTime);
  digitalWrite(waterPump, HIGH);
}

void waterPumpHandler()
{
  /* Activates each individual water pump if the moisture is low enough.
   *
   * Individual checks are done for each moisture sensor. If the value read is lower than
   * the minimum moisture required, the corresponding water pump is activated.
   */
  if (moistureLevel1 > moistureMin)
    activatePump(pumpRelay1, 5000);

  if (moistureLevel2 > moistureMin)
    activatePump(pumpRelay2, 5000);

  if (moistureLevel3 > moistureMin)
    activatePump(pumpRelay3, 5000);

  if (moistureLevel4 > moistureMin)
    activatePump(pumpRelay4, 5000);
}

void autoLight()
{
  /* Provides automatic control over the light bulb through the use of the values picked
   * up by the photosensor.
   *
   * If the value recieved from the light sensor is below the minimum amount of light
   * required, the light bulb is turned on.
   */
  if (lightLevel < lightMin)
    digitalWrite(lightRelay, HIGH);
  else
    digitalWrite(lightRelay, LOW);
}

void manLight()
{
  /* Provides manual control for the light bulb via a button.
   *
   * Reads the current state of the button to check if its on. If it's being pressed,
   * its previous state was off and it's been more than the debounce duration since
   * the last time it was pressed, then the state of the light is switched. If it was
   * on then it is turned off, otherwise it's turned on. The new light state is then
   * written to the lights relay, and the state of the previous button state is saved.
   *
   * Note: manLight() code sourced from https://www.electroschematics.com/turn-on-led-button-arduino/
   */
  buttonStateAuto = digitalRead(lightButton);
  if (buttonStateAuto == HIGH && previous == LOW && millis() - buttonTime > debounce)
  {
    if (lightState == HIGH)
      lightState = LOW;
    else
      lightState = HIGH;

    buttonTime = millis();
  }
  digitalWrite(lightRelay, lightState);
  previous == buttonStateAuto;
}

void lightControl()
{
  /* Allows the user to switch between auto and manual lighting.
   *
   * Reads the current state of the button and compares it against the previous state.
   * If they are not the same and the button is being pressed, the time it was pressed
   * is logged. Once the button is released, the time of release is logged.
   * The difference between these two values is calculated, which tells us how much
   * time the button was held down for. If held for enough time, the current state of
   * the light control is switched (e.g. auto changes to manual).
   *
   * Note: lightControl() code sourced from https://stackoverflow.com/questions/32427630/how-to-detect-how-long-a-button-was-pressed-in-arduino
   */
  buttonState = digitalRead(lightButton);

  if (buttonState != lastButtonState)
  {
    if (buttonState == HIGH)
    {
      startPressed = millis();
    }
    else
    {
      endPressed = millis();
      timeHold = endPressed - startPressed;

      if (timeHold >= timeMin)
        manualLight = !manualLight;
    }
  }
  lastButtonState = buttonState;

  if (manualLight)
    manLight();
  else
    autoLight();
}

void loop()
{
  /* Loops through each function of the program repeatedly.
   *
   * Majority of the functions run on delayed time (e.g. every 1 second) to help save
   * power, since readings are not necessary to gather very frequently. However, in order
   * to still allow code to run and not stop entirely (as is with the case of delay()),
   * we measure how much time has passed between each loop and only run the enclosed
   * functions when the specific delay time has passed.
   *
   * Note: Timed delay code sourced from https://dzone.com/articles/arduino-using-millis-instead-of-delay
   */
  if (millis() > currentTime + delayTime)
  {
    currentTime = millis();
    collectReadings();
    printReadings();
    lcdDiplay();
    waterPumpHandler();
  }
  lightControl();
}
