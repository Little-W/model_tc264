/*
 * myctrl.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */


#include <myctrl.h>

int Fixed_Purpost_Speed = 0;                   //固定速度标志位

extern volatile _pid_param_t  Speed_pid;

/*
void Update_Purpost(void)
{

    //记得做线性变换

    Purpost_Speed = RX_data[0];
    Servo_Duty =  2 * RX_data[1] + 500;
    if(RX_data[0] == 0 && RX_data[1] == 0)
    {
        Servo_Duty = 750;
        Purpost_Speed = 0;
    }
}
*/



//改变舵机
void ChangeCita(int cita)
{
    Servo_Duty = cita; //线性映射
    Set_Servo_Duty(Servo_Duty);
}

//设置固定速度
void SetToFixedSpeed(short SetFixed_Speed)
{
    Purpost_Speed = SetFixed_Speed;
    Fixed_Purpost_Speed = 1;
}


//取消固定速度
void CancelSetToFixedSpeed(void)
{
    Fixed_Purpost_Speed = 0;
}


//获得车轮控制量
float Get_Uk(sint16 Ek)
{
    return Incremental_PID(&Speed_pid, Ek, Ui_Motor_Max_Out);
}

//小车出库
void Get_Out(void)
{
    Deviation = 60; //出库角度，待定
    Servo_Duty = Ui_Servo_Mid - Servo_PID(&Steer_pid, Ui_Servo_Interval);
    Set_Servo_Duty(Servo_Duty);
}

//电量计函数
void power_warning(void)
{

}

