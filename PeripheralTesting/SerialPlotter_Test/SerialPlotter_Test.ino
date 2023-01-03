uint8_t testVar;

void setup(){
  // put your setup code here, to run once:
  Serial.begin(2000000);
}

void loop(){
  // put your main code here, to run repeatedly:
  Serial.println(testVar);
  testVar+=10;
  delay(50);
}
