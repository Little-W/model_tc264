/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
ASC异步串行通信，可以作为UART和汽车LIN总线使用，
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_UART.h>
#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>
#include "myVariables.h"

#define ASC_TX_BUFFER_SIZE 64        //发送缓冲区长度
#define ASC_RX_BUFFER_SIZE 64        //接收缓冲区长度
#define speed_t 100

//串口通信结构体
IfxAsclin_Asc g_UartConfig[4];

static uint8 s_AscTxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 s_AscRxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/* UART中断 */
IFX_INTERRUPT(UART0_RX_IRQHandler, UART0_VECTABNUM, UART0_RX_PRIORITY);
IFX_INTERRUPT(UART1_RX_IRQHandler, UART1_VECTABNUM, UART1_RX_PRIORITY);
IFX_INTERRUPT(UART2_RX_IRQHandler, UART2_VECTABNUM, UART2_RX_PRIORITY);
IFX_INTERRUPT(UART3_RX_IRQHandler, UART3_VECTABNUM, UART3_RX_PRIORITY);
IFX_INTERRUPT(UART0_TX_IRQHandler, UART0_VECTABNUM, UART0_TX_PRIORITY);
IFX_INTERRUPT(UART1_TX_IRQHandler, UART1_VECTABNUM, UART1_TX_PRIORITY);
IFX_INTERRUPT(UART2_TX_IRQHandler, UART2_VECTABNUM, UART2_TX_PRIORITY);
IFX_INTERRUPT(UART3_TX_IRQHandler, UART3_VECTABNUM, UART3_TX_PRIORITY);
IFX_INTERRUPT(UART0_ER_IRQHandler, UART0_VECTABNUM, UART0_ER_PRIORITY);
IFX_INTERRUPT(UART1_ER_IRQHandler, UART1_VECTABNUM, UART1_ER_PRIORITY);
IFX_INTERRUPT(UART2_ER_IRQHandler, UART2_VECTABNUM, UART2_ER_PRIORITY);
IFX_INTERRUPT(UART3_ER_IRQHandler, UART3_VECTABNUM, UART3_ER_PRIORITY);


/** UART中断CPU标号 */
const uint8 UartIrqVectabNum[4] = {UART0_VECTABNUM, UART1_VECTABNUM, UART2_VECTABNUM, UART3_VECTABNUM};

/** UART中断优先级 */
const uint8 UartIrqPriority[12] = {UART0_RX_PRIORITY, UART0_TX_PRIORITY, UART0_ER_PRIORITY, UART1_RX_PRIORITY, UART1_TX_PRIORITY, UART1_ER_PRIORITY,
                                   UART2_RX_PRIORITY, UART2_TX_PRIORITY, UART2_ER_PRIORITY, UART3_RX_PRIORITY, UART3_TX_PRIORITY, UART3_ER_PRIORITY};

/** UART中断服务函数地址 */
const void *UartIrqFuncPointer[12] = {&UART0_RX_IRQHandler, &UART0_TX_IRQHandler, &UART0_ER_IRQHandler,
                                       &UART1_RX_IRQHandler, &UART1_TX_IRQHandler, &UART1_ER_IRQHandler,
                                       &UART2_RX_IRQHandler, &UART2_TX_IRQHandler, &UART2_ER_IRQHandler,
                                       &UART3_RX_IRQHandler, &UART3_TX_IRQHandler, &UART3_ER_IRQHandler,};

/*
    传输开始标志: 0x55 即 0b01010101
    传输结束标志: 0x6a 即 0b01101010
*/
void uart_data_decoder(void)
{
    unsigned char data,flag_code;
    static unsigned char last_servo_data;
    static boolean servo_p1_received = FALSE;
    static boolean trans_begin = FALSE;
    static sint16 Purpost_Speed_New;
    static uint16 Servo_Duty_New;  

    flag_code = RX_data & 0xc0;
    switch (flag_code)
    {
    case 0x00:
        data = RX_data & 0x1f;
        Purpost_Speed_New = data * 22; // +- 0 ~ 682
        if(RX_data & 0x20)
        {
            Purpost_Speed_New = -Purpost_Speed_New;
        }
        break;
    case 0x40:
        if(RX_data == 0x55)
            trans_begin = TRUE;
        else if(RX_data == 0x6a) //检测到传输结束，更新数据
        {
            if(trans_begin)
            {
                trans_begin = FALSE;
        if(Purpost_Speed_New == 0 || (Purpost_Speed_New < 0 && Purpost_Speed > 0) || (Purpost_Speed_New > 0 && Purpost_Speed < 0))
                {
                    Purpost_Speed = Purpost_Speed_New;
                }
                else
                {        
                    // 防止速度变化过大，感觉没啥用
                    if(Purpost_Speed_New - Purpost_Speed > 500)
                    {
                        Purpost_Speed_New = Purpost_Speed + 500;
                    }
                    else if(Purpost_Speed_New - Purpost_Speed < -500)
                    {
                        Purpost_Speed_New = Purpost_Speed - 500;
                    }
                    Purpost_Speed = 0.3 * Purpost_Speed_New + 0.7 * Purpost_Speed;
                }
                if(Servo_Duty_New == Ui_Servo_Mid || (Servo_Duty_New < Ui_Servo_Mid && Servo_Duty > Ui_Servo_Mid) || (Servo_Duty_New > Ui_Servo_Mid && Servo_Duty < Ui_Servo_Mid))
                {
                    Servo_Duty = Servo_Duty_New;
                }
                else
                {
                    Servo_Duty = 0.8 * Servo_Duty_New + 0.2 * Servo_Duty;
                }
            }
        }
        break;
    case 0x80:
        last_servo_data = RX_data & 0x3f;
        servo_p1_received = TRUE;
        break;
    case 0xc0:
        if(servo_p1_received)
        {
            data = RX_data & 0x3f;
            Servo_Duty_New = last_servo_data & 0x20 ?
                    Ui_Servo_Mid - (((last_servo_data & 0x1f) << 6) + data) :
                    Ui_Servo_Mid + (((last_servo_data & 0x1f) << 6) + data);
        }
        servo_p1_received = FALSE;
        break;
    }
}
/*************************************************************************
*  函数名称：void UART0_RX_IRQHandler(void)
*  功能说明：UART0_RX_IRQHandler中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：
*************************************************************************/
void UART0_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[0]);

        /* 用户代码 */
         //if(!Get_Out_Flag)
        RX_data = UART_GetChar(UART0);
        uart_data_decoder();
}

void UART0_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[0]);

    /* 用户代码 */
}

void UART0_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[0]);

    /* 用户代码 */

}

void UART1_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[1]);

    /* 用户代码 */
     //if(!Get_Out_Flag)

    RX_data = UART_GetChar(UART1);

    uart_data_decoder();

            // //UART_PutChar(UART0, UART_GetChar(UART0));
            // //UART_PutChar(UART0, RX_data[1]);
            // //LED_test = RX_data[0];

            /**
            @author reverie
            @date 23/3/24
            @brief
                接受数据：参考gitee中CarDo与EB通信协议

                第一字节：帧头，统一为0x42
                第二字节：地址位，用于判别不同类型的数据，0x01代表小车控制
                第三字节：帧长，表示此次发送数据的总长度
                then     数据位
                末字节：  校验位
            */
    //        uint8_t receiveBuff[USB_FRAME_LENMAX];//存放数据寄存器
    //        RX_data = UART_GetChar(UART0);      //临时接受一个字节
    //
    //
    //        if(RX_data == USB_FRAME_HEAD && !UART_Rev)//监测帧头
    //        {
    //            UART_Rev+=1;
    //            receiveBuff[0] = RX_data;
    //            receiveBuff[2] = USB_FRAME_LENMIN;
    //        }
    //        else if(UART_Rev == 2)  //接收帧长度
    //        {
    //            receiveBuff[UART_Rev] = RX_data;
    //            UART_Rev++;
    //
    //            if(RX_data > USB_FRAME_LENMAX || RX_data < USB_FRAME_LENMIN) //帧长错误
    //            {
    //                receiveBuff[2] = USB_FRAME_LENMIN;
    //                UART_Rev = 0;
    //            }
    //        }
    //        else if(UART_Rev && UART_Rev < USB_FRAME_LENMAX)
    //        {
    //            receiveBuff[UART_Rev] = RX_data;
    //            UART_Rev++;
    //        }
    //
    //        //接收帧完毕
    //        if((UART_Rev >= USB_FRAME_LENMAX || UART_Rev >= receiveBuff[2]) && UART_Rev > USB_FRAME_LENMIN)
    //        {
    //            uint8_t check = 0;
    //            uint8_t length = USB_FRAME_LENMIN;
    //
    //            length = receiveBuff[2];
    //            for(int i=0;i<length-1;i++)
    //                check += receiveBuff[i];
    //
    //            if(check == receiveBuff[length-1])//校验数据正确性
    //            {
    //                if(0x01  == receiveBuff[1])//判断为智能车控制指令
    //                {
    //                    Bint16_Union bint16_Union;
    //                    Bint32_Union bint32_Union;
    //                    for(int i=0;i<4;i++)
    //                        bint32_Union.U8_Buff[i] = receiveBuff[3+i];
    //
    //                    bint16_Union.U8_Buff[0] = receiveBuff[7];
    //                    bint16_Union.U8_Buff[1] = receiveBuff[8];
    //                    Set_Servo_Duty(500);
    //                    delayms(3000);
    //                    for(int i=0;i<4;i++){
    //                                UART_PutChar(UART0,bint32_Union.U8_Buff[i]);
    //                            }
    //                    for(int i=0;i<2;i++){
    //                                UART_PutChar(UART0,bint16_Union.U8_Buff[i]);
    //                            }
    //
    //                    speed = bint32_Union.Float;
    //                    servoPwm = bint16_Union.U16;
    //                    Set_Servo_Duty(300);
    ////                  Purpost_Speed = speed*30/3.14/0.03;
    //                }
    //                if(0x04 == receiveBuff[1]){
    ////                    PIN_Write(P20_3,1);
    //
    //                }
    //            }
    //
    //            UART_Rev = 0;
    //        }
    //
    //        /*
    //            适配接收到的数据float speed和uint16 servoPwm
    //        */
//    UART_PutChar(UART1, UART_GetChar(UART1));
}

void UART1_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART1_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[1]);

    /* 用户代码 */
}

void UART2_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[2]);

    /* 用户代码 */
    UART_PutChar(UART2, UART_GetChar(UART2));
}

void UART2_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART2_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[2]);

    /* 用户代码 */
}

void UART3_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&g_UartConfig[3]);

    /* 用户代码 */
    UART_PutChar(UART3, UART_GetChar(UART3));
}

void UART3_TX_IRQHandler(void)
{
    IfxAsclin_Asc_isrTransmit(&g_UartConfig[3]);

    /* 用户代码 */
}

void UART3_ER_IRQHandler(void)
{
    IfxAsclin_Asc_isrError(&g_UartConfig[3]);

    /* 用户代码 */
}

/*************************************************************************
*  函数名称：void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate)
*  功能说明：串口模块初始化
*  参数说明：
  * @param    RxPin   ： 串口接收管脚
  * @param    TxPin   ： 串口发送管脚
  * @param    baudrate： 波特率
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);   //初始化串口0 波特率 115200 无奇偶校验 1停止位 使用管脚P14_0 P14_1
*************************************************************************/
void UART_InitConfig(UART_RX_t RxPin, UART_TX_t TxPin, unsigned long baudrate)
{
    int i,j;
    //关闭CPU中断
    IfxCpu_disableInterrupts();

    Ifx_P *portRx = PIN_GetModule(RxPin);
    uint8 pinIndexRx = PIN_GetIndex(RxPin);

    Ifx_P *portTx = PIN_GetModule(TxPin);
    uint8 pinIndexTx = PIN_GetIndex(TxPin);

    IfxAsclin_Rx_In  * IfxAsclin_Rx = NULL_PTR;
    IfxAsclin_Tx_Out * IfxAsclin_Tx = NULL_PTR;

    for( i = 0; i < IFXASCLIN_PINMAP_NUM_MODULES; i++)
    {
        for( j = 0; j < IFXASCLIN_PINMAP_RX_IN_NUM_ITEMS; j++)
        {
            if(IfxAsclin_Rx_In_pinTable[i][j] == NULL_PTR)
            {

            }
            else if((unsigned long)portRx == (unsigned long)(IfxAsclin_Rx_In_pinTable[i][j]->pin.port) && pinIndexRx == IfxAsclin_Rx_In_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Rx = IfxAsclin_Rx_In_pinTable[i][j];
            }
        }

        for(j = 0; j < IFXASCLIN_PINMAP_TX_OUT_NUM_ITEMS; j++)
        {
            if(IfxAsclin_Tx_Out_pinTable[i][j] == NULL_PTR)
            {

            }
            else if((unsigned long)portTx == (unsigned long)(IfxAsclin_Tx_Out_pinTable[i][j]->pin.port) && pinIndexTx == IfxAsclin_Tx_Out_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Tx = IfxAsclin_Tx_Out_pinTable[i][j];
            }
        }
    }

    if(IfxAsclin_Rx->module != IfxAsclin_Tx->module)
    {
#pragma warning 557         // 屏蔽警告
        while (1);          // 检查 RX和TX 是否为同一UART
#pragma warning default     // 打开警告
    }

    //新建串口配置结构体
    IfxAsclin_Asc_Config ascConfig;

    //初始化模块
    IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);

    ascConfig.baudrate.baudrate  = (float)baudrate;        //波特率
    ascConfig.frame.frameMode    = IfxAsclin_FrameMode_asc;//数据帧模式
    ascConfig.frame.dataLength   = IfxAsclin_DataLength_8; //数据长度
    ascConfig.frame.stopBit      = IfxAsclin_StopBit_1;    //停止位
    ascConfig.frame.shiftDir     = IfxAsclin_ShiftDirection_lsbFirst;//移位方向
    ascConfig.frame.parityBit    = FALSE;//无奇偶校验

    unsigned char uartNum = IfxAsclin_getIndex(IfxAsclin_Tx->module);

    //中断优先级配置
    ascConfig.interrupt.rxPriority = UartIrqPriority[uartNum * 3];
    ascConfig.interrupt.txPriority = UartIrqPriority[uartNum * 3 + 1];
    ascConfig.interrupt.erPriority = UartIrqPriority[uartNum * 3 + 2];
    ascConfig.interrupt.typeOfService = UartIrqVectabNum[uartNum];

    //接收和发送FIFO的配置
    ascConfig.txBuffer     = &s_AscTxBuffer[uartNum][0];
    ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
    ascConfig.rxBuffer     = &s_AscRxBuffer[uartNum][0];
    ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

    IfxAsclin_Asc_Pins pins =
    {
        NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
        IfxAsclin_Rx,             IfxPort_InputMode_pullUp,        /* Rx pin */
        NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
        IfxAsclin_Tx,             IfxPort_OutputMode_pushPull,     /* Tx pin */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    //调用上面结构体所预设的参数，完成模块的初始化
    IfxAsclin_Asc_initModule(&g_UartConfig[uartNum], &ascConfig);

    //接收，发送和错误中断配置
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3],     UartIrqPriority[uartNum * 3]);
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 1], UartIrqPriority[uartNum * 3 + 1]);
    IfxCpu_Irq_installInterruptHandler((void*)UartIrqFuncPointer[uartNum * 3 + 2], UartIrqPriority[uartNum * 3 + 2]);

    //开启CPU中断
    IfxCpu_enableInterrupts();
}

/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART_PutChar(UART_t  uratn, char ch)
{
    IfxAsclin_Asc_blockingWrite(&g_UartConfig[uratn], ch);
}

/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *str)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART_PutStr(UART_t  uratn, char *str)
{
    int i=0;
   while(*str)
    {
        UART_PutChar(uratn, *str++);
        i++;
        if(i==2820)
        {
            delayms(100);
        }
    }
}
void UART_esp8266int(void)
{
    LED_Ctrl(LED0,ON);
    UART_PutStr(UART0,"AT\n");
    delayms(1000);
    UART_PutStr(UART0,"AT+CWMODE=3\n");
    delayms(1000);
    UART_PutStr(UART0, "AT+CWJAP=\"iPhone\",\"2019060401010\"\n");
    delayms(10000);
    UART_PutStr(UART0, "AT+CIPMUX=0\n");
    delayms(3000);
    UART_PutStr(UART0, "AT+CIPMODE=1\n");
    delayms(3000);
    UART_PutStr(UART0, "AT+CIPSTART=\"TCP\",\"172.20.10.5\",6000\n");
    delayms(10000);
    UART_PutStr(UART0, "AT+CIPSEND\n");
    delayms(1000);
    UART_PutStr(UART0, "READY");
    LED_Ctrl(LEDALL,ON);
    while(1)
    {
        UART_PutStr(UART0,"ready\n");
        delayms(2000);
    }
}
/*************************************************************************
*  函数名称：void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：
* @param    uratn ： UART0 - UART3
* @param    buff  ： 要打印的字符串地址
* @param    len   ： 要打印的长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutBuff(UART4, "123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’
*************************************************************************/
void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len)
{
    while(len--)
    {
        UART_PutChar(uratn, *buff);
        buff++;
    }
}

/*************************************************************************
*  函数名称：Ifx_SizeT UART_GetCount(UART_t  uratn)
*  功能说明：获取 接收缓冲区 存放当前接收数据个数
*  参数说明： uratn ： UART0 - UART3
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
uint8 UART_GetCount(UART_t  uratn)
{
    return (uint8)IfxAsclin_Asc_getReadCount(&g_UartConfig[uratn]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      uartn           串口模块号(UART_0,UART_1,UART_2,UART_3)
//  @param      *dat            接收数据的地址
//  @return     uint8           1：接收成功   0：未接收到数据
//  Sample usage:               uint8 dat; uart_query(UART_0,&dat);       // 接收串口0数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UART_t uratn, uint8 *dat)
{
    if(IfxAsclin_Asc_getReadCount(&g_UartConfig[uratn])>0)
    {
        *dat = IfxAsclin_Asc_blockingRead(&g_UartConfig[uratn]);
        return 1;
    }
    return 0;
}


/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART_GetChar(UART_t  uratn)
{
    uint8 data=0;
    Ifx_SizeT count = 1;

    /* 获取接收到的一个字节数据 注意如果没有接收到，会一直等待 */
    IfxAsclin_Asc_read(&g_UartConfig[uratn], &data, &count, TIME_INFINITE);

    return  data;
}

/*************************************************************************
*  函数名称：char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len)
*  功能说明：接收一定长度字符 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会接受失败
*  参数说明：uratn ： UART0 - UART3
*  函数返回：0:读取正常  其他 读取失败
*  修改时间：2020年3月10日
*  备    注：ART_GetChar(UART0, data, 10); //接收10个字符
*************************************************************************/
char UART_GetBuff(UART_t  uratn, unsigned char *data, unsigned char len)
{
    Ifx_SizeT count = len;

    if(UART_GetCount(uratn) < len)
    {
        return 1;  //判断当前接收到的数据数量 不够读取长度
    }

    /* 获取接收到数据  */
    IfxAsclin_Asc_read(&g_UartConfig[uratn], data, &count, TIME_INFINITE);
    return  0;
}








/////////////////////////////////////////////////////////////////////////////////////
