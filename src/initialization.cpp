#include <initialization.h>
#include <Operational.h>
#include <context.h>
#include <Arduino.h>
#include <timer_msec.h>
#include <encoder.h>
#include <digital_out.h>
#include <p_controller.h>
#include <analog_out.h>


void InitState::on_entry(){
	Serial.println("Init entry");
	on_do();
}

void InitState::on_exit(){
	Serial.println("Init exit");
}

void InitState::on_do(){
	Serial.println("Device startup");
	const double Kp = 0.05;

	Encoder encA(1);
	Encoder encB(2);

	Digital_out led(5); //used to verify period on oscilloscope
	Analog_out analog(4);
	P_controller control(Kp);

	const int max_speed = 2800; 
	const float speed_63_ref = max_speed*0.63;
	bool speed_63_found = 0;
	int current_speed = 0;
	const double ref = 100; // reference speed
	double duty_cycle_first = 50; //original duty cycle
	int duty_cycle = 50; //used to change the duty cycle according to u

	bool last_state_A = encA.is_low();
	bool last_state_B = encB.is_low();
	bool curr_state_A;
	bool curr_state_B;
	int print_counter1 = 0;

	Timer_msec timer_speed;
	Timer_msec timer_pulses;
	
	led.init();
	analog.pin_digi.set_lo();
	Serial.println("Variables created !");
}

void InitState::on_event(char event)
{
  if (event == 'o'){
	this->context_->transition_to(new OperationalState);
  }
}