#include <LedControl.h>                 // to drive the matrix
#include <NewTone.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define red 0                           // The address of the MAX7221 for the red leds
#define green 1                         // The address of the MAX7221 for the green leds
#define orange 2 //not an address - just a reference

#define ISR_FREQ 190     //190=650Hz    // Sets the speed of the ISR - LOWER IS FASTER 
// prescaler is /128 -  125,000/ISR_FREQ+1 (i.e 249=500Hz, 190=650Hz) 
// Tweaked depending on the overhead in the ISR & and other factors in the sketch
// Display will be slow if too FAST. Sketch will hang on delay() if way too fast!

#define potxp 0 //x axis potentiometer input
#define potyp 1 //y axis potentiometer input
#define potthreshold 300

#define buttp 7 //joystick button pin (just an easter egg)

#define tonep 5 //tone() output

#define drawp 1

int maxInShutdown = green;                // tells which MAX7221 is currently off 

int ipotxv, ipotyv;
int cx = 4, px;
int cy = 7, py;

int buttc = red; //player color

int dropsx[8];
double dropp = 200, movepe = 60;
int dropsi = 0;
int droppt = 0, movepet = 0;

LedControl lc = LedControl(2,3,4,2); // pins 10=DataIn, 9=CLK, 8=LOAD + 2 MAX7221s 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
boolean play = true;

int score = 0;
int highscore = 0;

void setup(){
  ipotxv = analogRead(potxp);
  ipotyv = analogRead(potyp);
  
  lc.setIntensity(red,15);              // red needs less brightness
  lc.setIntensity(green,15);            // 0 = dim, 15 = full brightness
  
  lcd.begin(16,2);
  lcd.clear();
  //Serial.begin(9600);
  
  randomSeed(analogRead(6));
  
  pinMode(tonep, OUTPUT);
  
  lcd.setCursor(0,0); lcd.print("evade :");
  lcd.setCursor(0,1); lcd.print(" an arduino game");
  delay(1500);
  
  setISRtimer();                        // setup the timer
  startISR();                           // start the timer to toggle shutdown
}

void loop(){
  reset();
  while(play){
    droppt += drawp;
    movepet += drawp;
    if(movepet >= movepe){
      movep();
      movepet = 0;
      lcd.setCursor(7,0);
      lcd.print(score);
    }         
    if(droppt >= dropp){
      movedrops();
      droppt = 0;
    }
    if(random(3) == random(777)){
      randomSeed(analogRead(6));
      dropp /= 1+(log(dropp+random(100))/100);
      movepe = dropp/4+10;
    }
    delay(drawp);
  }
}

void reset(){
  startt();
  
  for(int i = 0; i < dropsi; ++i) dropsx[i] = 0;
  dropsi = droppt = movepet = 0;
  cx = px = 4;
  cy = py = 7;
  dropp = 200;
  clearall();
  lcd.clear();
  
  if(score > highscore) highscore = score;
  score = 0;
  
  lcd.setCursor(0,0);
  lcd.print("highscore: ");
  lcd.setCursor(2,1);
  lcd.print(highscore);
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("score: 0");
}

void boom(){
  play = false;
  collisiont();
  collisiona(cx, cy);
  for(int i = dropsi-1; i >= 0; --i){
    setboth(dropsx[i], i, true);
    delay(50);
  }
  delay(1000);
  reset();
  delay(500);
  play = true;
}

void startt(){
  NewTone(tonep, 261, 70);
  delay(100);
  NewTone(tonep, 261, 100);
}
void collisiont(){
  if(random(100) == random(100)){
    NewTone(tonep, 130, 70);
    delay(100);
  }
  NewTone(tonep, 116, 100);
}
void movet(){
  NewTone(tonep, 165, 100);
}

void collisiona(int ix, int iy){
  for(int i = 0; i < 3; ++i){
    setone(red,ix+i,iy,true);
    setone(red,ix-i,iy,true);
    setone(red,ix,iy+i,true);
    setone(red,ix,iy-i,true);
    setone(red,ix+i,iy+i,true);
    setone(red,ix-i,iy+i,true);
    setone(red,ix+i,iy-i,true);
    setone(red,ix-i,iy-i,true);
    delay(40);
  }
}

void movep(){
  px = cx; py = cy;
  
  int potxv = analogRead(potxp);
  if(potxv > ipotxv+potthreshold){
    if(cx < 7) cx += 1;
  } else if(potxv < ipotxv-potthreshold){
    if(cx > 0) cx -= 1;
  }
  
  int potyv = analogRead(potyp);
  if(potyv > ipotyv+potthreshold){
    if(cy < 7) cy += 1;
  } else if(potyv < ipotyv-potthreshold){
    if(cy > 0) cy -= 1;
  }
  
  setone(red, px, py, false);
  setone(red, cx, cy, true);
  
  for(int i = 0; i < dropsi; ++i){
    if(i == cy && dropsx[i] == cx){
      boom();
      return;
    }
  }
}

void movedrops(){
  for(int i = dropsi; i >= 0; --i){
    setone(green, dropsx[i], i, false);
    dropsx[i+1] = dropsx[i];
  }
  dropsx[0] = random(0,8);
  if(dropsi < 8) dropsi++;
  else score++;
  for(int i = 0; i < dropsi; ++i) setone(green, dropsx[i], i, true);
}

