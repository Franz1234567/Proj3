#include <timer_msec.h>
#include <encoder.h>
#include <digital_out.h>
#include <avr/interrupt.h>
#include <p_controller.h>
#include <analog_out.h>
#include <util/delay.h>
#include <Arduino.h>

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

int  main(){
    Serial.begin(9600);
    led.init();
    analog.init(duty_cycle_first);

    timer_speed.init_speed();
    timer_speed.count_speed  = 0;

    timer_pulses.init_pulses_count();

    analog.pin_digi.set_hi();
    sei();
    
    while(1){}
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

  // if (encA.count == 0){ //checking if the motor is running 
  //   timer_speed.count_speed = 0;
  //   speed_63_found = 0;
  // }

  // if((speed_63_ref <= abs(encA.count)) && (speed_63_found == 0)){
  //   int speed_63 = encA.count;
  //   float time_cst = timer_speed.count_speed * 0.008; // 0.008s each 125 clock cycle (see report for more details)
  //   speed_63_found = 1;

  //   Serial.print("----------Speed 63%: ");
  //   Serial.print(speed_63);
  //   Serial.print("pps\n");

  //   Serial.print("----------Time cst: ");
  //   Serial.print(time_cst);
  //   Serial.print("s\n");
  // }

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
