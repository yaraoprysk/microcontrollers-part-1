#include <Arduino.h>

//THE KEYPAD REQUIRES PULL-UP RESISTORS ON THE ROW PINS. This defines a custom keypad and enable you to enter hexadecimal values
#include <Keypad.h>


//This library allows an Arduino board to control LiquidCrystal displays (LCDs) based on the Hitachi HD44780
#include <LiquidCrystal.h>

//Port C
#define D4 A3
#define D5 A2
#define D6 A1
#define D7 A0

//Port F
#define RS 5
#define RW 6
#define EN 7

#define BUZZER 11

#define ROWS 4
#define COLUMNS 4

#define SHORT_SIGNAL_DELAY 50
#define LONG_SIGNAL_DELAY 500
#define INTERMEDIATE_VALUES 60

LiquidCrystal lcd(RS, RW, EN, D4, D5, D6, D7);

//define the cymbols on the buttons of the customKeypads
char hexaKeys[ROWS][COLUMNS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {37, 36, 35, 34};//connect to the row pinouts of the customKeypad
byte columnPins[COLUMNS] = {33, 32, 31, 30 }; //connect to the column pinouts of the customKeypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, columnPins, ROWS, COLUMNS);

bool isSignalBeep = false;
bool stop = false;


struct Time
{
  uint8_t hour, minute, second;
};
Time time = {0, 0, 0};

Time memoryRegister[INTERMEDIATE_VALUES];

uint8_t sizeOfMemoryRegister = 0;



//Interrupt Service Routine
ISR(TIMER5_COMPA_vect)
{
  if (!stop)
  {
    if (++time.second == 60)
    {
      time.second = 0;
      if (++time.minute == 60)
      {
        time.minute = 0;
        if (++time.hour == 24)
          time.hour = 0;
      }
    }

    if (time.second == 0)
    {
      isSignalBeep = true;
    }
  }
}

inline void stopwatch(const Time &time);

inline void buttonA();
inline void buttonB(uint8_t &recentTimeSavedId);
inline void buttonC();
inline void buttonD();

bool isApply();

inline void shortSignal();
inline void longSignal();
inline void doubleShortSignal();

inline bool checkId(const uint8_t &index);
inline void resetMemoryRegister();
inline void saveToMemoryRegister();

void setup()
{
  noInterrupts();//Disables interrupts

  pinMode(BUZZER, OUTPUT);
  lcd.begin(16, 2);

//T5
  TCCR5A = WGM50;
  TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); //CTC mode & Prescaler @ 1024
  TIMSK5 = (1 << OCIE5A);                            // permition for interruption
  OCR5A = 0x3D08;                                    // 1 sec (16MHz AVR)

  interrupts();//Re-enables interrupts 
}

void loop()
{
  char selectedButton = customKeypad.getKey();
  delay(100);
  switch (selectedButton)
  {
  case 'A':
    shortSignal;
    buttonA();
    break;

  case 'C':
    shortSignal;
    buttonC();
    break;

  case 'D':
    shortSignal;
    buttonD();
    break;

  case '*':
    shortSignal;
    resetMemoryRegister();
    break;
  
  case '#':
    shortSignal;
    saveToMemoryRegister();
    break;
  
  default:
    stopwatch(time);
    delay(100);
    break;
  }

  if (isSignalBeep)
  {
    longSignal();
    isSignalBeep = false;
  }
}



inline void resetMemoryRegister()
{
  sizeOfMemoryRegister = 0;
  doubleShortSignal();
}



inline void saveToMemoryRegister()
{
  if ((sizeOfMemoryRegister + 1) <= INTERMEDIATE_VALUES)
  {
    memoryRegister[++sizeOfMemoryRegister - 1] = time;
    doubleShortSignal();
  }
  else
  {
    longSignal();
  }
}

//displays the stored memory log on the LCD countdown. Pressing again returns to the previous mode.
inline void buttonA()
{
  Serial.begin(9600);
  Serial.print("a");
  lcd.clear();
  if (sizeOfMemoryRegister > 0)
  {
    uint8_t input[2];
    uint8_t inputSize = 0;
    uint8_t recentTimeSavedId = 0;
    lcd.setCursor(0, 0);
    lcd.print("0");
    lcd.print(recentTimeSavedId + 1);
    lcd.setCursor(3, 0);
    stopwatch(memoryRegister[recentTimeSavedId]);
    lcd.setCursor(13, 0);
    lcd.print("M");
    if (sizeOfMemoryRegister < 10)
    {
      lcd.print("0");
    }
    lcd.print(sizeOfMemoryRegister);
    lcd.setCursor(5, 1);
    lcd.print("<-A Y-# B->");
    lcd.setCursor(0, 1);

    char currentButton;
    while (true)
    {
      if (currentButton = customKeypad.getKey())
      {
        if (currentButton == 'A')
        {
          shortSignal;
          lcd.clear();
          break;
        }
        else if (currentButton == 'B')
        {
          shortSignal;
          buttonB(recentTimeSavedId);
        }
        else if (currentButton == '#')
        {
          shortSignal;
          if (inputSize == 0)
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longSignal();
            lcd.setCursor(0, 1);
            lcd.print("   ");
          }
          else if (checkId(inputSize == 2 ? (input[0] * 10 + input[1] - 1) : input[0] - 1))
          {
            recentTimeSavedId = inputSize == 2 ? (input[0] * 10 + input[1] - 1) : input[0] - 1;
            lcd.setCursor(0, 0);
            if (recentTimeSavedId < 10)
            {
              lcd.print("0");
            }
            lcd.print(recentTimeSavedId + 1);
            lcd.setCursor(3, 0);
            stopwatch(memoryRegister[recentTimeSavedId]);
            inputSize = 0;

            lcd.setCursor(0, 1);
            lcd.print("DONE");
            doubleShortSignal();
            delay(200);
            lcd.setCursor(0, 1);
            lcd.print("    ");
          }
          else
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longSignal();
            lcd.setCursor(0, 1);
            lcd.print("   ");
            inputSize = 0;
          }
        }
        else if (int(currentButton) <= int('9') && int(currentButton) >= int('0'))
        {
          shortSignal;
          if (inputSize < 2)
          {
            lcd.setCursor(inputSize, 1);
            lcd.print(currentButton);
            inputSize++;
            input[inputSize - 1] = currentButton - '0';
          }
          else
          {
            lcd.setCursor(0, 1);
            lcd.print("BAN");
            longSignal();
            lcd.setCursor(0, 1);
            lcd.print("   ");
            inputSize = 0;
          }
        }
      }
    }
  }
  else
  {
    lcd.print("Register's empty");

    lcd.setCursor(10, 1);
    lcd.print("Back-*");

    while (true)
    {
      if (customKeypad.getKey() == '*')
      {
        break;
        Serial.print("*");
      }
    }
    lcd.clear();
  }
}

//in memory view mode alternately scrolls the values ​​of the saved time readings on the LCD ordinal no.
inline void buttonB(uint8_t &recentTimeSavedId)
{
  Serial.print("b");
  if (checkId(recentTimeSavedId + 1))
  {
    recentTimeSavedId++;
  }
  lcd.setCursor(0, 0);
  if (recentTimeSavedId < 10)
  {
    lcd.print("0");
  }
  lcd.print(recentTimeSavedId + 1);
  stopwatch(memoryRegister[recentTimeSavedId]);
}

//resets the countdown to zero and stops readout.
inline void buttonC()
{
  Serial.print("c");
  time = {0, 0, 0};
  stop = true;
}

//starts or stops the countdown
inline void buttonD()
{
  Serial.print("a");
  stop = !stop;
}

inline bool checkId(const uint8_t &index)
{
  return index >= 0 && index < sizeOfMemoryRegister;
}

bool isApply()
{
  lcd.setCursor(11, 1);
  lcd.print("Yes-#");

  while (true)
  {
    if (customKeypad.getKey() == '#')
    {
      return true;
      Serial.print("#");
    }
    delay(50);
  }
}

inline void shortSignal()
{
  digitalWrite(BUZZER, HIGH);
  delay(SHORT_SIGNAL_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void doubleShortSignal()
{
  digitalWrite(BUZZER, HIGH);
  delay(SHORT_SIGNAL_DELAY);
  digitalWrite(BUZZER, LOW);
  delay(SHORT_SIGNAL_DELAY / 2);
  digitalWrite(BUZZER, HIGH);
  delay(SHORT_SIGNAL_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void longSignal()
{
  digitalWrite(BUZZER, HIGH);
  delay(LONG_SIGNAL_DELAY);
  digitalWrite(BUZZER, LOW);
}

inline void stopwatch(const Time &time)
{
  lcd.setCursor(3, 0);
  if (time.hour < 10)
    lcd.print("0");
  lcd.print(time.hour);
  lcd.print(":");
  if (time.minute < 10)
    lcd.print("0");
  lcd.print(time.minute);
  lcd.print(":");
  if (time.second < 10)
    lcd.print("0");
  lcd.print(time.second);
}
