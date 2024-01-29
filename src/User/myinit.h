/*
 * myinit.h
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#ifndef SRC_USER_MYINIT_H_
#define SRC_USER_MYINIT_H_

#include <LQ_TFT18.h> //
#include <LQ_STM.H> //
#include <LQ_GTM.H>
#include <LQ_GPIO.h>//

#include <Encoder.h>//
#include "myVariables.h"
#include "myUivariables.h"
#include "PID.h"
#include "myDuty.h"
#include <LQ_UART.h>
#include "LQ_CCU6.h"



//电机频率
#define MOTOR_FREQUENCY    12000

#define LED_0    P20_8
#define LED_1    P20_9

extern volatile int time_run;

void Motor_Init(void);

void Servo_Init(void);

void GPIO_Init(void);

void Encoder_Init(void);
void CCU_Init(void);
void Motor_PID_Init(void);
void Steer_PID_Init(void);
void Imu_Init(void);

void Init_All(void);
void Ui_Init(void);




#endif /* SRC_USER_MYINIT_H_ */
