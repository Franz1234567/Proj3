#include "global.h"


void PreOpState::on_entry(){
	Serial.println("Entering Pre Op State");
  led.set_hi();
  Serial.println("Type Kp, press enter then type Ti, press enter");
  on_do();
	
}

void PreOpState::on_exit(){
	Serial.println("Exiting Pre Op State");
}

void PreOpState::on_do(){
    bool kp_received = false;
    bool ti_received = false;
    String Kp_str;
    String Ti_str;

    unsigned long previousMillis = 0;       // Stores last time LED was updated
    const long interval = 500;              // Interval for LED toggle (500 ms)

    while(1){
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) { // every 500 ms
          previousMillis = currentMillis;
          led.toggle();
      }

      if (Serial.available() > 0) {
        char receivedChar = Serial.read();  // Read the next available character
        
        if (receivedChar == 'r' || receivedChar == 'o' || receivedChar == 's') {
            break;
        }

        // Handle input for Kp
        if (!kp_received) {
          if (receivedChar == '\n') {  // Newline indicates input is complete
            if (Kp_str.length() > 0) {
              Kp = Kp_str.toDouble();
              kp_received = true;
              Serial.print("Kp received: ");
              Serial.println(Kp, 5);
              Kp_str = "";
            }
          }
          else{
            Kp_str += receivedChar; 
          }
        }

        // Handle input for Ti
        else if (!ti_received) {
          if (receivedChar == '\n') {  // Newline indicates input is complete
              if (Ti_str.length() > 0) {
                  Ti = Ti_str.toDouble(); 
                  ti_received = true;
                  Serial.print("Ti received: ");
                  Serial.println(Ti, 5);
                  Ti_str = ""; 
              }
          }
        else {
          Ti_str += receivedChar;  // Add received character to the Ti string
        }
      }
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