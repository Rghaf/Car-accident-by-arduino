#include <Keypad.h>
#include <LiquidCrystal.h>

//pins for lcd
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

//switch
const int switchPin = 0;
int switchStatus;

//motor
const int motorPin = 10;

//pins for ultrasonicsensor
const int echoPin = 9;
const int trigPin = 8;


bool lock = true; //bool that shows car is locked or not

#define Password_Lenght 7 // Give enough room for six chars + NULL char

char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "147258";
byte data_count = 0, master_count = 0; //number of inputs char
bool Pass_is_good;
char customKey; //inputs from keypad

//define keypads buttons
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Robotics Lab");
  lcd.setCursor(0, 1);
  lcd.print("----Car Security---");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Reza Ghadiri ");
  lcd.setCursor(0,1);
  lcd.print(" Sepehr Bayatnezhad ");
  delay(1000);
  lcd.clear();

  pinMode(switchPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop()
{
  if (lock == 0)
  {
    delay(1000);
    switchStatus = digitalRead(switchPin);
    if(switchStatus == LOW){
      digitalWrite(motorPin, HIGH);
      AccidentCheck();
    }
    else {
      digitalWrite(motorPin, LOW);
    }
    customKey = customKeypad.getKey();
    if (customKey == '*')
    {
      lcd.clear();
      lcd.print(" Car is locked");
      delay(3000);
      lock = 1;
    }
  }

  else Open();
}

void clearData()
{
  while (data_count != 0)
  { // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}


void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count, 1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Lenght - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
      lcd.clear();
      lcd.print("  Car is Open  ");
      lock = 0;
    }
    else
    {
      lcd.clear();
      lcd.print(" Wrong Password ");
      delay(1000);
      lock = 1;
    }
    clearData();
  }
}

void AccidentCheck()
{
  lcd.clear();
  int carspeed = 100;
  long duration, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration=pulseIn(echoPin, HIGH);
  cm=MicroToCm(duration);
  lcd.print("distance: ");
  lcd.print(cm);
  lcd.print(" cm");
  if(cm < 50){
    carspeed = 0;
  }
  lcd.setCursor(0,1);
  lcd. print("speed: ");
  lcd.print(carspeed);
  lcd.print("km");
}

unsigned long MicroToCm(unsigned long Micros){
    return (Micros/29)/2;
  }
