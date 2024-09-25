#include "global.h"
#include <avr/interrupt.h>

int command_break = 0;

void OperationalState::on_do()
{
    analog.init(duty_cycle_first);
    timer_speed.init_speed();
    timer_speed.count_speed  = 0;
    timer_pulses.init_pulses_count();
    analog.pin_digi.set_hi();
    sei();
    
    while(1){
      command_break = Serial.read();
      if((char)command_break == 'r'){
        break;
      }
    }
    Serial.print("---------> I received: ");
    Serial.println((char)command_break);
    context_->event((char)command_break);

}

void OperationalState::on_entry()
{
  Serial.println("Entering Operational State");
  led.set_hi();
  on_do();
}

void OperationalState::on_exit()
{
  // Stop Timer0 (used for speed)
  TCCR0B &= ~(1 << CS00);  // Clear the clock source bits to stop Timer0
  TCCR0B &= ~(1 << CS02);
  TIMSK0 &= ~(1 << OCIE0A); // Disable Timer0 Compare A interrupt

  // Stop Timer1 (used for PWM)
  TCCR1B &= ~(1 << CS10);   // Clear the clock source bits to stop Timer1
  TCCR1B &= ~(1 << CS11);
  TIMSK1 &= ~(1 << OCIE1A); // Disable Timer1 Compare A interrupt
  TIMSK1 &= ~(1 << OCIE1B); // Disable Timer1 Compare B interrupt

  // Stop Timer2 (used for pulse counting)
  TCCR2B &= ~(1 << CS20);   // Clear the clock source bits to stop Timer2
  TCCR2B &= ~(1 << CS22);
  TIMSK2 &= ~(1 << OCIE2A); // Disable Timer2 Compare A interrupt
  Serial.println("Exiting Operational State");

}

void OperationalState::on_event(char event){
//   if (event == 'p') { // Going to PreOperational
//     context_->transition_to(new PreOpState());
//   }
  if (event == 'r'){ // Reset 
    context_->transition_to(new InitState);
  }
}

ISR(TIMER2_COMPA_vect)
{
    if(last_state_A == last_state_B){
      curr_state_A = encA.is_low();
      if(curr_state_A != last_state_A){
        encA.count++;
        //led.toggle(); //to verify pulse count
      }
      else{ 
        curr_state_B  = encB.is_low();
      }
    }
    else{
      curr_state_A = encA.is_low();
      if(curr_state_A != last_state_A){
        encA.count--;
        //led.toggle(); //to verify pulse count
      }
      else{ 
        curr_state_B  = encB.is_low();
      }
    }
    last_state_A = curr_state_A;
    last_state_B = curr_state_B;
  }

ISR(TIMER0_COMPA_vect){
  timer_speed.count_speed++;

  if(timer_speed.count_speed >= 125){ //1s
    current_speed = encA.count;
    encA.count = 0;
    timer_speed.count_speed = 0;
    Serial.print("Ref: ");
    Serial.print(ref);
    Serial.print("--------->Current speed: ");
    Serial.print(current_speed);
    Serial.print("------>PWM: ");
    Serial.println(duty_cycle);
    //led.toggle(); // to verify 1s delay for speed
  }

  double u = control.update(ref, (double) current_speed);
  duty_cycle = (int) (duty_cycle - u/max_speed*100);
  if (duty_cycle > 95){ duty_cycle = 95;} //limiting the bound of the duty cycle
  if (duty_cycle <= 5){ duty_cycle = 5;} //limiting the bound of the duty cycle
  analog.set(duty_cycle);
  //led.toggle(); //to verify stable update
}

ISR(TIMER1_COMPA_vect){
  analog.pin_digi.set_lo();
  //led.set_lo(); // to verify stable pwm
}

ISR(TIMER1_COMPB_vect){
  analog.pin_digi.set_hi();
  // led.set_hi(); //to verify stable pwm
}