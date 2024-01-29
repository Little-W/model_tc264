/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��    д������Ƽ�
@E-mail  ��chiusir@163.com
@����IDE��AURIX Development Studio1.4�����ϰ汾
@ʹ��ƽ̨�������������ܿƼ�TC297TX���İ��ĸ��
@SYS  PLL��300MHz*3 ��Crystal�� 20.000Mhz
@�����£�2022��02��22�գ��������£����ע���°棡
@���ܽ��ܣ�
@�����Ϣ�ο����е�ַ
@��    վ��http://www.lqist.cn
@�Ա����̣�http://longqiu.taobao.com
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@����������Ƶ��ַ��https://space.bilibili.com/95313236
****************************************************************
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·�����л�workspace����ǰ���������ļ��У�
����RIFΪTC3xxxA�����⣬�����Ĵ������TC3xx
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c��׼��͵ײ���ļ�//////////////////////////////////////////////////
#include "Platform_Types.h"
#include <CompilerTasking.h>
#include <IfxQspi_PinMap.h>
#include <IfxQspi_SpiMaster.h>
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "../User/Encoder.h"
#include "../User/LQ_CCU6.h"
#include "../User/pid.h"
/////////////////////������ļ�////////////////////////////////////////////
/**************APP*******************/
#include "LQ_Atom_Motor.h"
#include "LQ_CAMERA.h"
#include "LQ_CCU6_Timer.h"
#include "LQ_EEPROM_TEST.h"
#include "LQ_FFT_TEST.h"
#include "LQ_GPIO_ExInt.h"
#include "LQ_GPIO_KEY.h"
#include "LQ_GPIO_LED.h"
#include "LQ_GPT_mini512.h"
#include "LQ_I2C_9AX.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_I2C_VL53.h"
#include "LQ_ICM20602.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_MT9V034.h"
#include "LQ_OLED096.h"
#include "LQ_STM_Timer.h"
#include "LQ_TFT18.h"
#include "LQ_TFT2.h"
#include "LQ_Tim_InputCature.h"
#include "LQ_Tom_Servo.h"
#include "LQ_UART_Bluetooth.h"

/**************Drv*******************/
#include "LQ_ADC.h"
#include "LQ_DMA.h"
#include "LQ_EEPROM.h"
#include "LQ_GPIO.h"
#include "LQ_GPSR.h"
#include "LQ_GTM.h"
#include "LQ_QSPI.h"
#include "LQ_SOFTI2C.h"
#include "LQ_SPI.h"
#include "LQ_STM.h"
#include "LQ_UART.h"

/**************User******************/
#include "LQ_GPS_BD.h"

////////////////////////////////////////////////////////////////////////////
typedef struct
{
    float32 sysFreq;  //Actual SPB frequency
    float32 cpuFreq;  //Actual CPU frequency
    float32 pllFreq;  //Actual PLL frequency
    float32 stmFreq;  //Actual STM frequency
} AppInfo;

// Application information
typedef struct
{
    AppInfo info;     //Info object
} App_Cpu0;

#endif




