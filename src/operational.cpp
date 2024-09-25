#include "operational.h"
#include "context.h"
#include <Arduino.h> 
#include <util/delay.h>

void OperationalState::on_do()
{
    led.init();
    analog.init(duty_cycle_first);

    timer_speed.init_speed();
    timer_speed.count_speed  = 0;

    timer_pulses.init_pulses_count();

    analog.pin_digi.set_hi();
    sei();
    
    while(1){}

}

void OperationalState::on_entry()
{

}

void OperationalState::on_exit()
{
  Serial.println("Exiting Operational state");
}

void OperationalState::on_event(char event){
//   if (event == 'p') { // Going to PreOperational
//     context_->transition_to(new PreOpState());
//   }
  if (event == 'r'){ // Reset 
    context_->transition_to(new InitState());
  }
}