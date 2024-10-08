#include "global.h"
#include <avr/interrupt.h>

void OperationalState::on_do()
{
    command_break = 0;
    analog.init(duty_cycle_first);
    timer_speed.init_speed();
    timer_speed.count_speed  = 0;
    timer_pulses.init_pulses_count();
    analog.pin_digi.set_hi();
    sei();
    
    while(1){
      command_break = Serial.read();
      if(((char)command_break == 'r') || ((char)command_break == 's')){
        break;
      }
      if (fault.is_lo() == 1){
        command_break = 's'; // We give the command as if the stopped button was pressed to do an autonomous emergency stop when FLT raised
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
  if (advanced_control){ // pi controller was chosen
    control.init(Kp, Ti, T);
  }
  else{
    control.init(Kp, 1, 0); // p controller was chosen, when T is 0 the integral term is not used
  }
  on_do();
}

void OperationalState::on_exit() // reset the timers
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
  if (event == 'p') { // Going to PreOperational
    context_->transition_to(new PreOpState());
  }
  if (event == 'r'){ // Reset 
    context_->transition_to(new InitState);
  }
  else if (event == 's'){ // Stopping
    context_->transition_to(new StoppedState);
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

  double u = control.update(ref, (double) current_speed);
  if (u > 99){ u = 99;} //limiting the upper bound of the duty cycle
  if (u < 1){ u = 1;} //limiting the lower bound of the duty cycle
  analog.set(abs(100 - u));

  if(timer_speed.count_speed >= 125){ // prints every 1s
    current_speed = encA.count;
    encA.count = 0;
    timer_speed.count_speed = 0;
    Serial.print("Ref: ");
    Serial.print(ref);
    Serial.print("--------->Current speed: ");
    Serial.print(current_speed);
    Serial.print("------>PWM: ");
    Serial.println(u);
    Serial.println(control.get_sum_error());
    //led.toggle(); // can be used to verify 1s delay of the timer
  }
}

ISR(TIMER1_COMPA_vect){
  analog.pin_digi.set_lo();
}

ISR(TIMER1_COMPB_vect){
  analog.pin_digi.set_hi();
}