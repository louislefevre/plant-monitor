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

void loop()
{
}
