//程序调试

#include <include.h>//各个模块的头文件
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <LQ_UART.h>
#include <LQ_CCU6.h>
#include <myCtrl.h>
#include <Main.h>
#include "PID.h"
#include "myDuty.h"
#include <Encoder.h>
#include <Function.h>
#include <myInit.h>
#include <myUivariables.h>
#include <myVariables.h>

App_Cpu0 g_AppCpu0; // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
int core0_main()
{
    //short Speed=2000; //库函数模板最大110，最小90
    //int Speed_test, Speed_pur;
    unsigned char uart_send_data_1,uart_send_data_2;
    unsigned short speed_tmp;
    Servo_Duty = 4896; //舵机产品书maikuan
    // 关闭CPU总中断
    IfxCpu_disableInterrupts();

    // 关闭看门狗，如果不设置看门狗喂狗需要关闭
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    //IIC_Init();=
    //STM_InitConfig(STM0, STM_Channel_0, 1000000); //50000
    //mpu6050_dmp_init();

    // 串口P14.0管脚输出,P14.1输入，波特率115200
    UART_InitConfig(UART1_RX_P11_10,UART1_TX_P11_12, 115200); //引脚待定
    UART_InitConfig(UART2_RX_P10_6,UART2_TX_P10_5, 115200); //引脚待定
//
//    //Ui_Init();
//    //Init_All();
//
    ATOM_PWM_InitConfig(IfxGtm_ATOM2_4_TOUT34_P33_12_OUT, 5000, MOTOR_FREQUENCY);
//    ATOM_PWM_SetDuty(IfxGtm_ATOM1_1_TOUT47_P22_0_OUT, 0, MOTOR_FREQUENCY);
    Motor_Init();
    Motor_PID_Init();

    Servo_Init();
    Steer_PID_Init();
    Encoder_Init();
    GPIO_Init(); //LED初始化利用imu值点灯
//    //Read_DMP(&imu[0], &imu[1], &imu[2]); //读取imu初值
    CCU_Init();
    RX_data = 0;
//    Purpost_Speed = 3000;
//    //delayms(2000);
//    //Get_Out_Flag = 0;
//    ATOM_PWM_InitConfig(IfxGtm_ATOM1_1_TOUT54_P21_3_OUT, 0, 12000);
//    ATOM_PWM_InitConfig(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 0, 12000);
    UART_PutStr(UART1,"Hello World\n");
    UART_PutStr(UART2,"Hello World\n");
    while (1)
    {
//        UART_PutChar(UART1,'A');
//          if(countTime <3){
//              Set_Servo_Duty(900);
//              Set_Motor_Duty(1750);

          ATOM_PWM_SetDuty(IfxGtm_ATOM2_4_TOUT34_P33_12_OUT, 5000, MOTOR_FREQUENCY);
          if(Enc_Val < 0)
          {
              speed_tmp = - Enc_Val;
              uart_send_data_1 = 0x80;
              uart_send_data_1 |= speed_tmp >> 8;
              uart_send_data_2 = speed_tmp & 0xff;
          }
          else
          {
              speed_tmp = Enc_Val;
              uart_send_data_1 = 0x00;
              uart_send_data_1 |= speed_tmp >> 8;
              uart_send_data_2 = speed_tmp & 0xff;
          }
          UART_PutChar(UART1,uart_send_data_1);
          UART_PutChar(UART1,uart_send_data_2);
          delayms(50);
//        ATOM_PWM_SetDuty(IfxGtm_ATOM1_1_TOUT54_P21_3_OUT,3000,12000);
//        ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT,0,12000);
//        Set_Motor_Duty(3000);
//        UART_PutChar(UART0,'a');
//Set_Servo_Duty(1000);
//PIN_Write(P20_3,0);

//Set_Servo_Duty(750);
//delayms(1000);
//Set_Servo_Duty(500);
//PIN_Write(P20_3,1);
    }
}


