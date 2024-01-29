/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编    写：龙邱科技
@E-mail  ：chiusir@163.com
@编译IDE：AURIX Development Studio1.4及以上版本
@使用平台：北京龙邱智能科技TC297TX核心板和母板
@SYS  PLL：300MHz*3 【Crystal】 20.000Mhz
@最后更新：2022年02月22日，持续更新，请关注最新版！
@功能介绍：
@相关信息参考下列地址
@网    站：http://www.lqist.cn
@淘宝店铺：http://longqiu.taobao.com
@软件版本：V1.0 版权所有，单位使用请先联系授权
@程序配套视频地址：https://space.bilibili.com/95313236
****************************************************************
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，切换workspace到当前工程所放文件夹！
除了RIF为TC3xxxA独有外，其它的代码兼容TC3xx
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c标准库和底层库文件//////////////////////////////////////////////////
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
/////////////////////龙邱库文件////////////////////////////////////////////
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




