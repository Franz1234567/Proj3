#include "global.h"

double error = 0;
double u = 0;

PI_Controller::PI_Controller(double Kp, double Ti, double T) : P_controller(Kp){
    Ti_priv = Ti;
    T_priv = T;
    Kp_priv = Kp;
    sum_error = 0;
}

double PI_Controller::update(double ref, double actual){
    error = ref - actual;
    sum_error += error * T_priv;
    u =  Kp_priv * error + (Kp_priv/Ti_priv) * sum_error;
    if (u >= max_speed){
        sum_error -= error * T_priv;
    }
    return -1;
}

double get_sum_error(){
    sum_error = -1;
    return sum_error;
}