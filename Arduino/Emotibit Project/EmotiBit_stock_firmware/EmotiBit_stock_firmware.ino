#include "EmotiBit.h"
#include "EmotiBitVersionController.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <arduinoFFT.h>
#include <SPI.h>

// #define SAMPLES       128//Number of samples in vReal data. MUST ALWAYS be a power of 2
// #define SAMPLING_FREQ 0.2//Sampling Frequency of the data

#define X_LIM 240
#define Y_LIM 280
#define HISTORY_LENGTH 1936 //60s = 1714
#define SerialUSB SERIAL_PORT_USBVIRTUAL // Required to work in Visual Micro / Visual Studio IDE
const uint32_t SERIAL_BAUD = 2000000; //115200


// float findMin(float *array, int size);
// float findMax(float *array, int size);
// void updateLCD(void);
// void printGrid(void);
// void printGraph(int row, int col, float* variable, int color);

// void ppgIR_print(int row, int col, int color);
// void ppgRED_print(int row, int col, int color);
// void ppgGREEN_print(int row, int col, int color);
// void EDA_print(int row, int col, int color);
// void EDL_print(int row, int col, int color);
// void temp0_print(int row, int col, int color);
// void temp1_print(int row, int col, int color);
// void thermopile_print(int row, int col, int color);
// void humid_print(int row, int col, int color);
// void accelX_print(int row, int col, int color);
// void accelY_print(int row, int col, int color);
// void accelZ_print(int row, int col, int color);
// void gyroX_print(int row, int col, int color);
// void gyroY_print(int row, int col, int color);
// void gyroZ_print(int row, int col, int color);
// void magnetX_print(int row, int col, int color);
// void magnetY_print(int row, int col, int color);
// void magnetZ_print(int row, int col, int color);
// void battery_print(int row, int col, int color);
// void findLocalMaximaMinima(int n, float* arr);
// float calculateBPM(void);
// int rollingAvg(int roll);

char hrStr[5], battStr[5], ambTempStr[5], bodyTempStr[5], roseStr[5], compStr[5], SpO2Str[10];

EmotiBit emotibit;
const size_t dataSize = EmotiBit::MAX_DATA_BUFFER_SIZE;
float data[dataSize];

float hist[HISTORY_LENGTH];

float tempPeakCheck, ppgIR[Y_LIM], normal_ppgIR[Y_LIM]; //Set the size to the widest part of the screen
float ppgRED[Y_LIM], normal_ppgRED[Y_LIM]; //Set the size to the widest part of the screen
float ppgGREEN[Y_LIM], normal_ppgGREEN[Y_LIM]; //Set the size to the widest part of the screen
float EDA[Y_LIM], normal_EDA[Y_LIM]; //Set the size to the widest part of the screen
float EDL[Y_LIM], normal_EDL[Y_LIM]; //Set the size to the widest part of the screen
float EDR[Y_LIM], normal_EDR[Y_LIM]; //Set the size to the widest part of the screen
float temp0[Y_LIM], normal_temp0[Y_LIM]; //Set the size to the widest part of the screen
float temp1[Y_LIM], normal_temp1[Y_LIM]; //Set the size to the widest part of the screen
float thermopile[Y_LIM], normal_thermopile[Y_LIM]; //Set the size to the widest part of the screen
float humid[Y_LIM], normal_humid[Y_LIM]; //Set the size to the widest part of the screen
float accelX[Y_LIM], normal_accelX[Y_LIM]; //Set the size to the widest part of the screen
float accelY[Y_LIM], normal_accelY[Y_LIM]; //Set the size to the widest part of the screen
float accelZ[Y_LIM], normal_accelZ[Y_LIM]; //Set the size to the widest part of the screen
float gyroX[Y_LIM], normal_gyroX[Y_LIM]; //Set the size to the widest part of the screen
float gyroY[Y_LIM], normal_gyroY[Y_LIM]; //Set the size to the widest part of the screen
float gyroZ[Y_LIM], normal_gyroZ[Y_LIM]; //Set the size to the widest part of the screen
float magnetX[Y_LIM], normal_magnetX[Y_LIM]; //Set the size to the widest part of the screen
float magnetY[Y_LIM], normal_magnetY[Y_LIM]; //Set the size to the widest part of the screen
float magnetZ[Y_LIM], normal_magnetZ[Y_LIM]; //Set the size to the widest part of the screen
float batt[Y_LIM], normal_batt[Y_LIM]; //Set the size to the widest part of the screen
float currentHR, fftFreq;
int minList[HISTORY_LENGTH] = {0}, maxList[HISTORY_LENGTH] = {0};
int indMin = 0, indMax = 0;
int rollCount, bpmHist[HISTORY_LENGTH];
int battRead = 77, ambTemp = 70, bodyTemp = 96;
int compassRose;

int i, j, size;
int textColor = ST77XX_WHITE, bgColor = ST77XX_BLACK;//, graphColor = ST77XX_CYAN;
int graphWidth = 240, graphMin = 0, graphMax = 62;
int histLength = HISTORY_LENGTH, lastTime, deltaTime;
int peakDelta, currPeakTime, lastPeakTime, peakCount, countPeak;
int hrTime, currTime, sampleCount;

#define DELTA_HIST 30
int peakDeltaHist[DELTA_HIST];

//EmotiBit I2C Defined Pins
//SDA     -> Pin 27
//SCL     -> Pin 13

//EmotiBit SPI Defined Pins
//SCLK    -> Pin 5
//MOSI    -> Pin 18
//MISO    -> Pin 19
//SD_CS   -> Pin 4
//TFT_CS  -> Pin 16

//EmotiBit Misc. Defined Pins
//HIB Pin -> Pin 32


#define TFT_RST        15
#define TFT_CS         16
#define TFT_DC         17

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

extern int reset;

void onShortButtonPress(){
	// toggle wifi on/off
	if (emotibit.getPowerMode() == EmotiBit::PowerMode::NORMAL_POWER){
		emotibit.setPowerMode(EmotiBit::PowerMode::WIRELESS_OFF);
		Serial.println("PowerMode::WIRELESS_OFF");
	}else{
		emotibit.setPowerMode(EmotiBit::PowerMode::NORMAL_POWER);
		Serial.println("PowerMode::NORMAL_POWER");
	}
}

void onLongButtonPress(){
	emotibit.sleep();
}

void setup(){
  uint16_t x,y;
	Serial.begin(SERIAL_BAUD);
	Serial.println("Serial started");
	// delay(2000);	// short delay to allow user to connect to serial, if desired

	emotibit.setup();

  EmotiBitVersionController emotiBitVersionController;
  emotiBitVersionController.echoPinMapping();

	// Attach callback functions
	emotibit.attachShortButtonPress(&onShortButtonPress);
	emotibit.attachLongButtonPress(&onLongButtonPress);

  // Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  //use this initializer (uncomment) if using a 1.69" 280x240 TFT:
  tft.init(240, 280);           // Init ST7789 280x240
  tft.fillScreen(bgColor);
  tft.setCursor(55, 2);
  tft.setTextSize(2);
  tft.setTextColor(textColor, bgColor);
  tft.println("EmotiViewer");

  //Print Wifi Connection Marker
  tft.setCursor(35, 270);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_BLACK, ST77XX_GREEN);
  tft.println("Conn");

  //Print Battery Indicator
  tft.setCursor(110, 270);
  tft.setTextSize(2);
  tft.setTextColor(textColor, bgColor);
  sprintf(battStr, "%d %c", battRead, 37);
  tft.println(battStr);

  //Print Ambient Temperature Indicator
  tft.setCursor(190, 220);
  tft.setTextSize(2);
  tft.setTextColor(textColor, bgColor);
  sprintf(ambTempStr, "%d%cF", ambTemp, 247);
  tft.println(ambTempStr);

  //Print Body Temperature Indicator
  tft.setCursor(190, 235);
  tft.setTextSize(2);
  tft.setTextColor(textColor, bgColor);
  sprintf(bodyTempStr, "%d%cF", bodyTemp, 247);
  tft.println(bodyTempStr);

  //Print Compass Direction
  // printArrow(2, 236, 0, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 1, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 2, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 3, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 4, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 5, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 6, ST77XX_RED, ST77XX_WHITE);
  // delay(2000);
  // printArrow(2, 236, 7, ST77XX_RED, ST77XX_WHITE);

  //Print Menu Button
  tft.setCursor(175, 250);
  tft.setTextSize(2);
  tft.setTextColor(textColor, bgColor);
  tft.println("Menu");


  tft.drawFastHLine(0, 20, 240, textColor);

  for(y = 20; y < Y_LIM; y+= 65){
    tft.drawFastHLine(0, y, 240, textColor);
  }
  tft.drawFastVLine(120, 20, 195, textColor);
}


void loop(){
  reset = 0;
	//Serial.println("emotibit.update()");
	emotibit.update();
  ppgIR_print(1, 1, ST77XX_CYAN);
  ppgRED_print(2, 1, ST77XX_RED);
  ppgGREEN_print(3, 1, ST77XX_GREEN);

  // magnetX_print(1, 2, ST77XX_ORANGE);
  // magnetY_print(2, 2, ST77XX_CYAN);
  // magnetZ_print(3, 2, ST77XX_YELLOW);

  // float redDC = extractDC(ppgRED);
  // float irDC = extractDC(ppgIR);

  // float redAC = ppgRED[graphWidth - 1] - redDC;
  // float irAC = ppgIR[graphWidth - 1] - irDC;
  
  // printf("ppgRed: %f, redDC: %f, SpO2: %f   ", ppgRED[graphWidth - 1], redDC, (ppgRED[graphWidth - 1] / redDC) * 100);
  // printf("ppgInfrared: %f, irDC: %f, SpO2: %f\n", ppgIR[graphWidth - 1], irDC, (ppgIR[graphWidth - 1] / irDC) * 100);

  rollingAvg(50);



/////////////////// ////TESTING BELOW////////////////////////

  printf("\nAVERAGE: %f",avgArray(peakDeltaHist, DELTA_HIST));
  printf("\nRMSSD: %f",RMSSD(peakDeltaHist, DELTA_HIST));
  printf("\nSTD DEV: %f\n\n",standardDeviation(peakDeltaHist, DELTA_HIST));
}

float avgArray(int arrayInput[], int arrayLength) {
  float sum = 0;
  for (int i = 0; i < arrayLength; i++) {
    sum += static_cast<float>(arrayInput[i]);
  }
  return (sum / arrayLength);
}

float standardDeviation(int arr[], int size){ 
  float mean = 0.0, sum_deviation = 0.0; 
  int i;

  for(i = 0; i < size; ++i){ 
    mean += arr[i]; 
  }
 
  mean = mean/size; 
 
  for(i = 0; i < size; ++i){ 
    sum_deviation += (arr[i] - mean)*(arr[i] - mean); 
  }
 
  return sqrt(sum_deviation/size);
}

float RMSSD(int arr[], int size){ 
  float mean = 0.0, sum_deviation = 0.0; 
  int i;

  for(i = 0; i < size - 1; ++i){ 
    mean += ((arr[i] - arr[i+1])*(arr[i] - arr[i+1])); 
  }
 
  mean = mean/size; 
 
  // for(i = 0; i < size - 1; ++i){ 
  //   sum_deviation += (arr[i] - arr[i+1])*(arr[i] - arr[i+1]); 
  // }
 
  return sqrt(mean);
}

float average(float array[]) {
  float sum = 0;
  int arraySize = sizeof(array)/sizeof(array[0]);

  for (int i = 0; i < arraySize; i++) {
    sum += array[i];
  }

  return (sum/arraySize);
}

float HRV(void){

}

//Write a function to calculate oxygen saturation from PPG data from MAX30101 Sensor for Arduino. Let the function take as input the raw ppg data from the sensor
float calculateSpO2(float ppgRed, float ppgInfrared){
  // // Use a low-pass filter to remove the DC component and isolate the AC component
  // // (Note: this is a simplified example and may not be accurate)
  // float ACRed = ppgRed - ((ppgRed + ppgInfrared) / 2);
  // float ACIR = ppgInfrared - ((ppgRed + ppgInfrared) / 2);

  // // Calculate the ratio of the AC components of the PPG signals
  // float ACRatio = ACRed / ACIR;

  // // Apply the oxygen saturation formula
  // // (Note: this formula is for reference only and may not be accurate)
  // float SpO2 = (-45.060 * ACRatio * ACRatio) + (30.354 * ACRatio) + 94.845;

  // float redDC = extractDC(ppgRED);
  // float irDC = extractDC(ppgIR);

  // float redAC = ppgRED[graphWidth - 1] - redDC;
  // float irAC = ppgIR[graphWidth - 1] - irDC;

  float SpO2 = (ppgInfrared / ppgRed) * 100;

  // printf("ppgRed: %f, ppgInfrared: %f, ACRed: %f, ACIR: %f, SpO2: %f\n", ppgRed, ppgInfrared, ACRed, ACIR, SpO2);
  // printf("ppgRed: %f, ppgInfrared: %f, SpO2: %f\n", ppgRed, ppgInfrared, SpO2);

  // tft.fillRect(60, 220, 84, 14, bgColor);
  // tft.setCursor(60, 220);
  // tft.setTextSize(2);
  // tft.setTextColor(textColor, bgColor);
  // sprintf(SpO2Str, "SpO2:%f", SpO2);
  // tft.println(SpO2Str);

  // printf("ppgRed: %f, redDC: %f, redAC: %f, ppgInfrared: %f, irDC: %f, irAC: %f\n", ppgRED[graphWidth - 1], redDC, redAC, ppgIR[graphWidth - 1], irDC, irAC);

  // Return the calculated oxygen saturation value
  return SpO2;
}

float extractDC(float ppg[]) {
  float sum = 0.0;
  
  for (int i = 0; i < sizeof(ppg); i++) {
    sum += ppg[i];
  }
  
  return sum / sizeof(ppg);
}

void ppgIR_print(int row, int col, int color){
  size_t available_ppgIR = emotibit.readData(EmotiBit::DataType::PPG_INFRARED, &data[0], dataSize);
  if (available_ppgIR > 0){
    sampleCount += available_ppgIR;
    currTime = (millis() - lastTime);
    hrTime += currTime;
    deltaTime = (millis() - lastTime) / available_ppgIR;
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_ppgIR; j++){
      for(i = 0; i < graphWidth; i++){
        ppgIR[i] = ppgIR[i + 1];
      }
      for(i = 0; i < histLength; i++){
        hist[i] = hist[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_ppgIR && i < dataSize; i++){
      ppgIR[graphWidth - available_ppgIR + i] = data[i];
      hist[(HISTORY_LENGTH - available_ppgIR) + i] = data[i];

      //Check if peak has been reached as data is being read
      if((ppgIR[graphWidth - available_ppgIR + i - 2] < ppgIR[graphWidth - available_ppgIR + i - 1])&&(ppgIR[graphWidth - available_ppgIR + i - 1] > ppgIR[graphWidth - available_ppgIR + i])){
        //Only check every other peak given normal PPG data.
          if(countPeak == 1){
            peakCount++;

            for(int i = 0; i < DELTA_HIST - 1; i++){
              peakDeltaHist[i] = peakDeltaHist[i + 1];
              printf("%d: %d\n", i, peakDeltaHist[i]);
            }

            //Get current time
            currPeakTime = millis();
            peakDelta = currPeakTime - lastPeakTime;
            peakDeltaHist[DELTA_HIST - 1] = peakDelta;
            printf("%d: %d\n", DELTA_HIST - 1, peakDeltaHist[DELTA_HIST - 1]);
            //Store the previous time there was a peak in the data.
            lastPeakTime = currPeakTime;
            countPeak = 0;
          }else{
            countPeak = 1;
          }
      }
		}

    for(i = 0; i < graphWidth; i++){
      normal_ppgIR[i] = normalize(ppgIR[i], findMin(ppgIR, graphWidth), findMax(ppgIR, graphWidth), graphMin, graphMax);
    }
    printGraph(20+(65*row), col, normal_ppgIR, color);
    lastTime = millis();
    tempPeakCheck = 60/(static_cast<float>(peakDelta)/1000);
    // printf("peakDelta: %d ms, hr: %f\n", peakDelta, tempPeakCheck);
  }
}

void ppgRED_print(int row, int col, int color){
  size_t available_ppgRED = emotibit.readData(EmotiBit::DataType::PPG_RED, &data[0], dataSize);
  if (available_ppgRED > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_ppgRED; j++){
      for(i = 0; i < graphWidth; i++){
        ppgRED[i] = ppgRED[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_ppgRED && i < dataSize; i++){
      ppgRED[graphWidth - available_ppgRED + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_ppgRED[i] = normalize(ppgRED[i], findMin(ppgRED, graphWidth), findMax(ppgRED, graphWidth), graphMin, graphMax);
      // Serial.println(normal_ppgRED[i]);
    }
    printGraph(20+(65*row), col, normal_ppgRED, color);
  }
}

void ppgGREEN_print(int row, int col, int color){
  size_t available_ppgGREEN = emotibit.readData(EmotiBit::DataType::PPG_GREEN, &data[0], dataSize);
  if (available_ppgGREEN > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_ppgGREEN; j++){
      for(i = 0; i < graphWidth; i++){
        ppgGREEN[i] = ppgGREEN[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_ppgGREEN && i < dataSize; i++){
      ppgGREEN[graphWidth - available_ppgGREEN + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_ppgGREEN[i] = normalize(ppgGREEN[i], findMin(ppgGREEN, graphWidth), findMax(ppgGREEN, graphWidth), graphMin, graphMax);
      // Serial.println(normal_ppgGREEN[i]);
    }
    printGraph(20+(65*row), col, normal_ppgGREEN, color);
  }
}

void EDA_print(int row, int col, int color){
  size_t available_EDA = emotibit.readData(EmotiBit::DataType::EDA, &data[0], dataSize);
  if (available_EDA > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_EDA; j++){
      for(i = 0; i < graphWidth; i++){
        EDA[i] = EDA[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_EDA && i < dataSize; i++){
      EDA[graphWidth - available_EDA + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_EDA[i] = normalize(EDA[i], findMin(EDA, graphWidth), findMax(EDA, graphWidth), graphMin, graphMax);
      // Serial.println(normal_EDA[i]);
    }
    printGraph(20+(65*row), col, normal_EDA, color);
  }
}

void EDL_print(int row, int col, int color){
  size_t available_EDL = emotibit.readData(EmotiBit::DataType::EDL, &data[0], dataSize);
  if (available_EDL > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_EDL; j++){
      for(i = 0; i < graphWidth; i++){
        EDL[i] = EDL[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_EDL && i < dataSize; i++){
      EDL[graphWidth - available_EDL + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_EDL[i] = normalize(EDL[i], findMin(EDL, graphWidth), findMax(EDL, graphWidth), graphMin, graphMax);
      // Serial.println(normal_EDL[i]);
    }
    printGraph(20+(65*row), col, normal_EDL, color);
  }
}

void EDR_print(int row, int col, int color){
  size_t available_EDR = emotibit.readData(EmotiBit::DataType::EDR, &data[0], dataSize);
  if (available_EDR > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_EDR; j++){
      for(i = 0; i < graphWidth; i++){
        EDR[i] = EDR[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_EDR && i < dataSize; i++){
      EDR[graphWidth - available_EDR + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_EDR[i] = normalize(EDR[i], findMin(EDR, graphWidth), findMax(EDR, graphWidth), graphMin, graphMax);
      // Serial.println(normal_EDR[i]);
    }
    printGraph(20+(65*row), col, normal_EDR, color);
  }
}

void temp0_print(int row, int col, int color){
  size_t available_temp0 = emotibit.readData(EmotiBit::DataType::TEMPERATURE_0, &data[0], dataSize);
  if (available_temp0 > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_temp0; j++){
      for(i = 0; i < graphWidth; i++){
        temp0[i] = temp0[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_temp0 && i < dataSize; i++){
      temp0[graphWidth - available_temp0 + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_temp0[i] = normalize(temp0[i], findMin(temp0, graphWidth), findMax(temp0, graphWidth), graphMin, graphMax);
      // Serial.println(normal_temp0[i]);
    }
    printGraph(20+(65*row), col, normal_temp0, color);
  }
}

void temp1_print(int row, int col, int color){
  size_t available_temp1 = emotibit.readData(EmotiBit::DataType::TEMPERATURE_1, &data[0], dataSize);
  if (available_temp1 > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_temp1; j++){
      for(i = 0; i < graphWidth; i++){
        temp1[i] = temp1[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_temp1 && i < dataSize; i++){
      temp1[graphWidth - available_temp1 + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_temp1[i] = normalize(temp1[i], findMin(temp1, graphWidth), findMax(temp1, graphWidth), graphMin, graphMax);
      // Serial.println(normal_temp1[i]);
    }
    printGraph(20+(65*row), col, normal_temp1, color);
  }
}

void thermopile_print(int row, int col, int color){
  size_t available_thermopile = emotibit.readData(EmotiBit::DataType::THERMOPILE, &data[0], dataSize);
  if (available_thermopile > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_thermopile; j++){
      for(i = 0; i < graphWidth; i++){
        thermopile[i] = thermopile[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_thermopile && i < dataSize; i++){
      thermopile[graphWidth - available_thermopile + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_thermopile[i] = normalize(thermopile[i], findMin(thermopile, graphWidth), findMax(thermopile, graphWidth), graphMin, graphMax);
      // Serial.println(normal_thermopile[i]);
    }
    printGraph(20+(65*row), col, normal_thermopile, color);
  }
}

void humid_print(int row, int col, int color){
  size_t available_humid = emotibit.readData(EmotiBit::DataType::HUMIDITY_0, &data[0], dataSize);
  if (available_humid > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_humid; j++){
      for(i = 0; i < graphWidth; i++){
        humid[i] = humid[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_humid && i < dataSize; i++){
      humid[graphWidth - available_humid + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_humid[i] = normalize(humid[i], findMin(humid, graphWidth), findMax(humid, graphWidth), graphMin, graphMax);
      // Serial.println(normal_humid[i]);
    }
    printGraph(20+(65*row), col, normal_humid, color);
  }
}

void accelX_print(int row, int col, int color){
  size_t available_accelX = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_X, &data[0], dataSize);
  if (available_accelX > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_accelX; j++){
      for(i = 0; i < graphWidth; i++){
        accelX[i] = accelX[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_accelX && i < dataSize; i++){
      accelX[graphWidth - available_accelX + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_accelX[i] = normalize(accelX[i], findMin(accelX, graphWidth), findMax(accelX, graphWidth), graphMin, graphMax);
      // Serial.println(normal_accelX[i]);
    }
    printGraph(20+(65*row), col, normal_accelX, color);
  }
}

void accelY_print(int row, int col, int color){
  size_t available_accelY = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_Y, &data[0], dataSize);
  if (available_accelY > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_accelY; j++){
      for(i = 0; i < graphWidth; i++){
        accelY[i] = accelY[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_accelY && i < dataSize; i++){
      accelY[graphWidth - available_accelY + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_accelY[i] = normalize(accelY[i], findMin(accelY, graphWidth), findMax(accelY, graphWidth), graphMin, graphMax);
      // Serial.println(normal_accelY[i]);
    }
    printGraph(20+(65*row), col, normal_accelY, color);
  }
}

void accelZ_print(int row, int col, int color){
  size_t available_accelZ = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_Z, &data[0], dataSize);
  if (available_accelZ > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_accelZ; j++){
      for(i = 0; i < graphWidth; i++){
        accelZ[i] = accelZ[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_accelZ && i < dataSize; i++){
      accelZ[graphWidth - available_accelZ + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_accelZ[i] = normalize(accelZ[i], findMin(accelZ, graphWidth), findMax(accelZ, graphWidth), graphMin, graphMax);
      // Serial.println(normal_accelZ[i]);
    }
    printGraph(20+(65*row), col, normal_accelZ, color);
  }
}

void gyroX_print(int row, int col, int color){
  size_t available_gyroX = emotibit.readData(EmotiBit::DataType::GYROSCOPE_X, &data[0], dataSize);
  if (available_gyroX > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_gyroX; j++){
      for(i = 0; i < graphWidth; i++){
        gyroX[i] = gyroX[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_gyroX && i < dataSize; i++){
      gyroX[graphWidth - available_gyroX + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_gyroX[i] = normalize(gyroX[i], findMin(gyroX, graphWidth), findMax(gyroX, graphWidth), graphMin, graphMax);
      // Serial.println(normal_gyroX[i]);
    }
    printGraph(20+(65*row), col, normal_gyroX, color);
  }
}

void gyroY_print(int row, int col, int color){
  size_t available_gyroY = emotibit.readData(EmotiBit::DataType::GYROSCOPE_Y, &data[0], dataSize);
  if (available_gyroY > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_gyroY; j++){
      for(i = 0; i < graphWidth; i++){
        gyroY[i] = gyroY[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_gyroY && i < dataSize; i++){
      gyroY[graphWidth - available_gyroY + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_gyroY[i] = normalize(gyroY[i], findMin(gyroY, graphWidth), findMax(gyroY, graphWidth), graphMin, graphMax);
      // Serial.println(normal_gyroY[i]);
    }
    printGraph(20+(65*row), col, normal_gyroY, color);
  }
}

void gyroZ_print(int row, int col, int color){
  size_t available_gyroZ = emotibit.readData(EmotiBit::DataType::GYROSCOPE_Z, &data[0], dataSize);
  if (available_gyroZ > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_gyroZ; j++){
      for(i = 0; i < graphWidth; i++){
        gyroZ[i] = gyroZ[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_gyroZ && i < dataSize; i++){
      gyroZ[graphWidth - available_gyroZ + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_gyroZ[i] = normalize(gyroZ[i], findMin(gyroZ, graphWidth), findMax(gyroZ, graphWidth), graphMin, graphMax);
      // Serial.println(normal_gyroZ[i]);
    }
    printGraph(20+(65*row), col, normal_gyroZ, color);
  }
}

void magnetX_print(int row, int col, int color){
  size_t available_magnetX = emotibit.readData(EmotiBit::DataType::MAGNETOMETER_X, &data[0], dataSize);
  if (available_magnetX > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_magnetX; j++){
      for(i = 0; i < graphWidth; i++){
        magnetX[i] = magnetX[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_magnetX && i < dataSize; i++){
      magnetX[graphWidth - available_magnetX + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_magnetX[i] = normalize(magnetX[i], findMin(magnetX, graphWidth), findMax(magnetX, graphWidth), graphMin, graphMax);
      // Serial.println(normal_magnetX[i]);
    }
    printGraph(20+(65*row), col, normal_magnetX, color);
  }
}

void magnetY_print(int row, int col, int color){
  size_t available_magnetY = emotibit.readData(EmotiBit::DataType::MAGNETOMETER_Y, &data[0], dataSize);
  if (available_magnetY > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_magnetY; j++){
      for(i = 0; i < graphWidth; i++){
        magnetY[i] = magnetY[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_magnetY && i < dataSize; i++){
      magnetY[graphWidth - available_magnetY + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_magnetY[i] = normalize(magnetY[i], findMin(magnetY, graphWidth), findMax(magnetY, graphWidth), graphMin, graphMax);
      // Serial.println(normal_magnetY[i]);
    }
    printGraph(20+(65*row), col, normal_magnetY, color);
  }
}

void magnetZ_print(int row, int col, int color){
  size_t available_magnetZ = emotibit.readData(EmotiBit::DataType::MAGNETOMETER_Z, &data[0], dataSize);
  if (available_magnetZ > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_magnetZ; j++){
      for(i = 0; i < graphWidth; i++){
        magnetZ[i] = magnetZ[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_magnetZ && i < dataSize; i++){
      magnetZ[graphWidth - available_magnetZ + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_magnetZ[i] = normalize(magnetZ[i], findMin(magnetZ, graphWidth), findMax(magnetZ, graphWidth), graphMin, graphMax);
      // Serial.println(normal_magnetZ[i]);
    }
    printGraph(20+(65*row), col, normal_magnetZ, color);
  }
}

void battery_print(int row, int col, int color){
  size_t available_batt = emotibit.readData(EmotiBit::DataType::BATTERY_VOLTAGE, &data[0], dataSize);
  if (available_batt > 0){
		// Hey cool, I got some data! Maybe I can light up my shoes whenever I get excited!
    for(j = 0; j < available_batt; j++){
      for(i = 0; i < graphWidth; i++){
        batt[i] = batt[i + 1];
      }
    }
    
    for (size_t i = 0; i < available_batt && i < dataSize; i++){
      batt[graphWidth - available_batt + i] = data[i];
		}

    for(i = 0; i < graphWidth; i++){
      normal_batt[i] = normalize(batt[i], findMin(batt, graphWidth), findMax(batt, graphWidth), graphMin, graphMax);
      // Serial.println(normal_batt[i]);
    }
    printGraph(20+(65*row), col, normal_batt, color);
  }
}

float findMin(float *array, int size){
  float min = array[size - 1];
  int i;
  for(i = size - 1; 0 <= i; i--){
    if(array[i] < min){
      min = array[i];
    }
  }
  return min;
}

float findMax(float *array, int size){
  float max = array[size - 1];
  int i;
    for(i = size - 1; 0 <= i; i--){
        if(array[i] > max){
          max = array[i];
        }
    }
  return max;
}

// Function to find all the local maxima
// and minima in the given array arr[]
void findLocalMaximaMinima(int n, float* arr){
    float min = arr[0], max = arr[0];
    int maxFound = 1, minFound = 0;
    indMin = 0;
    indMax = 0;
    for(i = 1; i < n - 1; i++){
        //Check for local minima
        if((arr[i - 1] > arr[i]) && (arr[i] < arr[i + 1])){
            minList[indMin] = i;
            indMin++;
        //Check for local maxima
        }else if((arr[i - 1] < arr[i])&&(arr[i] > arr[i + 1])){
            maxList[indMax] = i;
            indMax++;
        }
    }
}

float calculateBPM(int count){
  float toggle = 1, pkCount = 0, bpm = 0;
  int deltaI, lastI;
  //calculateBPM from normal_ppgIR array (about 10 seconds of data)
  // findLocalMaximaMinima(graphWidth, normal_ppgIR);
  // for(i = 0; i < graphWidth; i++){
  //   for(j = 0; j < indMax; j++){
  //     if(normal_ppgIR[i] == normal_ppgIR[maxList[j]]){
  //       if(toggle == 1){
  //         pkCount = pkCount + 1;
  //         toggle = 0;
  //       }else{
  //         toggle = 1;
  //       }
  //     }
  //   }
  // }

  //calculateBPM from hist array (about 60 seconds of data)
  findLocalMaximaMinima(histLength, hist);
  for(i = 0; i < histLength; i++){
    for(j = 0; j < indMax; j++){
      if(hist[i] == hist[maxList[j]]){
        deltaI = i - lastI;
        if(toggle == 1){
          pkCount = pkCount + 1;
          toggle = 0;
        }else{
          toggle = 1;
        }
        lastI = i;
      }
    }
  }


  // if(graphWidth == 240){
  //   //pkCount calculates beat count per 10 seconds
  //   bpm = pkCount * 6; //Calculate beats per 60 seconds    
  // }else if(graphWidth == 120){
  //   //pkCount calculates beat count per 5 seconds
  //   bpm = pkCount * 12; //Calculate beats per 60 seconds
  // }

  if(histLength == HISTORY_LENGTH){
    bpm = pkCount * 4;
  }
  return bpm;
}

int rollingAvg(int roll){
  int sum = 0, avg = 0;
  // bpmHist[rollCount] = calculateBPM();
  if((60/(static_cast<float>(peakDelta)/1000)) < 400){
    bpmHist[rollCount] = 60/(static_cast<float>(peakDelta)/1000);
  }else{
    bpmHist[rollCount] = bpmHist[rollCount - 1];
  }
  if(rollCount >= roll){
    for(i = 0; i < rollCount; i++){
      sum += bpmHist[i];
      bpmHist[i] = 0;
    }
    avg = sum / roll;
    rollCount = 0;
    tft.fillRect(2, 220, 84, 14, bgColor);
    tft.setCursor(2, 220);
    tft.setTextSize(2);
    tft.setTextColor(textColor, bgColor);
    if(avg < 130){
      sprintf(hrStr, "%c %d", 3, avg);
      tft.println(hrStr);
    }else{
      sprintf(hrStr, "%c High", 3);
      tft.println(hrStr);
    }
    sprintf(hrStr, "%c %d", 3, avg);
    tft.println(hrStr);
    return 1;
  }else{
    rollCount++;
    return 0;
  }
}

float normalize(float measurement, float measureMin, float measureMax, float outputMin, float outputMax){
  float calc;
  calc = (((measurement - measureMin) / (measureMax - measureMin)) * (outputMax - outputMin)) + outputMin;
  return calc; 
}

void printGrid(void){
  //240x280 TFT LCD
  uint16_t x,y;
  for(y = 0; y < Y_LIM; y+= 5){
    tft.drawFastHLine(0, y, 240, ST77XX_WHITE);
  }
  for(x = 0; x < X_LIM; x+= 5){
    tft.drawFastVLine(x, 0, 280, ST77XX_WHITE);
  }
}

//Direction 0 - 7
void printArrow(int x, int y, int direction, uint16_t color, uint16_t bg){
  // tft.setCursor(2, 220);
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.setTextColor(color, bg);
  switch(direction){
    case 0://Print Up
      sprintf(roseStr, "%c", 24);
      tft.println(roseStr);
      break;
    case 1://Print Up-Right
      tft.fillRect(x, y, 12, 16, bg);

      tft.drawLine(x+2, y+2, x+9, y+2, color);  //Top side of arrow
      tft.drawLine(x+2, y+3, x+9, y+3, color);

      tft.fillRect(x+6, y+4, 2, 2, color);  //Mid-line of arrow
      tft.fillRect(x+4, y+6, 2, 2, color);
      tft.fillRect(x+2, y+8, 2, 2, color);
      tft.fillRect(x+0, y+10, 2, 2, color);

      tft.drawLine(x+8, y+2, x+8, y+9, color);  //Right side of arrow
      tft.drawLine(x+9, y+2, x+9, y+9, color);
      break;
    case 2://Print Right
      sprintf(roseStr, "%c", 26);
      tft.println(roseStr);
      break;
    case 3://Print Right-Down
      tft.fillRect(x, y, 12, 16, bg);

      tft.drawLine(x+2, y+12, x+9, y+12, color);  //Bot side of arrow
      tft.drawLine(x+2, y+13, x+9, y+13, color);

      tft.fillRect(x+6, y+10, 2, 2, color);  //Mid-line of arrow
      tft.fillRect(x+4, y+8, 2, 2, color);
      tft.fillRect(x+2, y+6, 2, 2, color);
      tft.fillRect(x+0, y+4, 2, 2, color);

      tft.drawLine(x+8, y+6, x+8, y+13, color);  //Right side of arrow
      tft.drawLine(x+9, y+6, x+9, y+13, color);
      break;
    case 4://Print Down
      sprintf(roseStr, "%c", 25);
      tft.println(roseStr);
      break;
    case 5://Print Down-Left
      tft.fillRect(x, y, 12, 16, bg);

      tft.drawLine(x, y+12, x+7, y+12, color);  //Bot side of arrow
      tft.drawLine(x, y+13, x+7, y+13, color);

      tft.fillRect(x+8, y+4, 2, 2, color);  //Mid-line of arrow
      tft.fillRect(x+6, y+6, 2, 2, color);
      tft.fillRect(x+4, y+8, 2, 2, color);
      tft.fillRect(x+2, y+10, 2, 2, color);

      tft.drawLine(x+0, y+6, x+0, y+13, color);  //Right side of arrow
      tft.drawLine(x+1, y+6, x+1, y+13, color);
      break;
    case 6://Print Left
      sprintf(roseStr, "%c", 27);
      tft.println(roseStr);
      break;
    case 7://Print Up-Left
      tft.fillRect(x, y, 12, 16, bg);

      tft.drawLine(x, y+2, x+7, y+2, color);  //Top side of arrow
      tft.drawLine(x, y+3, x+7, y+3, color);

      tft.fillRect(x+2, y+4, 2, 2, color);  //Mid-line of arrow
      tft.fillRect(x+4, y+6, 2, 2, color);
      tft.fillRect(x+6, y+8, 2, 2, color);
      tft.fillRect(x+8, y+10, 2, 2, color);

      tft.drawLine(x, y+2, x, y+9, color);  //Right side of arrow
      tft.drawLine(x+1, y+2, x+1, y+9, color);
      break;      
  }
  tft.drawPixel(x, y, ST77XX_MAGENTA);
}

void printGraph(int row, int col, float* variable, int color){
  if(col == 1){
    for(i = 0; i < graphWidth; i++){
      tft.drawFastVLine(i, row - 1, -63, bgColor);
      tft.drawPixel(i, row - 1 - variable[i], color);
    }
  }else if(col == 2){
    for(i = 0; i < 121 + graphWidth; i++){
      tft.drawFastVLine(121 + i, row - 1, -63, bgColor);
      tft.drawPixel(121 + i, row - 1 - variable[i], color);
    }
  }
}