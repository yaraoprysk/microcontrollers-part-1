#define BASIC_LED_PIN A0 // 3-10
#define BUTTON_PIN 67 // A13
#define CYCLE_DELAY 1150 // ms 

int led_pins[8];

void setup(){
    for (int i = 0; i < 8; i++){
      led_pins[i] = BASIC_LED_PIN;
      led_pins[i] += i;
      pinMode(led_pins[i], OUTPUT);
    }
   pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void algorithm(){
  // HIGH 
  // LOW
int multic = 1;
for(int i = 0; i < 4; i+= 1){
       digitalWrite(led_pins[int(3.5 + multic *(3.5 - i))], HIGH);
       delay(CYCLE_DELAY);
       digitalWrite(led_pins[int(3.5 + multic *(3.5 - i))], LOW);
        // NEXT
       digitalWrite(led_pins[int(3.5 - multic *(3.5 - i))], HIGH);
       delay(CYCLE_DELAY);
       digitalWrite(led_pins[int(3.5 - multic *(3.5 - i))], LOW);
  }
}


void loop(){
  if (!digitalRead(BUTTON_PIN)){
    algorithm();
  }
}
