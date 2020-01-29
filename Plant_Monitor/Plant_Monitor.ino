// Digital Pin Variables
const int pumpRelay1 = 2;
const int pumpRelay2 = 3;
const int pumpRelay3 = 4;
const int pumpRelay4 = 5;
const int lightRelay = 6;
const int lightButton = 7;

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

void loop()
{
  collectReadings();
  printReadings();
}
