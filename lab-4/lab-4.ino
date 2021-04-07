#define DING 21
#define B1 10
#define B2 11
#define B3 50
#define B4 51

struct Time {
  int secs; 
  int mins; 
  int hours;
} ;

int portapins[6] = {22, 23, 24, 25, 26, 27};
Time main_time = {0, 0, 0};
int scaler = 0;

const int REGISTER_PATTERNS[6] = { 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000 };
const int SEGMENT_PATTERNS[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111 };

Time MEMORY[10];
volatile int current_memory_position = 0;

volatile bool TIMER_ACTIVE = true;

void setup() {
  pinMode(30, OUTPUT);
  for (int i = 0; i < 6; i ++){
    pinMode(portapins[i], OUTPUT);
  }
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 65535;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect){
  if (scaler == 1){
  if (TIMER_ACTIVE){
    if (main_time.secs < 59){
      main_time.secs ++;
    }
    else if (main_time.mins < 59){
      main_time.secs = 0;
      main_time.mins ++;
      beep();
    }
    else {
      main_time.hours++;
      main_time.mins = 0;
      main_time.secs = 0;
      beep();
    }
  }
  scaler = 0;
  }
  else{
    scaler ++;
  }
}

void beep(){
  digitalWrite(DING, HIGH);
  delay(200);
  digitalWrite(DING, LOW);
}

void loop() {
  show_time();
  int b1 = digitalRead(B1);
  int b2 = digitalRead(B2);
  int b3 = digitalRead(B3);
  int b4 = digitalRead(B4);

  delay(30);
  if (!b1 && digitalRead(B1)) pressA();
  if (!b2 && digitalRead(B2)) pressB();
  if (!b3 && digitalRead(B3)) pressC();
  if (!b4 && digitalRead(B4)) pressD();
}

void clear_all(){
  int portapins[6] = {22, 23, 24, 25, 26, 27};
  for (int i = 0; i < 6; i++){
    digitalWrite(portapins[i], LOW);
  }
}

void make_dot(){
  digitalWrite(30, HIGH);
  delayMicroseconds(2000);
  digitalWrite(30, LOW);
}

void pressA(){
  TIMER_ACTIVE = !TIMER_ACTIVE;
  if(TIMER_ACTIVE){
    main_time = {0, 0, 0};
  }
}

void pressB(){
  MEMORY[current_memory_position] = main_time;
  if (++current_memory_position >= 10){
    current_memory_position = 0;
  }
}
void pressC(){
  current_memory_position = 0;
}
void pressD(){
  Time memory_time = main_time;
  if (!TIMER_ACTIVE){
    for(int i = 0; i < current_memory_position; i++){
      main_time = MEMORY[i];
      for (int j = 0; j < 50; ++j){
        show_time();
      }
    }
  }
  main_time = memory_time;
}



void show_time(){
  int array_to_show[6] = {main_time.secs % 10, main_time.secs / 10, main_time.mins % 10, main_time.mins / 10, main_time.hours % 10, main_time.hours / 10};
  clear_all();
  digitalWrite(22, HIGH);
  PORTC = SEGMENT_PATTERNS[(int)main_time.hours / 10];
  delayMicroseconds(2000);
  clear_all();
  digitalWrite(23, HIGH);
  make_dot();
  PORTC = SEGMENT_PATTERNS[(int)main_time.hours % 10];
  delayMicroseconds(2000);
  clear_all();
  digitalWrite(24, HIGH);
  PORTC = SEGMENT_PATTERNS[(int)main_time.mins / 10];
  delayMicroseconds(2000);
  clear_all();
  digitalWrite(25, HIGH);
  make_dot();
  PORTC = SEGMENT_PATTERNS[(int)main_time.mins % 10];
  delayMicroseconds(2000);
  clear_all();
  digitalWrite(26, HIGH);
  PORTC = SEGMENT_PATTERNS[(int)main_time.secs / 10];
  delayMicroseconds(2000);
  clear_all();
  digitalWrite(27, HIGH);
  PORTC = SEGMENT_PATTERNS[(int)main_time.secs % 10];
  delayMicroseconds(2000);
  
}
