// Motor
#define MOTOR_1 2 // d2
#define MOTOR_2 3 // d3
#define MOTOR_3 4 // d4
#define MOTOR_4 5 // d5

#define MOTOR_ENA A4 // A4
#define MOTOR_ENB A3 // A3

// IR Sensor
#define IR1 11 // d7
#define IR2 12 // d8
#define IR3 13 // d9

void setup() {
  // Motor pin setup
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);
  pinMode(MOTOR_4, OUTPUT);

  // IR sensor pin setup
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  pinMode(MOTOR_ENA, OUTPUT);
  digitalWrite(MOTOR_ENA, HIGH);
  pinMode(MOTOR_ENB, OUTPUT);
  digitalWrite(MOTOR_ENB, HIGH);
}

void move_forward() {
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, HIGH);
  digitalWrite(MOTOR_4, LOW);
}

void move_backward() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, HIGH);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, HIGH);
}

void move_left() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, HIGH);
  digitalWrite(MOTOR_4, LOW);
}

void move_right() {
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}

void loop() {
  move_forward();
  delay(1000);
  move_backward();
  delay(1000);
  move_right();
  delay(1000);
  move_left();
  delay(1000);
}
