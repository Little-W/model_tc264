/*
 * myUivariables.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */


#include <myUivariables.h>

//PID参数
volatile float  Ui_Steer_P = 2.5f;                   //舵机P
volatile float  Ui_Steer_D = 0.033f;                  //舵机D

//加速电机
volatile float  Ui_Motor_P = 30.0f;                 //电机P参数 3.0f 2 10 15.0 18.0 25
volatile float  Ui_Motor_I = 0.8f; //0.8f;                  //电机I参数 0.38f 0.5 0.48 0.48 0.37 0.42
volatile float  Ui_Motor_D = 0.0f;//0.02f;                 //电机D参数 0.0f
//减速电机
volatile float  Ui_Motor_dP = 1.6f;                 //电机P参数 4.0f
volatile float  Ui_Motor_dI = 0.53f;                  //电机I参数 0.4f
volatile float  Ui_Motor_dD = 0.25f;                 //电机D参数 0.0f

volatile float  Ui_Motor_Brake_P = 20.0f;                 //刹车P参数
volatile float  Ui_Motor_Brake_I = 0.0f;                  //刹车I参数
volatile float  Ui_Motor_Brake_D = 0.0f;                 //刹车D参数
volatile uint16 Ui_Motor_Brake_Max_Out = 10000;         
volatile uint16 Ui_Motor_Brake_Bias = 8000; 

volatile uint16 Ui_Motor_Max_Out_Pos = 7000;            //电机PID输出限幅
volatile uint16 Ui_Motor_Max_Out_Neg = 8500;            //电机PID输出限幅

volatile uint8  Ui_PID_Error_Thr = 105;               //对误差阈值（使用bang - bang）
volatile uint8  Ui_Decelerate_K = 6;                 //用于图像看到弯道提前减速

volatile float  Ui_Fixed_Diff_K = 0;// 0.025f;             //(极小，后期需加大)固定速度的减速比（单位：MotorDuty / ServoDuty）
volatile float  Ui_Straight_Diff_K = 0;// 0.033f;          //(极小，后期需加大)直道差速时减速比（单位：MotorDuty / ServoDuty）
volatile float  Ui_Bend_Diff_K = 0;// 0.028f;              //(极小，后期需加大)弯道差速时减速比（单位：MotorDuty / ServoDuty）

//速度
volatile short  Ui_Max_Speed = 110;                  //正常跑时最大速度（单位：EncSpeed）
volatile short  Ui_Min_Speed = 90;                   //正常跑时最小速度（单位：EncSpeed）
volatile short  Ui_Inner_Max_Speed = 80;             //让内轮速度更低  （单位：EncSpeed）
volatile short  Ui_Center_Lost_Speed = 50;           //丢中线时用的速度（单位：EncSpeed）
volatile short  Ui_Stop_Speed = 30;                  //进入车库速度    （单位：EncSpeed）


volatile boolean  send_speed = FALSE;  