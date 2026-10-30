const int trigPin = 9;
const int echoPin = 10;

const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 11;

const int buttonPin = 2;

const unsigned long debounceTime = 50;
unsigned long lastButtonChange = 0;

int buttonState = HIGH;
int lastButtonReading = HIGH;

unsigned long lastMeasurement = 0;

// Функция одного измерения
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2;
}

// Медиана трёх измерений
float getMedianDistance() {
  float a = measureDistance();
  delay(30);

  float b = measureDistance();
  delay(30);

  float c = measureDistance();

  if (a < 0 || b < 0 || c < 0) {
    return -1;
  }

  if (a > b) { float t = a; a = b; b = t; }
  if (b > c) { float t = b; b = c; c = t; }
  if (a > b) { float t = a; a = b; b = t; }

  return b;
}

void allLEDsOff() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  allLEDsOff();
}

void loop() {
  // Проверка кнопки
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonReading) {
    lastButtonChange = millis();
  }

  if (millis() - lastButtonChange >= debounceTime) {
    buttonState = reading;
  }

  lastButtonReading = reading;

  // Кнопка отпущена
  if (buttonState == HIGH) {
    allLEDsOff();
    return;
  }

  // Измерение каждые 150 мс
  if (millis() - lastMeasurement >= 150) {
    lastMeasurement = millis();

    float distance = getMedianDistance();

    if (distance < 0) {
      Serial.println("Sensor error");
      allLEDsOff();
      return;
    }

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20) {
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
    }
    else if (distance <= 50) {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(greenLED, LOW);
    }
    else {
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, HIGH);
    }
  }
}
