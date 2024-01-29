/*
 * myUivariables.h
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#ifndef SRC_USER_MYUIVARIABLES_H_
#define SRC_USER_MYUIVARIABLES_H_

#include <Platform_Types.h>

//PID
extern volatile float Ui_Steer_P;
extern volatile float Ui_Steer_D;
extern volatile float Ui_Motor_P;
extern volatile float Ui_Motor_I;
extern volatile float Ui_Motor_D;
extern volatile float Ui_Motor_dP;
extern volatile float Ui_Motor_dI;
extern volatile float Ui_Motor_dD;
extern volatile uint16 Ui_Motor_Max_Out;
extern volatile uint8 Ui_PID_Error_Thr;
extern volatile uint8 Ui_Decelerate_K;
extern volatile float Ui_Fixed_Diff_K;
extern volatile float Ui_Straight_Diff_K;
extern volatile float Ui_Bend_Diff_K;


//速度
extern volatile short Ui_Max_Speed;
extern volatile short Ui_Min_Speed;
extern volatile short Ui_Inner_Max_Speed;
extern volatile short Ui_Center_Lost_Speed;
extern volatile short Ui_Stop_Speed;





#endif /* SRC_USER_MYUIVARIABLES_H_ */
