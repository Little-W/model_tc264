/*
 * myVariables.h
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#ifndef SRC_USER_MYVARIABLES_H_
#define SRC_USER_MYVARIABLES_H_

#include "Platform_Types.h"
#include "PID.h"
#include "myctrl.h"
//全局变量
#define Ui_Stop_Flag 1                //停车标志位
#define Ui_Servo_Mid 4720                //舵机中值对应的数值占空比
#define Ui_Servo_Interval 1200          //舵机左右转动的阈值
/****************PID结构体******************/
extern volatile _pid_param_t  Speed_pid;          //电机控制PID
extern volatile _pid_param_t  Speed_brake_pid;
extern volatile _pid_param_t  Steer_pid;           //舵机控制PID

/****************电机、舵机、编码器******************/
extern volatile short Speed_Duty;               //电机设置占空比

extern volatile unsigned char Speed_cur_mode;

extern volatile uint16 countTime;

extern volatile uint16 lastDuty;

extern volatile uint16 Servo_Duty;                //舵机设置占空比

extern volatile sint16 Enc_Val;                  //编码器获得轮速度

extern volatile uint16 count;

extern int Deviation;

extern volatile float Enc_Total;                   //里程

extern volatile sint16 Purpost_Speed;               //目标速度    （以EncSpeed为单位）

extern volatile unsigned char RX_data;                     //串口接收数据
extern volatile int LED_test;

extern volatile uint8 Get_Out_Flag;                //出库标志
extern volatile uint8 UART_Rev;                //UART接收标志

//uart接受标志
extern volatile uint8 UART_Flag1;
extern volatile uint8 UART_Flag2;

extern volatile uint16 Get_Out_Time;                //出库时间 ms

extern volatile int Get_Out_Speed;             //出库速度 EncSpeed

extern volatile float speed;                        //接受到的速度结果 m/s
extern volatile uint16 servoPwm;                    //接受到的方向结果

extern volatile uint16 time_out_cnt;                      //串口未接收到数据超时计数

typedef struct val_list
{
    sint16 val;
    unsigned char cnt;
} val_list;

#endif /* SRC_USER_MYVARIABLES_H_ */
