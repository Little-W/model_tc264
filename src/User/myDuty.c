/*
 * myDuty.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#include <myDuty.h>

volatile uint16 CurrentServoDty;

/*
* @brief    设置舵机占空比
* @param    duty ：占空比*ATOM_PWM_MAX(10000.0f) （duty为数值占空比）
* @note     舵机中值 1.53ms高电平的PWM波    限幅在 1.03ms - 2.03ms之间
*           中值 duty = 0.48 * ATOM_PWM_MAX = 4896
*           限幅 1ms：0.32 * ATOM_PWM_MAX = 3296
*           限幅 2ms：0.64 * ATOM_PWM_MAX = 6496
*/
inline void Set_Servo_Duty(uint16 duty)
{
    duty = Constrain_Float((float)duty,(float)(Ui_Servo_Mid - Ui_Servo_Interval), (float)(Ui_Servo_Mid + Ui_Servo_Interval));
    CurrentServoDty = duty;
    ATOM_PWM_SetDuty(ATOMSERVO1, duty, 320);
}

/**
  * @brief   设置电机占空比
  * @param   duty ：占空比 * ATOM_PWM_MAX(10000.0f) （duty为数值占空比）
  * @note    num为0 修改右电机占空比；  num为1 修改左电机占空比
  * @date    2022.4.20
  */

inline void Set_Motor_Duty(short duty)
{
    if(duty >= 0)
    {
        ATOM_PWM_SetDuty(MOTOR1_P, duty, MOTOR_FREQUENCY);
        //IfxPort_setPinLow(&MODULE_P21, 2);
        ATOM_PWM_SetDuty(MOTOR1_N, 0, MOTOR_FREQUENCY);
    }
    else
    {
        ATOM_PWM_SetDuty(MOTOR1_P, 0, MOTOR_FREQUENCY);
        //IfxPort_setPinHigh(&MODULE_P21, 2);
        ATOM_PWM_SetDuty(MOTOR1_N, -duty, MOTOR_FREQUENCY);
    }
}


