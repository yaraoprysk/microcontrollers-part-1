int const basic_led = A0; // 3-10
int const button_pin_1 = 67; // A13
int const button_pin_2 = 69; // A15
int const cycle_delay_ms = 1150; // ms 

int led_pins[8];

void setup() {
  for (int i = 0; i < 8; i++) {
    led_pins[i] = basic_led;
    led_pins[i] += i;
    pinMode(led_pins[i], OUTPUT);
  }
  Serial.begin(9600);
  pinMode(button_pin_1, INPUT_PULLUP);
  pinMode(button_pin_2, INPUT_PULLUP);
}

void algorithm_1() {
  for (int i = 0; i < 4; i += 1) {
    digitalWrite(led_pins[int(3.5 + (3.5 - i))], HIGH);
    delay(cycle_delay_ms);
    digitalWrite(led_pins[int(3.5 + (3.5 - i))], LOW);
    // NEXT
    digitalWrite(led_pins[int(3.5 - (3.5 - i))], HIGH);
    delay(cycle_delay_ms);
    digitalWrite(led_pins[int(3.5 - (3.5 - i))], LOW);
  }
}

void algorithm_2() {
  for (int i = 0; i < 4; i += 1) {
    digitalWrite(led_pins[int(3.5 + (3.5 - i))], HIGH);
    digitalWrite(led_pins[int(3.5 - (3.5 - i))], HIGH);
    delay(cycle_delay_ms);
    digitalWrite(led_pins[int(3.5 + (3.5 - i))], LOW);
    digitalWrite(led_pins[int(3.5 - (3.5 - i))], LOW);
  }
}


void loop() {
  char algo_number = Serial.read();
  unsigned int button_1 = digitalRead(button_pin_1);
  unsigned int button_2 = digitalRead(button_pin_2);
  if (!button_1) {
    Serial.print('1');
    delay(cycle_delay_ms);
  }
  else if (algo_number == '1') {
    algorithm_1();
  }
  
  else if (algo_number == '2') {
    algorithm_2();
  }
  
  else if (!button_2){
    Serial.print('2');
    delay(cycle_delay_ms);
  }
}
