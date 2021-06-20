/********************************************
 * Includes and Macro
 ********************************************/
#include <SPI.h>
#include "RF24.h"

// Sender Types
#define TYPE_BUS 0x03

// ACK
#define CMD_OK 0x01
#define CMD_NO 0x02

// Move
#define CMD_MOVE_START 0x06
#define CMD_MOVE_END 0x07
#define CMD_MOVE 0x08

// Directions
#define DIR_F 0x01 // 0000 0001
#define DIR_B 0x02 // 0000 0010
#define DIR_R 0x04 // 0000 0100
#define DIR_L 0x08 // 0000 1000
#define DIR_N 0x10 // 0001 0000

// Protocol
#define FIELD_START 0
#define FIELD_SENDER 1
#define FIELD_COMMAND 2
#define FIELD_DATA_H8 3
#define FIELD_DATA_L8 4
#define FIELD_END 5

// Motor
#define MOTOR_1 2 // d2
#define MOTOR_2 3 // d3
#define MOTOR_3 4 // d4
#define MOTOR_4 5 // d5

#define MOTOR_ENA A4 // A4
#define MOTOR_ENB A3 // A3

// IR Sensor
#define IRL 11 // d7
#define IRC 12 // d8
#define IRR 13 // d9

#define ADDRESS_RPI 0
#define ADDRESS_BUS 1

/********************************************
 * Global Variables
 ********************************************/
 
// pin 10 : CE
// pin 9  : CSN
RF24 radio(10, 9);

uint8_t address[][6] = {"Node1", "Node2"};
uint8_t payload[6] = {0, 0, 0, 0, 0, 0};

bool is_start = false;

/********************************************
 * Setup
 ********************************************/
void setup() {
  // Serial setup
  Serial.begin(115200);
  
  // Motor pin setup
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);
  pinMode(MOTOR_4, OUTPUT);

  // IR sensor pin setup
  pinMode(IRL, INPUT);
  pinMode(IRC, INPUT);
  pinMode(IRR, INPUT);

  pinMode(MOTOR_ENA, OUTPUT);
  digitalWrite(MOTOR_ENA, HIGH);
  pinMode(MOTOR_ENB, OUTPUT);
  digitalWrite(MOTOR_ENB, HIGH);

  // RF setup
    while(!radio.begin()) {
    Serial.println("RF module...");
  }
  
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payload));

  radio.openWritingPipe(address[ADDRESS_BUS]);    // TX, pipe 0
  radio.openReadingPipe(1, address[ADDRESS_RPI]); // RX, pipe 1
  
  radio.startListening();
}

/********************************************
 * Loop
 ********************************************/
void loop() {
  uint8_t pipe;
  uint8_t command = 0;
  uint16_t data = 0;
  
  if (rf_read(&pipe)) {
    command = payload[FIELD_COMMAND];
    data = get_data_from_payload();
    
    if (command == CMD_MOVE_START && !is_start) {
      is_start = true;
      rf_send(CMD_OK, 0);
    } else if (command == CMD_MOVE) {
      if (data & DIR_F) {
        move_forward();
        while (digitalRead(IRL) || digitalRead(IRR));
        move_stop();
        rf_send(CMD_MOVE_END, 0);
        is_start = false;
      }
    }
  }
}
/********************************************
 * RF Communication Functions
 ********************************************/
uint16_t get_data_from_payload() {
  uint8_t data_h8 = payload[FIELD_DATA_H8];
  uint8_t data_l8 = payload[FIELD_DATA_L8];
  uint16_t data = (data_h8 << 8) | data_l8;
  return data;
}

void rf_send(uint8_t command, uint16_t data) {
  uint8_t data_h8 = (data & 0xFF00) >> 8;
  uint8_t data_l8 = data & 0x00FF;
  
  payload[FIELD_START] = 0x02;
  payload[FIELD_SENDER] = TYPE_BUS;
  payload[FIELD_COMMAND] = command;
  payload[FIELD_DATA_H8] = data_h8;
  payload[FIELD_DATA_L8] = data_l8;
  payload[FIELD_END] = 0x03;
  
  radio.stopListening();
  bool report = radio.write(&payload, sizeof(payload));
  while(!report) {
    report = radio.write(&payload, sizeof(payload));
  }
  radio.startListening();
}

bool rf_read(uint8_t *pipe) {
  bool is_available = radio.available(pipe) > 0;
  if (is_available) {
    uint8_t bytes = radio.getPayloadSize();
    radio.read(&payload, bytes);
  }
  return is_available;
}
 
/********************************************
 * Move Functions
 ********************************************/
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

void move_stop() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
}
