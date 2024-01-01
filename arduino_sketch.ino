#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
#include <dht.h>        // Include library
#define outPin 2
dht DHT; 
const int lm35_pin = A3;	

#define sensor_pin A1
int read_ADC;
int ntu;

#include <Wire.h>
#include <SimpleTimer.h>
 
SimpleTimer timer;
 
float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
 
float ph_act;
// for the OLED display



void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
    Wire.begin();
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Hello world!");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("LCD Tutorial");
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(readTemperature());

  // delay(2000);
  // lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("pH:");
  lcd.print(readpH());

  lcd.setCursor(8, 0);
  lcd.print("Tur:");
  lcd.print(int(readTurbidity()));

  delay(1000);
  lcd.clear();
}

float readTemperature() {
int readData = DHT.read11(2);

	float t = DHT.temperature;        // Read temperature

  return t;
}


float readpH() {

   for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 
 Serial.println("pH Val: ");
 Serial.print(ph_act);
 delay(1000);

  return ph_act;
}


float readTurbidity() {

  read_ADC = analogRead(A1);
if(read_ADC>208)read_ADC=208;

ntu = map(read_ADC, 0, 208, 300, 0); 
 Serial.println(ntu);
  return int(ntu);
}