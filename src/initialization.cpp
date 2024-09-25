#include "global.h"


void InitState::on_entry(){
	Serial.println("Entering Init State");
	on_do();
}

void InitState::on_exit(){
	Serial.println("Exiting Init State");
}

void InitState::on_do(){
	Kp = 0.05;

	max_speed = 2800; 
	current_speed = 0;
	ref = 100; // reference speed
	duty_cycle_first = 50; //original duty cycle
	duty_cycle = 50; //used to change the duty cycle according to u

	last_state_A = encA.is_low();
	last_state_B = encB.is_low();
	
	led.init();
	led.set_lo();
	analog.pin_digi.set_lo();
	Serial.println("Variables initialized !");
}

void InitState::on_event(char event)
{
  if (event == 'o'){
	this->context_->transition_to(new OperationalState);
  }
}