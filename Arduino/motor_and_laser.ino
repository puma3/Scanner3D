int laser_pin = 13;
int motor_pin = 12;

int serialData = 0;           // data and serial data are just variables to hold information. 
bool data = 0;
char incomingByte;

void setup() {
  Serial.begin(9600);
  pinMode(laser_pin, OUTPUT);
  pinMode(motor_pin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    serialData = Serial.read();
    Serial.println(serialData);
    incomingByte = (char)Serial.read();
//    Serial.print((char)Serial.read());
    data = true;
  }

  if (data) {
//  if(incomingByte == 's') {
    digitalWrite(laser_pin, HIGH);
    digitalWrite(motor_pin, HIGH);
    delay(69400);
    digitalWrite(motor_pin, LOW);
    delay(5000);
    digitalWrite(laser_pin, LOW);
    data = false;
  }
}
