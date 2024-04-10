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
float Get_Uk(void)
{
    sint16 Ek = Purpost_Speed - Enc_Val;
    // if(Purpost_Speed == 0)
    // {
    //     Speed_cur_mode = SPEED_CTRL_BRAKE;
    //     return Brake_PID(&Speed_brake_pid, abs(Ek) * 10, Ui_Motor_Brake_Max_Out);
    // }
    // if((Ek < -BRAKE_ERROR_THRESH && Enc_Val > BRAKE_SPEED_THRESH_MIN && Enc_Val < BRAKE_SPEED_THRESH_MAX))
    // {
    //     Speed_cur_mode = SPEED_CTRL_BRAKE;
    //     return Brake_PID(&Speed_brake_pid, -Ek, Ui_Motor_Brake_Max_Out);
    // }
    // else 
    // {
        // Speed_cur_mode = SPEED_CTRL_COMMON;
        return Incremental_PID(&Speed_pid, Purpost_Speed - Enc_Val , Ui_Motor_Max_Out_Pos, Ui_Motor_Max_Out_Neg);
    // }
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
void send_enc_speed(void)
{
    unsigned char uart_send_data_1,uart_send_data_2;
    unsigned short speed_tmp;
    uart_send_data_1 = 0x40;
    uart_send_data_2 = 0xc0;
    if(Enc_Val < 0)
    {
        speed_tmp = (unsigned short)(- Enc_Val);
        uart_send_data_1 |= 0x20;
        uart_send_data_1 |= (speed_tmp >> 6) & 0x1f;
        uart_send_data_2 |= speed_tmp & 0x3f;
    }
    else
    {
        speed_tmp = Enc_Val;
        uart_send_data_1 |= (speed_tmp >> 6) & 0x1f;
        uart_send_data_2 |= speed_tmp & 0x3f;
    }
    for(int i = 0; i < TRANS_COUNT; i++)
    {
        // UART_PutChar(UART2,uart_send_data_1);
        UART_PutChar(UART1,uart_send_data_1);
        // UART_PutChar(UART2,uart_send_data_2);
        UART_PutChar(UART1,uart_send_data_2);
    }

}
