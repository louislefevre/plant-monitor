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
  Serial.begin(9600);
  initializeInput();
  initializeOutput();
  delay(1000);
}

void initializeInput()
{
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
  waterLevel = analogRead(waterSensor);
  lightLevel = analogRead(lightSensor);
  moistureLevel1 = analogRead(moistureSensor1);
  moistureLevel2 = analogRead(moistureSensor2);
  moistureLevel3 = analogRead(moistureSensor3);
  moistureLevel4 = analogRead(moistureSensor4);
}

void printReadings()
{
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
  digitalWrite(waterPump, LOW);
  delay(flowTime);
  digitalWrite(waterPump, HIGH);
}

void waterPumpHandler()
{
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
  if (lightLevel < lightMin)
    digitalWrite(lightRelay, HIGH);
  else
    digitalWrite(lightRelay, LOW);
}

void manLight()
{
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
