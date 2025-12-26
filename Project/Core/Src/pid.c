#define MOTOR_DEADZONE 20
float DT = 0.010f;

typedef struct { float set_point; //should be zero 
float kp; float ki; float kd; float CoT; float prev_e_t; float integral; } PID;


float pid_controller (PID *pid, float dt, float PV){ //process variable   
    float e_t = pid->set_point - PV; //error
    pid->integral += e_t*dt;

    float d = 0;
    if(dt>0){
    d = pid->kd*((e_t - pid->prev_e_t )/dt);}
    float p = pid->kp * e_t;
    pid->prev_e_t = e_t;
    pid->CoT = p + d + (pid->integral*pid->ki);

    

    if(pid->CoT > 500)
        pid->CoT = 500;
    else if (pid->CoT < -500)
        pid->CoT = -500;

    return pid->CoT;
}

void speed_motor(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t s) {
    if (s > 500) s = 500;
    __HAL_TIM_SET_COMPARE(htim, channel, s);
}