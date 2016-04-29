#include <Wire.h>

// Declare input/output
const byte rheostatAnalogInput = A0; // use Analog input 0 for rheostat volgate (range from 0v to 3.3v)
const byte ledDigitalOutput = 6; // use Digital input/output 6 for LED blinking output (frequency from 1Hz to 10Hz according to Rheostat)
const byte motorPWMOutput = 9; // use PWM input/output 9 for motor output (on/off motor)

// Initial value for in/out
bool ledValue = HIGH; // LED is connected to VDD with a resistor. Output LOW to make LED on
int motorValue = 0; // PWM value for Motor control
int rheostatValue; //input volate from Rheostat
byte ledPeriod = 50; // period for led blinking
byte motorOnOffFromMaster = 'L';// default is LOW that indicates the motor is off

// Declare prototypes to avoid the error "was not declared in this scope"
byte SPITransceiver(byte sendingByte);
void SlaveInit(void);

void setup() {
    Serial.begin(9600);
    pinMode(ledDigitalOutput, OUTPUT);
    pinMode(motorPWMOutput, OUTPUT);
    Wire.begin(8);                // join i2c bus with address #8
    Wire.onReceive(receiveEvent); // register event
    Wire.onRequest(requestEvent); // register event
}

void loop() {
  //LED blinking
  digitalWrite(ledDigitalOutput, ledValue);
  delay(ledPeriod);
  ledValue = !ledValue;

  //Motor control
  rheostatValue = analogRead(rheostatAnalogInput);
  motorValue = map(rheostatValue, 0, 1023, 0, 255);
  ledPeriod = map(rheostatValue, 0, 1023, 255, 25);
  if (motorOnOffFromMaster == 'H'){
    analogWrite(motorPWMOutput, motorValue);  
  } else if (motorOnOffFromMaster == 'L') {
    analogWrite(motorPWMOutput, 0);  
  } 
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);      // print the character
  }
  motorOnOffFromMaster = Wire.read();    // receive byte as an integer
  Serial.println(motorOnOffFromMaster);         // print the integer
}

void requestEvent() {
  Wire.write(ledPeriod);
}


