// ==========================================
// DYNAMIC RUNWAY SAFETY CONTROL SYSTEM
// Ultrasonic + IR + LEDs + Buzzer
// RFID REMOVED
// ==========================================

// -------- Ultrasonic Sensor 1: ENTRY --------
#define TRIG1 2
#define ECHO1 3

// -------- Ultrasonic Sensor 2: RUNWAY/EXIT --------
#define TRIG2 4
#define ECHO2 5

// -------- IR Sensor --------
#define IR 6

// -------- LEDs --------
#define GREEN 7
#define YELLOW 8
#define RED A0

// -------- Buzzer --------
#define BUZZER A1

// Distance at which an object is detected
#define DETECTION_DISTANCE 20


// ==========================================
// ULTRASONIC DISTANCE FUNCTION
// ==========================================

float getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
  {
    return -1;
  }

  float distance = duration * 0.0343 / 2;

  return distance;
}


// ==========================================
// SETUP
// ==========================================

void setup()
{
  Serial.begin(9600);

  // Ultrasonic Sensor 1
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  // Ultrasonic Sensor 2
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  // IR Sensor
  pinMode(IR, INPUT);

  // LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Everything OFF initially
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  noTone(BUZZER);

  Serial.println("======================================");
  Serial.println("DYNAMIC RUNWAY SAFETY CONTROL SYSTEM");
  Serial.println("======================================");
  Serial.println("SYSTEM READY");
  Serial.println("Move your hand toward ENTRY SENSOR");
}


// ==========================================
// MAIN LOOP
// ==========================================

void loop()
{
  // Read Entry Ultrasonic
  float entryDistance = getDistance(TRIG1, ECHO1);

  delay(50);

  // Read Runway Ultrasonic
  float runwayDistance = getDistance(TRIG2, ECHO2);

  // Read IR Sensor
  int irValue = digitalRead(IR);


  // =========================================
  // IR SENSOR HAS HIGHEST PRIORITY
  // =========================================

  // Most common IR modules:
  // LOW = object detected

  if (irValue == LOW)
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);

    digitalWrite(RED, HIGH);

    tone(BUZZER, 1000);

    Serial.println("--------------------------------------");
    Serial.println("IR SENSOR: OBJECT DETECTED");
    Serial.println("RUNWAY OCCUPIED");
    Serial.println("RED LED: ON");
    Serial.println("BUZZER: ON");
    Serial.println("--------------------------------------");

    delay(300);

    return;
  }


  // =========================================
  // NO IR OBJECT
  // =========================================

  digitalWrite(RED, LOW);
  noTone(BUZZER);


  // =========================================
  // HAND APPROACHES ULTRASONIC 1
  // =========================================

  if (entryDistance > 0 &&
      entryDistance <= DETECTION_DISTANCE)
  {
    Serial.println("--------------------------------------");
    Serial.println("OBJECT DETECTED!");

    Serial.print("Entry Distance: ");
    Serial.print(entryDistance);
    Serial.println(" cm");


    // Yellow LED = checking
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);

    Serial.println("YELLOW LED: ON");
    Serial.println("CHECKING RUNWAY...");

    delay(500);


    // =======================================
    // CHECK ULTRASONIC 2
    // =======================================

    runwayDistance = getDistance(TRIG2, ECHO2);

    Serial.print("Runway Distance: ");

    if (runwayDistance == -1)
    {
      Serial.println("No Echo");
    }
    else
    {
      Serial.print(runwayDistance);
      Serial.println(" cm");
    }


    // =======================================
    // RUNWAY CLEAR
    // =======================================

    if (runwayDistance == -1 ||
        runwayDistance > DETECTION_DISTANCE)
    {
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, HIGH);

      Serial.println("RUNWAY CLEAR");
      Serial.println("GREEN LED: ON");
      Serial.println("SAFE TO PROCEED");
    }


    // =======================================
    // RUNWAY OCCUPIED
    // =======================================

    else
    {
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);

      digitalWrite(RED, HIGH);

      tone(BUZZER, 1000);

      Serial.println("RUNWAY OCCUPIED");
      Serial.println("RED LED: ON");
      Serial.println("BUZZER: ON");
    }

    Serial.println("--------------------------------------");

    delay(1000);
  }


  // =========================================
  // NO HAND / OBJECT AT ENTRY
  // =========================================

  else
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);

    // Red should already be OFF because
    // the IR condition was checked above.
    digitalWrite(RED, LOW);

    noTone(BUZZER);

    Serial.println("WAITING FOR APPROACHING OBJECT...");
  }

  delay(200);
}
