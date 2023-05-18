#define triggerPinDepth 2
#define triggerPinFirst 3
#define echoPinDepth 4
#define echoPinFirst 5
#define buzzerPin 12
int ledPins[6] = {
  6, 7, 8,        // depth
  9, 10, 11       // first line
};
int oldDistanceDepth;
int loopIndex;

// ******************************************* SETUP *******************************************
// *********************************************************************************************
// *********************************** DEPTH - CORRECT REANGE **********************************
int linePatternDepth[3] = {140, 150, 200}; // in cm (min, max, first line)
int buzzerTime = 1;
// ********************************* FIRST LINE - CORRECT RANGE ********************************
int linePatternFirst[2] = {50, 60}; // in cm (min, max)
// ***************************** SHOW DISTANCE ON SERIAL PORT MONITOR **************************
// **** CAUTION - RX LED WILL BE BLINKING ******* IF FALSE MEASUREMENT WILL BE MUCH FASTER *****
boolean showDistOnMonitor = true;
// *********************************************************************************************

void setup() {
  if (showDistOnMonitor) {
    Serial.begin (9600);
  }

  pinMode(triggerPinDepth, OUTPUT);
  pinMode(triggerPinFirst, OUTPUT);
  pinMode(echoPinDepth, INPUT);
  pinMode(echoPinFirst, INPUT);

  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);

  oldDistanceDepth = 300;
  loopIndex = 0;
}

void loop() {
  int distanceDepth = measureDist(triggerPinDepth, echoPinDepth);
  int distanceFirst = measureDist(triggerPinFirst, echoPinFirst);
  
  if (showDistOnMonitor) {
    Serial.print("Index: "); Serial.print(loopIndex); Serial.print(", depth: "); Serial.print(distanceDepth); Serial.print(", first: "); Serial.println(distanceFirst);
  }
  
  turnOffLeds();

  if (distanceDepth < linePatternDepth[0]) { // za blisko
    digitalWrite(ledPins[2], HIGH);
    noTone(buzzerPin);
    loopIndex = 0;
  } else if (distanceDepth >= linePatternDepth[0] && distanceDepth <= linePatternDepth[1]) { // wlasciwa glebokosc
    digitalWrite(ledPins[1], HIGH);
    if (loopIndex > buzzerTime) {
      noTone(buzzerPin);
      loopIndex = 100;
    } else {
      tone(buzzerPin, 500);
      loopIndex++;
    }
  } else if (distanceDepth > linePatternDepth[1] && distanceDepth <= linePatternDepth[2]) { // za daleko
    digitalWrite(ledPins[0], HIGH);
    noTone(buzzerPin);
    loopIndex = 0;
  } else { // first line
    noTone(buzzerPin);
    if (distanceFirst >= linePatternFirst[0] && distanceFirst <= linePatternFirst[1]) {
      digitalWrite(ledPins[4], HIGH);
    }
  }

  if (distanceFirst < linePatternFirst[0]) {
    digitalWrite(ledPins[3], HIGH);
  } else if (distanceFirst > linePatternFirst[1]) {
    digitalWrite(ledPins[5], HIGH);
  }

  oldDistanceDepth = distanceDepth;

  //delay(50);
}

int measureDist(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long pulseTime = pulseIn(echoPin, HIGH);
  if (pulseTime > 0) {
    return pulseTime / 58;
  } else {
    return 300;
  }
}

void turnOffLeds() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
