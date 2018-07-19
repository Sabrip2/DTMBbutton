

#include <CapacitiveSensor.h>

// resistor between pins 4 and 2, foil connected to wire which is connected to pin 2
// wiring diagram for hand is in separate diagram

int MOTOR_1 = 11;
//int MOTOR_2 = 11;
//int MOTOR_3 = 10;
int ALARM_PIN = 10;
int BUTTON_PIN = 9;
//int LIGHTS = 6;
const int buttonPin = 9;
long globalValue =0;
int buttonState = 0;         // variable for reading the pushbutton status
int buttonpreviousState = 0; 
int flag = 1;

//resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
CapacitiveSensor cs_4_2 = CapacitiveSensor(5, 6);

// threshold that determines when sensor is touched; depends on value of resistor
int delta = 2000;
int baseline;
boolean on;
int lastButtonVal;

int lightCount = -1;

void setup() {
  pinMode(MOTOR_1, OUTPUT);
//  pinMode(MOTOR_2, OUTPUT);
 // pinMode(MOTOR_3, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  //pinMode(LIGHTS, OUTPUT);
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);
  on = false;
  reset();
  lastButtonVal = digitalRead(BUTTON_PIN);
}

void loop() {
  buttonState = digitalRead(buttonPin);
// check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState != buttonpreviousState) {
    if (buttonState == HIGH) {
      // change the flag
      flag = 1-flag;
      Serial.println(flag);
    } 
    delay(50);
  }
  buttonpreviousState = buttonState;
  if (flag ==1) {
 
      long sensor =  cs_4_2.capacitiveSensor(30);
      
    Serial.println(sensor);
    if (sensor > baseline + delta) {
      globalValue += sensor;
    } else {
      globalValue = 0;
    }
    if(globalValue > 5 * (baseline + delta)) {
      if (!on) {
     //   digitalWrite(LIGHTS, HIGH);
        digitalWrite(ALARM_PIN, LOW);
        digitalWrite(MOTOR_1, LOW);
       // digitalWrite(MOTOR_2, HIGH);
        //digitalWrite(MOTOR_3, HIGH);
        on = true;
      } else {
//        digitalWrite(LIGHTS, LOW);
        digitalWrite(ALARM_PIN, HIGH);
        digitalWrite(MOTOR_1, HIGH);
       // digitalWrite(MOTOR_2, LOW);
        //digitalWrite(MOTOR_3, LOW);
        on = false;
      }
      lightCount = 0;
      delay(500);
    } else {
      digitalWrite(ALARM_PIN, LOW);
      digitalWrite(MOTOR_1, LOW);
      //digitalWrite(MOTOR_2, LOW);
      //digitalWrite(MOTOR_3, LOW);

      if (lightCount != -1 && lightCount != 50) {
      //  digitalWrite(LIGHTS, HIGH);
        lightCount++;
      } else {
      //  digitalWrite(LIGHTS, LOW);
        lightCount = -1;
      }
      
      on = false;
      delay(100);
    }
  } else {
    globalValue = 0;
  }
}

void reset() {
  int sum = 0;
  for (int i = 0; i < 20; i++) {
    sum = sum + cs_4_2.capacitiveSensor(30);
    delay(100);
  }
  baseline = (int) (sum / 20.0);
}

boolean checkButton() {
  int val = digitalRead(BUTTON_PIN);
  Serial.println(val);
  if (val != lastButtonVal) {
    if (val == HIGH) { // just turned on
      reset();
    } else { // just turned off
       digitalWrite(ALARM_PIN, LOW);
       digitalWrite(MOTOR_1, LOW);
      // digitalWrite(MOTOR_2, LOW);
      // digitalWrite(MOTOR_3, LOW);
//       digitalWrite(LIGHTS, LOW);
       on = false;
    }
  }
  lastButtonVal = val;
  return (val == HIGH);
}

