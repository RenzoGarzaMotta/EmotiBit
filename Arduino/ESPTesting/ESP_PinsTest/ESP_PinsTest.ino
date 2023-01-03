/***************ESP32 PINS******************/

//GPIO
#define PIN4            4   //A5/ADC2/TOUCH0
#define PIN5            5   //SPISCK

#define PIN12           12  //A11/ADC2/TOUCH5
#define PIN13           13  //A12/ADC2/TOUCH4
#define PIN14           14  //A6/ADC2/TOUCH6
#define PIN15           15  //A8/ADC2/TOUCH3
#define PIN16           16  //GPIO
#define PIN17           17  //GPIO
#define PIN18           18  //SPI MOSI
#define PIN19           19  //SPI MISO

#define PIN21           21  //GPIO
#define PIN22           22  //I2C SCL
#define PIN23           23  //I2C SDA

#define PIN25           25  //A1/ADC2/DAC1
#define PIN26           26  //A0/ADC2/DAC2
#define PIN27           27  //A10/ADC2/TOUCH7

#define PIN32           32  //A7/ADC1/TOUCH9
#define PIN33           33  //A9/ADC1/TOUCH8
#define PIN34           34  //INPUT ONLY
#define PIN36           36  //INPUT ONLY
#define PIN39           39  //INPUT ONLY 

//CircyitPython Name
#define A0              26
#define A1              25
#define A2              34
#define A3              39
#define A4              36
#define A5              4

#define SCK             5
#define MOSI            18
#define MISO            16

#define SCL             22
#define SDA             23

#define IO12            12
#define IO13            13
#define IO14            14
#define IO15            15
#define IO16            16
#define IO17            17

#define IO21            21
#define IO27            27
#define IO32            32
#define IO33            33

//ADC
#define ADC1_CH0        36
#define ADC1_CH3        39
#define ADC1_CH4        32
#define ADC1_CH5        33
#define ADC1_CH6        34

#define ADC2_CH0        4
#define ADC2_CH3        15
#define ADC2_CH4        13
#define ADC2_CH5        12
#define ADC2_CH6        14
#define ADC2_CH7        27
#define ADC2_CH8        25
#define ADC2_CH9        26

//HS/QSPI
#define HSPIHD          4
#define VSPICS0         5
#define VSPICLK         18
#define VSPIQ           19

#define VSPIHD          21

#define HSPID           13
#define HSPIQ           12

#define HSPICS0         15

#define HSPICLK         14
#define VSPIWP          22
#define VSPID           23

//SDMMC
#define HS1_DATA4       16
#define HS1_DATA5       17
#define HS1_DATA6       5
#define HS1_DATA7       18
#define HS1_STROBE      23


#define HS2_DATA1       4
#define HS2_DATA2       12
#define HS2_DATA3       13
#define HS2_CMD         15
#define HS2_CLK         14

//Touch
#define T0              4
#define T3              15
#define T4              13
#define T5              12
#define T6              14
#define T7              27
#define T8              33
#define T9              32

//RTC
#define RTC0            36
#define RTC3            39
#define RTC4            34
#define RTC6            25
#define RTC7            26
#define RTC8            33
#define RTC9            32
#define RTC10           4
#define RTC13           15
#define RTC14           13
#define RTC15           12
#define RTC16           14
#define RTC17           27

//UART
#define U2RXD           16
#define U2TXD           17
#define U0CTS           19
#define U0RTS           22

//Other
#define DAC2            26
#define DAC1            25
#define VDET1           34
#define SENSE_V_NEG     39
#define SENSE_V_POS     36

#define MTCK            13
#define MTDI            12

#define XN_32K          33
#define MTDO            15
#define XP_32K          32
#define MTMS            14

//Special
#define IN_ONLY1        34
#define IN_ONLY2        36
#define IN_ONLY3        39
#define ON_BOARD_LED    13  //

#define DELAY 250

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);

  pinMode(PIN12, OUTPUT);
  pinMode(PIN13, OUTPUT);
  pinMode(PIN14, OUTPUT);
  pinMode(PIN15, OUTPUT);
  pinMode(PIN16, OUTPUT);
  pinMode(PIN17, OUTPUT);
  pinMode(PIN18, OUTPUT);
  pinMode(PIN19, OUTPUT);

  pinMode(PIN21, OUTPUT);
  pinMode(PIN22, OUTPUT);
  pinMode(PIN23, OUTPUT);

  pinMode(PIN25, OUTPUT);
  pinMode(PIN26, OUTPUT);
  pinMode(PIN27, OUTPUT);

  pinMode(PIN32, OUTPUT);
  pinMode(PIN33, OUTPUT);
}

void loop() {
  // // put your main code here, to run repeatedly:
  // digitalWrite(PIN4, HIGH);
  // digitalWrite(PIN5, HIGH);

  // digitalWrite(PIN12, HIGH);
  // digitalWrite(PIN13, HIGH);
  // digitalWrite(PIN14, HIGH);
  // digitalWrite(PIN15, HIGH);
  // digitalWrite(PIN16, HIGH);
  // digitalWrite(PIN17, HIGH);
  // digitalWrite(PIN18, HIGH);
  // digitalWrite(PIN19, HIGH);

  // digitalWrite(PIN21, HIGH);
  // digitalWrite(PIN22, HIGH);
  // digitalWrite(PIN23, HIGH);

  // digitalWrite(PIN25, HIGH);
  // digitalWrite(PIN26, HIGH);
  // digitalWrite(PIN27, HIGH);

  // digitalWrite(PIN32, HIGH);
  // digitalWrite(PIN33, HIGH);
  // delay(DELAY);
  // digitalWrite(PIN4, LOW);
  // digitalWrite(PIN5, LOW);

  // digitalWrite(PIN12, LOW);
  // digitalWrite(PIN13, LOW);
  // digitalWrite(PIN14, LOW);
  // digitalWrite(PIN15, LOW);
  // digitalWrite(PIN16, LOW);
  // digitalWrite(PIN17, LOW);
  // digitalWrite(PIN18, LOW);
  // digitalWrite(PIN19, LOW);

  // digitalWrite(PIN21, LOW);
  // digitalWrite(PIN22, LOW);
  // digitalWrite(PIN23, LOW);

  // digitalWrite(PIN25, LOW);
  // digitalWrite(PIN26, LOW);
  // digitalWrite(PIN27, LOW);

  // digitalWrite(PIN32, LOW);
  // digitalWrite(PIN33, LOW);
  // delay(DELAY);
}