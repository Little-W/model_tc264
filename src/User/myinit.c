/*
 * myinit.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */




#include <myInit.h>

volatile int time_run =0;//10ms++
extern volatile _pid_param_t  Speed_pid;          //电机控制PID
extern volatile _pid_param_t  Steer_pid;           //舵机控制PID



/*电机初始化*/
void Motor_Init(void)
{
    ATOM_PWM_InitConfig(MOTOR1_P, 0, MOTOR_FREQUENCY);
    ATOM_PWM_InitConfig(MOTOR1_N, 0, MOTOR_FREQUENCY);
}


/*舵机初始化*/
void Servo_Init(void)
{
    ATOM_PWM_InitConfig(ATOMSERVO1, Ui_Servo_Mid,320);
}


/*GPIO管脚初始化*/
void GPIO_Init(void)
{
    //蜂鸣器
    PIN_InitConfig(P20_3,PIN_MODE_OUTPUT,0);

    //CW2015电源电压模块
    //PIN_InitConfig(P13_3,PIN_MODE_OUTPUT,1);

    //LED初始化
    PIN_InitConfig(LED_0,PIN_MODE_OUTPUT, 0);
    PIN_InitConfig(LED_1,PIN_MODE_OUTPUT, 1);
}


/*编码器初始化*/
void Encoder_Init(void)
{
    ENC_InitConfig(ENC2_InPut_P33_7,ENC2_Dir_P33_6);
}


/*PID参数初始化*/
void Motor_PID_Init(void)
{
    Speed_pid.kp                = Ui_Motor_P;
    Speed_pid.ki                = Ui_Motor_I;
    Speed_pid.kd                = Ui_Motor_D;
    Speed_pid.dp                = Ui_Motor_dP;
    Speed_pid.di                = Ui_Motor_dI;
    Speed_pid.dd                = Ui_Motor_dD;
    Speed_pid.pid_out_p         = 0;
    Speed_pid.pid_out_i         = 0;
    Speed_pid.pid_out_d         = 0;
    Speed_pid.pid_out_increment = 0;
    Speed_pid.pid_out           = 0;
    Speed_pid.current_error     = 0;
    Speed_pid.last_error        = 0;
    Speed_pid.far_error         = 0;
    Speed_pid.error_m           = Ui_PID_Error_Thr;

}

void Steer_PID_Init(void)
{
    Steer_pid.kp                = Ui_Steer_P;
    Steer_pid.ki                = 0;
    Steer_pid.kd                = Ui_Steer_D;
    Steer_pid.pid_out_p         = 0;
    Steer_pid.pid_out_i         = 0;
    Steer_pid.pid_out_d         = 0;
    Steer_pid.pid_out_increment = 0;
    Steer_pid.pid_out           = 0;
    Steer_pid.current_error     = 0;
    Steer_pid.last_error        = 0;
    Steer_pid.far_error         = 0;
    Steer_pid.error_m           = Ui_PID_Error_Thr;
}


/*对所有模块进行初始化*/
//void Init_All(void)
//{
//    STM_InitConfig(STM0, STM_Channel_0, 50000);
//    Motor_Init();
//    Servo_Init();
//    Encoder_Init();
//    Ui_Init();
//    UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 19200);
//    CCU_Init();
//    GPIO_Init();
//    //Imu_Init();
//}

void CCU_Init(void)
{
    CCU6_InitConfig(CCU60, CCU6_Channel0, 10*1000);  //10ms进行一次中断
    CCU6_EnableInterrupt(CCU60,CCU6_Channel0);

    CCU6_InitConfig(CCU61, CCU6_Channel0, 7*1000); //7ms进行一次中断
    CCU6_EnableInterrupt(CCU61,CCU6_Channel0);

//    CCU6_InitConfig(CCU61, CCU6_Channel1, 500*1000); //7ms进行一次中断
//    CCU6_EnableInterrupt(CCU61,CCU6_Channel1);
}

void Ui_Init(void)
{
    Motor_PID_Init();
    Steer_PID_Init();
}
