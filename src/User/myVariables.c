/*
 * myVariables.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#include <myVariables.h>
#include "myctrl.h"

//电机频率
#define MOTOR_FREQUENCY    12000


/****************PID结构体******************/
volatile _pid_param_t  Speed_pid;          //电机控制PID

volatile _pid_param_t  Speed_brake_pid;          //电机刹车控制PID

volatile _pid_param_t  Steer_pid;           //舵机控制PID

/****************电机、舵机、编码器******************/
volatile short Speed_Duty;               //电机设置占空比

volatile unsigned char Speed_cur_mode;

volatile uint16 countTime = 0;

volatile uint16 Servo_Duty;                //舵机设置占空比

volatile uint16 lastDuty = 750;

volatile uint16 count = 0;

volatile sint16 Enc_Val;                  //编码器获得车轮速度

volatile float Enc_Total;                   //里程

volatile sint16 Purpost_Speed;               //目标速度    （以EncSpeed为单位）

volatile unsigned char RX_data;                     //串口接收数据
volatile uint8 UART_Rev = 0;                        //UART接收标志
volatile int LED_test;

volatile uint8 Get_Out_Flag = 0;                //出库标志

//uart标志
volatile uint8 UART_Flag1 = 0;
volatile uint8 UART_Flag2 = 0;

volatile uint16 Get_Out_Time = 2000;                //出库时间 ms

volatile int Get_Out_Speed = 100;             //出库速度 EncSpeed

//舵机角度
int Deviation = 0;
volatile float speed = 0;                        //接受到的速度结果 m/s
volatile uint16 servoPwm = 0;                    //接受到的方向结果

volatile uint16 time_out_cnt = 0;                      //串口未接收到数据超时计数
