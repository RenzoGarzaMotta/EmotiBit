#define ROT_BTN 21
#define ROT_A   22
#define ROT_B   23

const uint32_t SERIAL_BAUD = 2000000; //115200

bool btn1Flag, A, B, prevA, prevB, print;
int clicks, position;

uint8_t debounceEncoderButton(void);
uint8_t debounceEncoderA(void);
uint8_t debounceEncoderB(void);

enum direction{
  CW, 
  CCW, 
  NA
} state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);
	Serial.println("Serial started");

  pinMode(ROT_BTN, INPUT_PULLUP);
  attachInterrupt(ROT_BTN, rotaryButtonISR, FALLING);

  pinMode(ROT_A, INPUT_PULLUP);
  attachInterrupt(ROT_A, encoderA_ISR, CHANGE);

  pinMode(ROT_B, INPUT_PULLUP);
  attachInterrupt(ROT_B, encoderB_ISR, CHANGE);

  // state = CW;
  // Serial.println(state);
  // state = CCW;
  // Serial.println(state);
  // state = NA;
  // Serial.println(state);

  A = digitalRead(ROT_A);
  B = digitalRead(ROT_B);
  prevA = A;
  prevB = B;
}

void loop() {
  if(btn1Flag){
    Serial.print("Button Pressed ");
    Serial.println(btn1Flag);
    btn1Flag = 0;
  }
  if(print){
    Serial.print("Encoder Location ");
    Serial.println(position);
    print = 0;
  }
}

void encoderA_ISR(void){
  delayMicroseconds(2000);
  print = 1;
  A = digitalRead(ROT_A);
  B = digitalRead(ROT_B);


  if(((prevA == 1) && (prevB == 0)) && ((A == 1) && (B == 1))){
    state = CW;
  }else if(((prevA == 1) && (prevB == 0)) && ((A == 0) && (B == 0))){
    state = CCW;
  }
  
  if(((prevA == 1) && (prevB == 1)) && ((A == 0) && (B == 1))){
    state = CW;
  }else if(((prevA == 1) && (prevB == 1)) && ((A == 1) && (B == 0))){
    state = CCW;
  }
  
  if(((prevA == 0) && (prevB == 1)) && ((A == 0) && (B == 0))){
    state = CW;
  }else if(((prevA == 0) && (prevB == 1)) && ((A == 1) && (B == 1))){
    state = CCW;
  }
  
  if(((prevA == 0) && (prevB == 0)) && ((A == 1) && (B == 0))){
    state = CW;
  }else if(((prevA == 0) && (prevB == 0)) && ((A == 0) && (B == 1))){
    state = CCW;
  }
  
  if(state == CW){
    clicks++;
  }else if(state == CCW){
    clicks--;
  }else{
    clicks = clicks;
  }

  position = clicks/4;

  prevA = A;
  prevB = B;
}

void encoderB_ISR(void){
  delayMicroseconds(2000);
  print = 1;
  A = digitalRead(ROT_A);
  B = digitalRead(ROT_B);

  if(((prevA == 1) && (prevB == 0)) && ((A == 1) && (B == 1))){
    state = CW;
  }else if(((prevA == 1) && (prevB == 0)) && ((A == 0) && (B == 0))){
    state = CCW;
  }
  
  if(((prevA == 1) && (prevB == 1)) && ((A == 0) && (B == 1))){
    state = CW;
  }else if(((prevA == 1) && (prevB == 1)) && ((A == 1) && (B == 0))){
    state = CCW;
  }
  
  if(((prevA == 0) && (prevB == 1)) && ((A == 0) && (B == 0))){
    state = CW;
  }else if(((prevA == 0) && (prevB == 1)) && ((A == 1) && (B == 1))){
    state = CCW;
  }
  
  if(((prevA == 0) && (prevB == 0)) && ((A == 1) && (B == 0))){
    state = CW;
  }else if(((prevA == 0) && (prevB == 0)) && ((A == 0) && (B == 1))){
    state = CCW;
  }

  if(state == CW){
    clicks++;
  }else if(state == CCW){
    clicks--;
  }else{
    clicks = clicks;
  }

  position = clicks/4;

  prevA = A;
  prevB = B;
}

void rotaryButtonISR(void){
  delayMicroseconds(1500);
  if(digitalRead(ROT_BTN) == LOW){         //Check that button1 has been pressed
    btn1Flag = 1;           //Set button1 flag to indicate that button1 was pressed
  }
}
