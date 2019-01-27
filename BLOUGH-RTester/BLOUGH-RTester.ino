
//Male USB Pins
#define MALE_GROUND 2
#define MALE_D_PLUS 3
#define MALE_D_MINUS 4
#define MALE_VCC 5
#define MALE_SHIELD 10

//Female USB Pins
#define FEMALE_GROUND 6
#define FEMALE_D_PLUS 7
#define FEMALE_D_MINUS 8
#define FEMALE_VCC 9
#define FEMALE_SHIELD 11

void setup() {

  //Male pins will be set in test

  pinMode(FEMALE_GROUND, INPUT_PULLUP);
  pinMode(FEMALE_D_PLUS, INPUT_PULLUP);
  pinMode(FEMALE_D_MINUS, INPUT_PULLUP);
  pinMode(FEMALE_VCC, INPUT_PULLUP);
  pinMode(FEMALE_SHIELD, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  bool allTestsPassed = true;
  allTestsPassed = allTestsPassed && initialStateTest();
  allTestsPassed = allTestsPassed && mainTest();
  allTestsPassed = allTestsPassed && BridgeTest(0, LOW); // Ground
  allTestsPassed = allTestsPassed && BridgeTest(1, LOW); // D Plus
  allTestsPassed = allTestsPassed && BridgeTest(2, LOW); // D Minus
  allTestsPassed = allTestsPassed && BridgeTest(3, HIGH); //VCC

  if (allTestsPassed) {
    Serial.println("All Tests Passed!");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    while(true){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
}

#define PIN_COUNT 5

int malePins[5] = { MALE_GROUND, MALE_D_PLUS, MALE_D_MINUS, MALE_VCC, MALE_SHIELD };
int femalePins[5] = { FEMALE_GROUND, FEMALE_D_PLUS, FEMALE_D_MINUS, FEMALE_VCC, FEMALE_SHIELD };

bool mainTest() {

  delay(10);
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(malePins[i], OUTPUT);
  }
  delay(10);

  Serial.println("Main Test....");
  for (int i = 0; i < PIN_COUNT; i++) {
    digitalWrite(malePins[i], LOW);
  }

  bool fGoundExpected = digitalRead(FEMALE_GROUND) == LOW;
  bool fDPlusExpected = digitalRead(FEMALE_D_PLUS) == LOW;
  bool fDMinusExpected = digitalRead(FEMALE_D_MINUS) == LOW;
  bool fVccExpected = digitalRead(FEMALE_VCC) == HIGH;
  bool fShieldExpected = digitalRead(FEMALE_SHIELD) == LOW;

  if (fGoundExpected && fDPlusExpected && fDMinusExpected && fVccExpected && fShieldExpected)
  {
    Serial.println("Passed!");
    return true;
  } else {
    Serial.println("FAILED!");
    errorPrinter(fGoundExpected, fDPlusExpected, fDMinusExpected, fVccExpected, fShieldExpected);
    return false;
  }
}

bool BridgeTest(int index, int expectedOutput) {

  for (int i = 0; i < PIN_COUNT; i++) {
    if (i == index) {
      pinMode(malePins[i], OUTPUT);
    } else {
      pinMode(malePins[i], INPUT);
    }
  }
  delay(10);
  digitalWrite(malePins[index], LOW);

  Serial.print("Bridge Test for ");
  Serial.print(index);
  Serial.println(" ....");


  bool failingTest = false;
  for (int i = 0; i < PIN_COUNT; i++) {
    Serial.print("i=");
    Serial.print(i);
    int pinReading = digitalRead(femalePins[i]);
    Serial.print(" pin reading: ");
    Serial.println(pinReading);
    if (i == index) {
      failingTest = (failingTest || pinReading != expectedOutput);
    } else {
      failingTest = (failingTest || pinReading != HIGH);
    }
  }

  if (!failingTest)
  {
    Serial.println("Passed!");
    return true;
  } else {
    Serial.println("FAILED!");
    return false;
  }
}

void errorPrinter(bool fGoundExpected, bool fDPlusExpected, bool fDMinusExpected, bool fVccExpected, bool fShieldExpected) {
  Serial.print("Female Gound was as expected Expected: ");
  Serial.println(fGoundExpected);
  Serial.print("Female DPlus was as Expected: ");
  Serial.println(fDPlusExpected);
  Serial.print("Female DMinus was as Expected: ");
  Serial.println(fDMinusExpected);
  Serial.print("Female Vcc was as Expected: ");
  Serial.println(fVccExpected);
  Serial.print("Female Shield was as Expected: ");
  Serial.println(fShieldExpected);
}

bool initialStateTest() {

  delay(10);
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(malePins[i], INPUT);
  }
  delay(10);

  Serial.println("Initial State Test....");
  bool fGoundExpected = digitalRead(FEMALE_GROUND) == HIGH;
  bool fDPlusExpected = digitalRead(FEMALE_D_PLUS) == HIGH;
  bool fDMinusExpected = digitalRead(FEMALE_D_MINUS) == HIGH;
  bool fVccExpected = digitalRead(FEMALE_VCC) == HIGH;
  bool fShieldExpected = digitalRead(FEMALE_SHIELD) == HIGH;

  if (fGoundExpected && fDPlusExpected && fDMinusExpected && fVccExpected && fShieldExpected)
  {
    Serial.println("Passed!");
    return true;
  } else {
    Serial.println("FAILED!");
    errorPrinter(fGoundExpected, fDPlusExpected, fDMinusExpected, fVccExpected, fShieldExpected);

    return false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
