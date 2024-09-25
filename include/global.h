#ifndef GLOBAL_H
#define GLOBAL_H

#include "timer_msec.h"
#include "encoder.h"
#include "digital_out.h"
#include <avr/interrupt.h">
#include "p_controller.h"
#include "analog_out.h"
#include "util/delay.h"
#include "Arduino.h"

extern const double Kp = 0.05;

extern Encoder encA;
extern Encoder encB;

extern Digital_out led; //used to verify period on oscilloscope
extern Analog_out analog;
extern P_controller control;

extern const int max_speed ; 
extern bool speed_63_found;
extern int current_speed;
extern const double ref; // reference speed
extern double duty_cycle_first; //original duty cycle
extern nt duty_cycle; //used to change the duty cycle according to u

extern bool last_state_A;
extern bool last_state_B;
extern bool curr_state_A;
extern bool curr_state_B;
extern int print_counter1 = 0;

extern Timer_msec timer_speed;
extern Timer_msec timer_pulses;

#endif