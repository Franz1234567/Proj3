#include <avr/interrupt.h>
#include "global.h"

// basic values that can be resetted in pre-operational state
double Kp = 0.0135;
double Ti = 1.66;
double T = 0.01;

// global variables 
Encoder encA(1);
Encoder encB(2);

Digital_out led(5);
Analog_out analog(4);
PI_Controller control;
Digital_in fault(3);

int max_speed; 
int current_speed;
double ref; // reference speed
double duty_cycle_first; //original duty cycle

bool last_state_A;
bool last_state_B;
bool curr_state_A;
bool curr_state_B;

Timer_msec timer_speed;
Timer_msec timer_pulses;

int command = 0; // for incoming serial data
int command_break = 0;
Context* context;

void setup()
{
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  context = new Context(new InitState);
}

void loop()
{
  if (Serial.available() > 0){
    command = Serial.read();
    Serial.print("---------> I received: ");
    Serial.println((char)command);
    context->event((char)command);
  }
}


