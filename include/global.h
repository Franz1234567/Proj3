#ifndef GLOBAL_H
#define GLOBAL_H

#include "timer_msec.h"
#include "encoder.h"
#include "digital_out.h"
#include "p_controller.h"
#include "analog_out.h"
#include "util/delay.h"
#include "Arduino.h"
#include "context.h"
#include <initialization.h>
#include <operational.h>
#include <stopped.h>
#include <pre_op.h>
#include <pi_controller.h>
#include <string.h>

extern double Kp;
extern double Ti;
extern double T;

extern Encoder encA;
extern Encoder encB;

extern Digital_out led;
extern Digital_in fault;

extern Analog_out analog;
extern PI_Controller control;

extern int max_speed ; 
extern int current_speed;
extern double ref; // reference speed
extern double duty_cycle_first; //original duty cycle
extern int duty_cycle; // not used anymore

extern bool last_state_A;
extern bool last_state_B;
extern bool curr_state_A;
extern bool curr_state_B;
extern bool advanced_control;

extern Timer_msec timer_speed;
extern Timer_msec timer_pulses;

extern int command_break; // for incoming serial data

#endif