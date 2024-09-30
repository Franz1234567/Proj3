#include "global.h"

void StoppedState::on_entry(){
	Serial.println("Entering Stopped State");
    led.set_hi();
    analog.pin_digi.set_lo();
	on_do();
}

void StoppedState::on_exit(){
	Serial.println("Exiting Stopped State");
}

void StoppedState::on_do(){
    command_break = 0;
    while(1){
        led.toggle();
        _delay_ms(250);
        command_break = Serial.read();
        if(((char)command_break == 'r') || ((char)command_break == 'o')||((char)command_break == 'p')){
            break;
        }
    }
    Serial.print("---------> I received: ");
    Serial.println((char)command_break);
    context_->event((char)command_break);
    }

void StoppedState::on_event(char event)
{
    if (event == 'r'){
	this->context_->transition_to(new InitState);
    }
    else if (event == 'o'){
        this->context_->transition_to(new OperationalState);
    }
    // else if (event == 'p'){
    //     this->context_->transition_to(new PreOpState);
    // }
}