#include <avr/interrupt.h>
#include "global.h"

double Kp = 0.0135;
// 0.03 value Ku
// 2s value Tu
// Kp = 0.0135
// Ti = 1.66
// Ki = 0.0081

double Ti = 1.66;
double T = 0.01;

Encoder encA(1);
Encoder encB(2);

Digital_out led(5); //used to verify period on oscilloscope
Analog_out analog(4);
PI_Controller control;
Digital_in fault(3);

int max_speed; 
int current_speed;
double ref; // reference speed
double duty_cycle_first; //original duty cycle
int duty_cycle; //used to change the duty cycle according to u

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


