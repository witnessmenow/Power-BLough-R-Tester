
//Male USB Pins
#define MALE_GROUND 2
#define MALE_D_PLUS 3
#define MALE_D_MINUS 4
#define MALE_VCC 5

//Female USB Pins
#define FEMALE_GROUND 6
#define FEMALE_D_PLUS 7
#define FEMALE_D_MINUS 8
#define FEMALE_VCC 9

void setup() {

  //Male pins will be set in test

  pinMode(FEMALE_GROUND, INPUT_PULLUP);
  pinMode(FEMALE_D_PLUS, INPUT_PULLUP);
  pinMode(FEMALE_D_MINUS, INPUT_PULLUP);
  pinMode(FEMALE_VCC, INPUT_PULLUP);

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

int malePins[4] = { MALE_GROUND, MALE_D_PLUS, MALE_D_MINUS, MALE_VCC };
int femalePins[4] = { FEMALE_GROUND, FEMALE_D_PLUS, FEMALE_D_MINUS, FEMALE_VCC };

bool mainTest() {

  delay(10);
  pinMode(MALE_GROUND, OUTPUT);
  pinMode(MALE_D_PLUS, OUTPUT);
  pinMode(MALE_D_MINUS, OUTPUT);
  pinMode(MALE_VCC, OUTPUT);
  delay(10);

  Serial.println("Main Test....");
  digitalWrite(MALE_GROUND, LOW);
  digitalWrite(MALE_D_PLUS, LOW);
  digitalWrite(MALE_D_MINUS, LOW);
  digitalWrite(MALE_VCC, LOW);

  bool fGoundExpected = digitalRead(FEMALE_GROUND) == LOW;
  bool fDPlusExpected = digitalRead(FEMALE_D_PLUS) == LOW;
  bool fDMinusExpected = digitalRead(FEMALE_D_MINUS) == LOW;
  bool fVccExpected = digitalRead(FEMALE_VCC) == HIGH;

  if (fGoundExpected && fDPlusExpected && fDMinusExpected && fVccExpected)
  {
    Serial.println("Passed!");
    return true;
  } else {
    Serial.println("FAILED!");
    errorPrinter(fGoundExpected, fDPlusExpected, fDMinusExpected, fVccExpected);
    return false;
  }
}

bool BridgeTest(int index, int expectedOutput) {

  for (int i = 0; i < 4; i++) {
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
  for (int i = 0; i < 4; i++) {
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

void errorPrinter(bool fGoundExpected, bool fDPlusExpected, bool fDMinusExpected, bool fVccExpected) {
  Serial.print("Female Gound was as expected Expected: ");
  Serial.println(fGoundExpected);
  Serial.print("Female DPlus was as Expected: ");
  Serial.println(fDPlusExpected);
  Serial.print("Female DMinus was as Expected: ");
  Serial.println(fDMinusExpected);
  Serial.print("Female Vcc was as Expected: ");
  Serial.println(fVccExpected);
}

bool initialStateTest() {

  delay(10);
  pinMode(MALE_GROUND, INPUT);
  pinMode(MALE_D_PLUS, INPUT);
  pinMode(MALE_D_MINUS, INPUT);
  pinMode(MALE_VCC, INPUT);
  delay(10);

  Serial.println("Initial State Test....");
  bool fGoundExpected = digitalRead(FEMALE_GROUND) == HIGH;
  bool fDPlusExpected = digitalRead(FEMALE_D_PLUS) == HIGH;
  bool fDMinusExpected = digitalRead(FEMALE_D_MINUS) == HIGH;
  bool fVccExpected = digitalRead(FEMALE_VCC) == HIGH;

  if (fGoundExpected && fDPlusExpected && fDMinusExpected && fVccExpected)
  {
    Serial.println("Passed!");
    return true;
  } else {
    Serial.println("FAILED!");
    errorPrinter(fGoundExpected, fDPlusExpected, fDMinusExpected, fVccExpected);

    return false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
