/*
 * myctrl.h
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#ifndef SRC_USER_MYCTRL_H_
#define SRC_USER_MYCTRL_H_

#include "stdlib.h"
#include "Platform_Types.h"
#include "PID.h"
#include "myVariables.h" //原宏文件为Variables.h
#include "myUivariables.h" //原宏文件为Uivariables.h
#include <Function.h>
#include "myDuty.h"

#define SPEED_CTRL_COMMON 1
#define SPEED_CTRL_BRAKE 2
#define BRAKE_ERROR_THRESH 70
#define BRAKE_SPEED_THRESH_MAX 80
#define BRAKE_SPEED_THRESH_MIN 80

float Get_Uk(void);
//void Update_Purpost(void);
//void New_Update_Purpost(void);
void ChangeCita(int cita);

//设置及取消固定速度
void SetToFixedSpeed(short SetFixed_Speed);
void CancelSetToFixedSpeed(void);

void Get_Out(void);
void send_enc_speed(void);



#endif /* SRC_USER_MYCTRL_H_ */
