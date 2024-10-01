#include "global.h"


void PreOpState::on_entry(){
	Serial.println("Entering Pre Op State");
    led.set_hi();
	on_do();
}

void PreOpState::on_exit(){
	Serial.println("Exiting Pre Op State");
}

void PreOpState::on_do(){
    while(1){
        led.toggle();
        _delay_ms(500);
        command_break = Serial.read();
        if(((char)command_break == 'r') || ((char)command_break == 'o')||((char)command_break == 's')){
            break;
        }
    }
}

void PreOpState::on_event(char event){
  if (event == 'o'){
	this->context_->transition_to(new OperationalState);
  }
  else if (event == 's'){
    this->context_->transition_to(new StoppedState);
  }
  else if (event == 'r'){
    this->context_->transition_to(new InitState);
  }
}