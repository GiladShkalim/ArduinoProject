   //      Gilad Shkalim     //
   
#include <FastLED.h>

#define BoostR 10     //Enable (PWM) pin for first motor  
#define R1 6          //control pin for Right motor
#define R2 7
#define L1 9          //control pin for Left motor
#define L2 8
#define BoostL 11
#define LED_PIN     5 //Matrix setup
#define NUM_LEDS    64
#define L_POWER 30

//Line follow
int speed = 100;
int L_S = A0;         //Right Sensor
int R_S = A1;         //Left Sensor
int L_SCHECK;
int R_SCHECK;
const int err = 150;

//Blutooth 
char bt = 0;       

//millis()
int interval = 1000;
int strobo_1 = 70;
unsigned long previousMillis = 0; 
unsigned long previousMillisM1rwb;
unsigned long previousMillisM1rb;
boolean isPatern = true;
CRGB leds[NUM_LEDS];


void setup(){
  Serial.begin(9600);
  //Matrix
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  //Tints
  pinMode(BoostR, OUTPUT);
  pinMode(BoostL, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  //IR
  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);
  // Disable both motors
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  
  //counter
  previousMillis = millis();
  delay(1000);
 }

void loop(){ 
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
    Serial.println("-------IN matrix");
    Matrix();
}

if (Serial.available() > 0){          //save incoming data to variable BT{
    Blutooth();
   }
   else{
    FollowLine();
   }
}

//Functions
void FollowLine(){

analogWrite(BoostR, speed);
analogWrite(BoostL, speed);

if ((L_SCHECK < err)&&(R_SCHECK < err)){    
  Serial.println("Forword"); 
  forword();
  }

if ((L_SCHECK < err)&&(R_SCHECK >=err)){
  Serial.println("TurnLeft");
  turnLeft();
  }

if ((L_SCHECK >=err)&&(R_SCHECK < err)){
  Serial.println("TurnRight");
  turnRight();
  }

if ((L_SCHECK >=err)&&(R_SCHECK >=err)){
  Serial.println("Stop");
  stop();
  }
}


//BT
void Blutooth(){
    bt = Serial.read();
    Serial.print("BT  --  ");
    Serial.println(bt);
    turnforBT(bt);
}

void turnforBT(char bt){
  if(bt == 'F')           //move forwards
     {
  digitalWrite(R1, LOW);                        // Set motor A (Right) forward
  digitalWrite(R2, HIGH);
  digitalWrite(L1, LOW);                        // Set motor B (Left)forward
  digitalWrite(L2, HIGH);
     }
     else if (bt == 'B')      //move backwards
     {
  digitalWrite(R1, HIGH);                       // Set motor A (Right) Backwards
  digitalWrite(R2, LOW);  
  digitalWrite(L1, HIGH);                        // Set motor B (Left)Backwards
  digitalWrite(L2, LOW);
     }
     else if (bt == 'S')     //stop!!
     {   
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
     }
     else if (bt == 'R')    // turn wheels right
     {
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);  // Set motor A (Right) forward
  digitalWrite(R2, HIGH);
     }
     else if (bt == 'L')     //turn wheels left
     {
  digitalWrite(L1, LOW); // Set motor B (Left)forward
  digitalWrite(L2, HIGH);
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW); 
     }
}

//Folow line Mobility
void stop(){
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
}

void forword(){
  digitalWrite(R1, LOW);                        // Set motor A (Right) forward
  digitalWrite(R2, HIGH);
  digitalWrite(L1, LOW);                        // Set motor B (Left)forward
  digitalWrite(L2, HIGH);
}

//void backword(){
//  digitalWrite(L1, LOW);                        // Set motor B (Left)Backwards
//  digitalWrite(L2, HIGH);
//  digitalWrite(R1, HIGH);                       // Set motor A (Right) Backwards
//  digitalWrite(R2, LOW);  
//}

void turnRight(){
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);  // Set motor A (Right) forward
  digitalWrite(R2, HIGH);
}

void turnLeft(){
  digitalWrite(L1, LOW); // Set motor B (Left)forward
  digitalWrite(L2, HIGH);
  digitalWrite(R1, LOW); 
  digitalWrite(R2, LOW); 
}

//Matrix
void Matrix(){
  
  int patern = 1;
  int restartMat = 0;
  int strobo_1 = 30;
  int strobo_2 = 80;
  unsigned long previousMillisM1;
  unsigned long previousMillisM2;
  unsigned long previousMillisM3;
  unsigned long previousMillisM4;

////--------------------------------------- STEP 1
if(patern == 1){
unsigned long currentMillisM1 = millis();
if (millis() - previousMillisM1 >= (50*strobo_1)) {
  mix_rwb();
  previousMillisM1 = millis();
}
patern = 2;
}
//-------------------------------------- STEP 2
if(patern == 2){
if (millis() - previousMillisM2 >= (200*strobo_1)) {
  mix_rb();
  previousMillisM2 = millis();
}
patern = 1;
}
}

// Light Control
void mix_rwb()  {
for (int i = 0; i <= 6; i++) {
    for(int n = 0; n <= 23;n++){  //red
      leds[n]=CRGB (L_POWER,0,0);
    }
    for(int n = 24; n <= 39;n++){ //bianco
      leds[n]=CRGB (L_POWER,L_POWER,L_POWER);
    }
    for(int n = 40; n <= 64;n++){ //blu
      leds[n]=CRGB (0,0,L_POWER);
    }
    unsigned long currentMillisM1rwb = millis();
    if (millis() - previousMillisM1rwb >= (70*strobo_1)) {
      FastLED.show();
    }
    if (millis() - previousMillisM1rwb >= (80*strobo_1)) {
      FastLED.clear ();
    }
}
}

void mix_rb()  {
for (int i = 0; i <= 3; i++) {
    for(int n = 0; n <= 23;n++){
      leds[n]=CRGB (L_POWER,0,0);
    }  
    unsigned long currentMillisM1rb = millis();
    if (millis() - previousMillisM1rb >= (10*strobo_1)) {
      FastLED.show();
    }
    if (millis() - previousMillisM1rb >= (20*strobo_1)) {
      FastLED.clear ();
    }
    if (millis() - previousMillisM1rb >= (30*strobo_1)) {
      FastLED.show();
    }
    if (millis() - previousMillisM1rb >= (40*strobo_1)) {
      FastLED.clear ();
    }

   for(int n = 40; n <= 64;n++){
      leds[n]=CRGB (0,0,L_POWER);
    } 
   if (millis() - previousMillisM1rb >= (5*strobo_1)) {
      FastLED.show();
   }
   if (millis() - previousMillisM1rb >= (10*strobo_1)) {
      FastLED.clear ();
   }
   if (millis() - previousMillisM1rb >= (20*strobo_1)) {
      FastLED.show();
   }
   if (millis() - previousMillisM1rb >= (30*strobo_1)) {
      FastLED.clear ();
      previousMillisM1rb = millis();
   }
  }
}
