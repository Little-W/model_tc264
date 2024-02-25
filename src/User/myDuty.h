/*Set_Motor_Duty
 * myDuty.h
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#ifndef SRC_USER_MYDUTY_H_
#define SRC_USER_MYDUTY_H_


#include <Platform_Types.h>
#include <Function.h>
#include <stdint.h>
#include "myUivariables.h"
#include <LQ_GTM.h>
#include "myInit.h"
#include "myctrl.h"

#define ATOMSERVO1    IfxGtm_ATOM0_1_TOUT31_P33_9_OUT
#define ATOMSERVO2    IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

#define MOTOR2_P      IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
//#define MOTOR2_N      IfxGtm_ATOM0_1_TOUT54_P21_3_OUT
#define MOTOR1_P      IfxGtm_ATOM0_0_TOUT53_P21_2_OUT//IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define MOTOR1_N      IfxGtm_ATOM1_1_TOUT54_P21_3_OUT


extern volatile uint16 CurrentServoDty;//当前的舵机占空比

void Set_Servo_Duty(uint16 duty);
void Set_Motor_Duty(short duty);

#endif /* SRC_USER_MYDUTY_H_ */
