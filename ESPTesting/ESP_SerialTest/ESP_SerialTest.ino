void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Define the baud-rate of the serial port to 9600 bps
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World"); //Print "Hello World" to the serial console
  delay(1000);//Delay 1 sec between serial print
}
