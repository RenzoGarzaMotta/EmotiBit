#define ON_BOARD_LED 13

int i = 0;
int brightness = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ON_BOARD_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i = 0; i <= 255; i++){
    brightness = i;
    analogWrite(ON_BOARD_LED, brightness);
    delay(8);
  }
  for(i = 255; i >= 0; i--){
    brightness = i;
    analogWrite(ON_BOARD_LED, brightness);
    delay(8);
  }
  delay(1000);
}
