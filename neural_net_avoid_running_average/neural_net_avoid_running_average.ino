// Based on AI_delta_float_v1.2
//14.04.20 by Tim Kälin
 
//Changed, updated for use with BPI-Bit and Adapt StarryBit robot
//LeRoy Miller, May 15, 2022

/*
 * Change Log: 
 * May 27, 2022 - Added Running Average
 *   -May 28, 2022 - minor change to how cells are loaded from sensor function
 */
 
//More Info: https://www.the-diy-life.com/running-an-artificial-neural-network-on-an-arduino-uno/
//https://www.youtube.com/watch?v=SL0z8HkPOmg

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/FastLED/FastLED.h"
#include "src/RunningAverage/RunningAverage.h"

FASTLED_USING_NAMESPACE

// ultrasonic SR04 sensor pins
#define sensor_A_out P20 
#define sensor_A_in  P19 
#define sensor_B_out P12 
#define sensor_B_in  P11 


#define LED_PIN      4  // neopixel pin
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    25
CRGB leds[NUM_LEDS];

//#define motA_E 2
#define motA_1 19
#define motA_2 5  //pwm
//#define motB_E 15
#define motB_1 18 
#define motB_2 23  //pwm


RunningAverage result1(10);
RunningAverage result2(10);

int modus = 0;  // 0 = training until double success then drive   1 = drive  2 = test motors with serial, input one of the outputs 0-5

// setting PWM properties motor
const int freq = 8000;
const int Channel_0 = 0;
const int Channel_1 = 1;
//const int Channel_2 = 2;
//const int Channel_3 = 3;
const int resolution = 8;   //Resolution 8, 10, 12, 15
int battery = 20;   //calibration for battery voltage/speed. 

const double InitWeights = 0.44;  
double epsilon = 0.0076;  // learning rate 
double mom = 0.024;  
double success = 0.4;   //training loops until failure rate is lower than this
double bias = 1;

int patern = 25;    // number of test paterns in test_in and test_out
int count = 0;     // counter for print serial and view leds in training session
int count_to = 50; //2000;    //cycles to count for serial out

// cells to input from sensor data, done in "sensor"
int input_A_toCell = 0;  
int input_B_toCell = 1;

int s,p = 0;  //  global counters
int out = 0;  //  varable for calculated output

double Error, Err_sum;
double Accum;
double winrate = 0;

const int cell_raster_in = 2 ;    //  inputs
const int cell_raster_out  = 5 ;  //  outputs maybe +1 bias

const int cell_raster_hidden  = cell_raster_out;

double Delta_hidden[cell_raster_hidden];
double Delta_out[cell_raster_hidden];
double cells_in_hidden_weight[cell_raster_in+1] [cell_raster_hidden]  = {   };
double cells_hidden_out_weight[cell_raster_hidden+1][cell_raster_out] = {   };
//double cells_hidden_out_weight[cell_raster_out][cell_raster_hidden] = {   };
double cells_in_hidden_weight_change[cell_raster_in+1] [cell_raster_hidden]  = {   };
double cells_hidden_out_weight_change[cell_raster_hidden+1][cell_raster_out] = {   }; 
//double cells_hidden_out_weight_change[cell_raster_out][cell_raster_hidden] = {   };             
double cells_in[cell_raster_in] = {   }; 
double cells_hidden[cell_raster_hidden] = {   };                
double cells_out[cell_raster_out]   = {   };

// training data sensor in cm, test_in[patern]
double test_in[25][cell_raster_in] = {
  25,14,   //1
  41,33,   //2
  44,44,   //3
  33,41,   //4
  14,25,   //5
  29,22,   //6
  43,33,   //7
  80,90,   //8
  33,43,   //9
  22,29,   //10
  35,26,   //11
  55,35,   //12
  55,55,   //13
  35,55,   //14
  26,35,   //15  
  33,25,   //16
  44,32,   //17
  150,150, //18
  32,44,   //19
  25,33,   //20 
  38,23,   //21
  50,36,   //22
  90,100,  //23
  36,50,   //24
  23,38    //25
};

//  training data supposed output  << < | > >>   ,  test_out[patern]
double test_out[25][cell_raster_out] = {
  1,0,0,0,0,  //1
  0,1,0,0,0,  //2
  0,0,1,0,0,  //3
  0,0,0,1,0,  //4
  0,0,0,0,1,  //5  
  1,0,0,0,0,  //6
  0,1,0,0,0,  //7
  0,0,1,0,0,  //8
  0,0,0,1,0,  //9
  0,0,0,0,1,  //10
  1,0,0,0,0,  //11
  0,1,0,0,0,  //12
  0,0,1,0,0,  //13  
  0,0,0,1,0,  //14 
  0,0,0,0,1,  //15
  1,0,0,0,0,  //16  
  0,1,0,0,0,  //17
  0,0,1,0,0,  //18
  0,0,0,1,0,  //19
  0,0,0,0,1,  //20   
  1,0,0,0,0,  //21
  0,1,0,0,0,  //22 
  0,0,1,0,0,  //23
  0,0,0,1,0,  //24 
  0,0,0,0,1   //25             
};

int incomingByte = 0;

//Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("Start");   

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); //HIGH Turns 5x5 matrix on, LOW turns it off
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<WS2812,23,COLOR_ORDER>(leds, 4);
  
  rst();   // reset all weights to 0, maybe not necessary
  seed(); 
  
  delay(2);
  pinMode(sensor_A_out, OUTPUT);
  pinMode(sensor_B_out, OUTPUT);
  pinMode(sensor_A_in, INPUT);
  pinMode(sensor_B_in, INPUT);
  //pinMode(motA_E, OUTPUT);
  pinMode(motA_1, OUTPUT);  
  pinMode(motA_2, OUTPUT);
  //pinMode(motB_E, OUTPUT);  
  pinMode(motB_1, OUTPUT);
  pinMode(motB_2, OUTPUT);
  ledcSetup(Channel_0, freq, resolution);
  ledcAttachPin(motA_2, Channel_0);
  ledcSetup(Channel_1, freq, resolution);
  ledcAttachPin(motB_2, Channel_1);
  stop();
}

void loop() {  
  if(modus == 0)     {  training();  } 
  else if(modus == 1){  sensor(); load(); outvalue(); /*set_leds();*/ serial_print_out();  drive(); }
  else if(modus == 2){  for(out = 0; out < 5; out++) {serial_in(); leds_out(); drive(); delay(2000);  }
  }
}


//backup working testpatern
/*double test_in[25][cell_raster_in] = {
  24,16,  //1
  50,32,  //2
  44,44,  //3
  32,50,  //4
  16,24,  //5
  40,21,  //6
  50,36,  //7
  100,120,  //8
  36,50, //9
  21,40,  //10
  30,24,  //11
  60,34,  //12
  50,60,  // 13
  34,60,  //14
  24,30,  //15  
  33,25,  //16
  44,35,  //17
  60,50,  //18
  35,44,  //19
  25,33,  //20 
  38,23,  //21
  45,36,  //22
  100,120,  //23
  36,45,  //24
  23,38   //25
};
double test_out[25][cell_raster_out] = {
//<< < | > >> !  
  1,0,0,0,0,  //1
  0,1,0,0,0,  //2
  0,0,1,0,0, //3
  0,0,0,1,0,  //4
  0,0,0,0,1,  //5  
  1,0,0,0,0,  //6
  0,1,0,0,0,  //7
  0,0,1,0,0,  //8
  0,0,0,1,0,  //9
  0,0,0,0,1,  //10
  1,0,0,0,0,  //11
  0,1,0,0,0,  //12
  0,0,1,0,0,  //13  
  0,0,0,1,0,  //14 
  0,0,0,0,1,  //15
  1,0,0,0,0,  //16  
  0,1,0,0,0,  //17
  0,0,1,0,0,  //18
  0,0,0,1,0,  //19
  0,0,0,0,1,  //20   
  1,0,0,0,0,  //21
  0,1,0,0,0,  //22 
  0,0,1,0,0,  //23
  0,0,0,1,0,   //24 
  0,0,0,0,1   //25             
};
*/

void biasrandom()
{
  randomSeed(analogRead(3)); //srand(time(0)); 
  double upper = 900;
  double lower = 0;     
  double tmp = double(random(100))/100;
  bias = 2.0 * (tmp - 0.5 ) * InitWeights;   
}

void learning()  // train the network
{  
  double sum = 0;

  for(int x = 0 ; x < cell_raster_hidden ; x++ ) 
  {    
    Accum = 0.0 ;
    for(int y = 0 ; y < cell_raster_out ; y++ )
    {
      Accum += cells_hidden_out_weight[x][y] * Delta_out[y] ;
    }
  Delta_hidden[x] = Accum * cells_hidden[x] * (1.0 - cells_hidden[x]) ;
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {    
    cells_in_hidden_weight_change[cell_raster_in][x] =  epsilon * Delta_hidden[x] + mom * cells_in_hidden_weight_change[cell_raster_in][x] ;
    cells_in_hidden_weight[cell_raster_in][x] += cells_in_hidden_weight_change[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {       
      cells_in_hidden_weight_change[y][x] =  epsilon * test_in[p][y] * Delta_hidden[x] + mom * cells_in_hidden_weight_change[y][x] ;
      cells_in_hidden_weight[y][x] +=  cells_in_hidden_weight_change[y][x];    
    }
  }
  
  for(int x = 0; x < cell_raster_out; x++)
  {
    cells_hidden_out_weight_change[cell_raster_hidden][x] = epsilon *  Delta_out[x] + mom * cells_hidden_out_weight_change[cell_raster_hidden][x] ;
    cells_hidden_out_weight[cell_raster_hidden][x] += cells_hidden_out_weight_change[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {     
      cells_hidden_out_weight_change[y][x] =  epsilon * cells_hidden[y] * Delta_out[x]  + mom * cells_hidden_out_weight_change[y][x] ;
      cells_hidden_out_weight[y][x] += cells_hidden_out_weight_change[y][x];
    }
  } 
}

void load()  // load data through the network
{
  //  biasrandom();  // test with a random bias
  double sum = 1.0; 
  int c = 0;
  Error = 0.0;
  for(int x = 0; x < cell_raster_hidden; x++)  // input        
  {  
    sum = cells_in_hidden_weight[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {
      sum += cells_in[y] * cells_in_hidden_weight[y][x];         
    }   
    cells_hidden[x] = 1.0/(1.0 + exp(-sum));   //?
  }  
  for(int x = 0; x < cell_raster_out; x++)  // hidden  
  {
    sum = cells_hidden_out_weight[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {
      sum += cells_hidden[y] * cells_hidden_out_weight[y][x];          
    }   
    cells_out[x] =  1.0/(1.0 + exp(-sum));
    Delta_out[x] =  (test_out[p][x] - cells_out[x]) * cells_out[x] * (1.0 -cells_out[x]);  
    Error += 0.5 * (test_out[p][x] - cells_out[x]) * (test_out[p][x] - cells_out[x]);
  }
}

void rst()  
{
  for(int c = 0; c < cell_raster_in; c++)
  {  
    cells_in[c] = 0;  
  }
  for(int c = 0; c < cell_raster_hidden; c++)
  {   
    cells_hidden[c] = 0;    
  } 
  for(int c = 0; c < cell_raster_out; c++)
  {  
    cells_out[c] = 0;
  }   
}

void outvalue()  // get the output with the highest value
{
  double o = cells_out[0]; 
  out = 0; 
  for(int i = 0; i < cell_raster_out; i++)
  {    
    if(cells_out[i] >= o)
    {
      o = cells_out[i];
      out = i; 
    }  
  } 
}

void seed() // random seed the weights
{  
  double upper = 1000;
  double lower = 50;
  for(int x = 0; x < cell_raster_in; x++)
  {
  for (int sy = 0 ; sy < cell_raster_hidden; sy++)
    { randomSeed(analogRead(3)); //srand(time(0));       
      double tmp = double(random(100))/100;
      cells_in_hidden_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_in_hidden_weight_change [x][sy] = 0.0;
    }
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {
  for (int sy = 0 ; sy < cell_raster_out; sy++)
    { randomSeed(analogRead(3));        
      double tmp = double(random(100))/100;
      cells_hidden_out_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_hidden_out_weight_change[x][sy] = 0.0;
    }
  }       
}
