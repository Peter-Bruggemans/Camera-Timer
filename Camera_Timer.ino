
    /*
    Camera Timer
    For Arduino with adjustable duration and interval
    22-11-2019
    Peter Bruggemans
    */

#include <LiquidCrystal.h>   //library for lcd
LiquidCrystal lcd(8,2,4,5,6,7); //Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

const int buttonPin_DU = 9;  // Duration Up
const int buttonPin_DD = 10; // Duration Down
const int buttonPin_IU = 11; // Interval Up
const int buttonPin_ID = 12; // Interval Down

const int outputPin = 3 ;

int duration = 300; // duration in minutes: 5 hour
int interval = 15;  // interval in minutes: 1 quarter

int camera_on = 0;

int buttonState_DU = 0;
int buttonState_DD = 0;
int buttonState_IU = 0;
int buttonState_ID = 0;

long lastTime = 0; // point in time in millisecs
long lastTimeDisplay = 0; // point in time in millisecs
long lastTimeButton_DU = 0; // point in time in millisecs
long lastTimeButton_DD = 0; // point in time in millisecs
long lastTimeButton_IU = 0; // point in time in millisecs
long lastTimeButton_ID = 0; // point in time in millisecs
long lastTimeAction = 0; // point in time in millisecs
long t = 0; //time in minutes
long r = 0;


void setup(){
  lcd.begin(16,2); //Initializes the interface to the lcd screen, and specifies the dimensions (width,height) of the display

  pinMode(buttonPin_DU, INPUT);
  pinMode(buttonPin_DD, INPUT);
  pinMode(buttonPin_IU, INPUT);
  pinMode(buttonPin_ID, INPUT);

  pinMode(outputPin, OUTPUT);
  }


void loop(){
  // count minutes
  if(millis()-lastTime > 60000){
    t++;
    lastTime = millis();
  }
  
  // the buttons
  // read the buttons
  if (digitalRead(buttonPin_DU) == 1){
    buttonState_DU = 1;
    lastTimeButton_DU = millis();
  }
  if (digitalRead(buttonPin_DD) == 1){
    buttonState_DD = 1;
    lastTimeButton_DD = millis();
  }
  if (digitalRead(buttonPin_IU) == 1){
    buttonState_IU = 1;
    lastTimeButton_IU = millis();
  }
  if (digitalRead(buttonPin_ID) == 1){
    buttonState_ID = 1;
    lastTimeButton_ID = millis();
  }
  //process the buttons
  //duration up: max = 300 minutes
  if (buttonState_DU == 1 and (millis()-lastTimeButton_DU) > 250){
    if (duration <= 270) {
    duration = duration + 30;
    }
  buttonState_DU = 0;
  lastTimeButton_DU = millis()*2;
  }
  //duration down: min = 30
  if (buttonState_DD == 1 and (millis()-lastTimeButton_DD) > 250){
    if (duration >= 60) {
    duration = duration - 30;
    }
  buttonState_DD = 0;
  lastTimeButton_DD = millis()*2;
  }
  //interval up:  max = 30 minutes
  if (buttonState_IU == 1 and (millis()-lastTimeButton_IU) > 250){
    if (interval <= 25) {
    interval = interval + 5; 
    }
  buttonState_IU = 0;
  lastTimeButton_IU = millis()*2;
  }
  //interval down:  min = 5
  if (buttonState_ID == 1 and (millis()-lastTimeButton_ID) > 250){
    if (interval >= 10) {
    interval = interval - 5;
    }
  buttonState_ID = 0;
  lastTimeButton_ID = millis()*2;
  }
  
  //calculate minutes remaining until nxt action
  r = interval - (t % interval);
  
  //the timed action
  //start action
  if (t % interval == 0 and camera_on == 0 and t < duration) {
    lastTimeAction = millis(); //start of action in millisecs
    digitalWrite(outputPin,1); //action on
    camera_on = 1;             //set signal for action to on
  }
  //stop action after 0.5 sec
  if (millis() - lastTimeAction > 250 and camera_on == 1) { //wait for 0.25 sec after start of action
    digitalWrite(outputPin,0);                              //set action to off
  }
  //End of timed action
  if (millis() - lastTimeAction > 60000 and camera_on == 1) { //wait for 60 secs after start of action
    camera_on = 0;                                            //set signal for action to off
  }

  
  // refresh display every 1 secs
  if(millis()-lastTimeDisplay > 1000){
  //the display
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Dur:");
  lcd.setCursor(4,0);
  lcd.print(duration);

  lcd.setCursor(8,0);
  lcd.print("Ela:");
  lcd.setCursor(12,0);
  lcd.print(t);

  lcd.setCursor(0,1);
  lcd.print("Int:");
  lcd.setCursor(4,1);
  lcd.print(interval);

  lcd.setCursor(8,1);
  lcd.print("Nxt:");
  lcd.setCursor(12,1);
  lcd.print(r);
  lastTimeDisplay = millis();
  }
  }
