#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include "HCPCA9685.h" 
#define I2CAdd 0x40 



const int chipSelect = 10;   //button pin for sd card reader
const int buttonPin = 5;   //button pin for recording current position
const int modeBtn = 9;  //button pin for selecting mode 
const int delBtn = 8;   //button for deleting card and card files
int modeSelect;
int buttonState = 0;
int delbuttonState = 0;
File dataFile;
String linija, buff; 
Servo myservo;
int val;       // pot values
int val1;
int val2;
int val3;
int val4;
int val5;

int delval = 500;
int pos1 = 210;      //new values for slow speed of servos
int pos2 = 210;
int pos3 = 210;
int pos4 = 210;
int pos5;
int newVal= 210;

const int ledPin1 =  3;     //led pins
const int ledPin2 =  4;
const int ledPin3 =  6;
const int ledPin4 =  7;

HCPCA9685 HCPCA9685(I2CAdd);




int indexi[4] = {0, 0, 0, 0};
int x, y, z, r;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(delBtn, INPUT);
  pinMode(modeBtn, INPUT);
  modeSelect = digitalRead(modeBtn);
  
  HCPCA9685.Init(SERVO_MODE); // Set to Servo Mode

  HCPCA9685.Sleep(false); // Wake up PCA9685 module


  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);


  


  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");

  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }
}

void loop()
{ 

delbuttonState = digitalRead(delBtn);

  if (modeSelect == HIGH) { //button is in learning mode
    buttonState = digitalRead(buttonPin);
    
    val = map(analogRead(0), 0, 1023, 420, 10);
    val1 = map(analogRead(1), 0, 1023, 10, 420);
    val2 = map(analogRead(2), 0, 1023, 10, 420);
    val3 = map(analogRead(3), 0, 1023, 10, 420);
    HCPCA9685.Servo(0, val);
    HCPCA9685.Servo(1, val1);
    HCPCA9685.Servo(2, val1);
    HCPCA9685.Servo(3, val2);
    HCPCA9685.Servo(4, val3);
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin3, LOW);

    
    String dataString = "";
    if (buttonState == HIGH) {



      
      for (int analogPin = 0; analogPin < 4; analogPin++) {
        if (analogPin == 0) {
          dataString += "x";
        }
        else if (analogPin == 1) {
          dataString += "y";
        }
        else if (analogPin == 2) {
          dataString += "z";
        }
        else if (analogPin == 3) {
          dataString += "r";
        }
        int sensor = analogRead(analogPin);
        dataString += String(sensor);
      }

      dataFile.println(dataString);
      digitalWrite(ledPin4, HIGH);
      
      dataFile.flush();
      delay(500);

      

      
    }
    else {
      //Serial.println("nista");
      digitalWrite(ledPin4, LOW);
    }
    if(delbuttonState == HIGH){
      
      SD.remove("datalog.txt");
      digitalWrite(ledPin2, HIGH);
      
      
      }
    else{
    //Serial.println("neobrisano obrisano"); 
    digitalWrite(ledPin2, LOW); 
      }
      

  }
  else if (modeSelect == LOW) { //button is in play mode
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin1, LOW);
    dataFile.close();
    dataFile = SD.open("datalog.txt", FILE_READ);
    while (dataFile.available()) {
      linija = dataFile.readStringUntil('\n');
      
      indexi[0] = linija.indexOf('x');
      indexi[1] = linija.indexOf('y');
      indexi[2] = linija.indexOf('z');
      indexi[3] = linija.indexOf('r');
      .
      buff = linija.substring(indexi[0]+1, indexi[1]);
      //Serial.println(buff);
      x = buff.toInt();
      buff = linija.substring(indexi[1]+1, indexi[2]);
      //Serial.println(buff);
      y = buff.toInt();
      buff = linija.substring(indexi[2]+1, indexi[3]);
      //Serial.println(buff);
      z = buff.toInt();
      buff = linija.substring(indexi[3]+1, linija.length());
      //Serial.println(buff);
      r = buff.toInt();


val = map(x, 0, 1023, 420, 10);
val1 = map(y, 0, 1023, 10, 420);
val2 = map(z, 0, 1023, 10, 420);
val3 = map(r, 0, 1023, 10, 420);
val4 = map(analogRead(2), 0, 1023, 0, 15);
val5 = map(analogRead(3), 0, 1023, 0, 4);

if(val5 <= 1){
  while(pos1 <= val){
    pos1 +=val4;
    HCPCA9685.Servo(0, pos1);
    delay(50);
  }


  while(pos1 >= val){
    pos1 -= val4;
    HCPCA9685.Servo(0, pos1);
    delay(50);
  }
 

while(pos2 <= val1){
    pos2 +=val4;
    HCPCA9685.Servo(1, pos2);
    HCPCA9685.Servo(2, pos2);
    delay(50);
  }


  while(pos2 >= val1){
    pos2 -= val4;
    HCPCA9685.Servo(1, pos2);
    HCPCA9685.Servo(2, pos2);
    delay(50);
  }


while(pos3 <= val2){
    pos3 +=val4;
    HCPCA9685.Servo(3, pos3);
    delay(50);
  }


  while(pos3 >= val2){
    pos3 -= val4;
    HCPCA9685.Servo(3, pos3);
    delay(50);
  }

while(pos4 <= val3){
    pos4 +=val4;
    HCPCA9685.Servo(4, pos4);
    delay(50);
  }


  while(pos4 >= val3){
    pos4 -= val4;
    HCPCA9685.Servo(4, pos4);
    delay(50);
  }

  
pos1 = val;
pos2 = val1;
pos3 = val2;
pos4 = val3;
}

else if(val5 > 1){
HCPCA9685.Servo(0, val); 
HCPCA9685.Servo(1, val1);
HCPCA9685.Servo(2, val1);
HCPCA9685.Servo(3, val2);
HCPCA9685.Servo(4, val3);

//Serial.println(val2);
      delay(500);
}

    }
  }
  
  
}
