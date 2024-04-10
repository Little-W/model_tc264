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
#include <string.h>
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
#define POLYNOMIAL 0x07
#define INITIAL_REMAINDER 0x00
// 使用0x07生成的表
const unsigned char crc_table[] = {
    0x00,0x07,0x0e,0x09,0x1c,0x1b,0x12,0x15,0x38,0x3f,0x36,0x31,0x24,0x23,0x2a,0x2d,
    0x70,0x77,0x7e,0x79,0x6c,0x6b,0x62,0x65,0x48,0x4f,0x46,0x41,0x54,0x53,0x5a,0x5d,
    0xe0,0xe7,0xee,0xe9,0xfc,0xfb,0xf2,0xf5,0xd8,0xdf,0xd6,0xd1,0xc4,0xc3,0xca,0xcd,
    0x90,0x97,0x9e,0x99,0x8c,0x8b,0x82,0x85,0xa8,0xaf,0xa6,0xa1,0xb4,0xb3,0xba,0xbd,
    0xc7,0xc0,0xc9,0xce,0xdb,0xdc,0xd5,0xd2,0xff,0xf8,0xf1,0xf6,0xe3,0xe4,0xed,0xea,
    0xb7,0xb0,0xb9,0xbe,0xab,0xac,0xa5,0xa2,0x8f,0x88,0x81,0x86,0x93,0x94,0x9d,0x9a,
    0x27,0x20,0x29,0x2e,0x3b,0x3c,0x35,0x32,0x1f,0x18,0x11,0x16,0x03,0x04,0x0d,0x0a,
    0x57,0x50,0x59,0x5e,0x4b,0x4c,0x45,0x42,0x6f,0x68,0x61,0x66,0x73,0x74,0x7d,0x7a,
    0x89,0x8e,0x87,0x80,0x95,0x92,0x9b,0x9c,0xb1,0xb6,0xbf,0xb8,0xad,0xaa,0xa3,0xa4,
    0xf9,0xfe,0xf7,0xf0,0xe5,0xe2,0xeb,0xec,0xc1,0xc6,0xcf,0xc8,0xdd,0xda,0xd3,0xd4,
    0x69,0x6e,0x67,0x60,0x75,0x72,0x7b,0x7c,0x51,0x56,0x5f,0x58,0x4d,0x4a,0x43,0x44,
    0x19,0x1e,0x17,0x10,0x05,0x02,0x0b,0x0c,0x21,0x26,0x2f,0x28,0x3d,0x3a,0x33,0x34,
    0x4e,0x49,0x40,0x47,0x52,0x55,0x5c,0x5b,0x76,0x71,0x78,0x7f,0x6a,0x6d,0x64,0x63,
    0x3e,0x39,0x30,0x37,0x22,0x25,0x2c,0x2b,0x06,0x01,0x08,0x0f,0x1a,0x1d,0x14,0x13,
    0xae,0xa9,0xa0,0xa7,0xb2,0xb5,0xbc,0xbb,0x96,0x91,0x98,0x9f,0x8a,0x8d,0x84,0x83,
    0xde,0xd9,0xd0,0xd7,0xc2,0xc5,0xcc,0xcb,0xe6,0xe1,0xe8,0xef,0xfa,0xfd,0xf4,0xf3
};

unsigned char crc8(unsigned char* data, size_t length) {
    unsigned char crc = INITIAL_REMAINDER;
    while(length--){
        crc = crc_table[crc ^ *data++];
    }
    return crc;
}

typedef struct {
    uint16_t corrected_data; // 更正后的数据
    uint16_t origin_data;    // 原始接收的数据
    uint16_t error_pos;      // 错误位的位置，如果为0，则表示无错误
} HammingDecodeResult;

HammingDecodeResult hammingDecode(uint16_t code) {
    HammingDecodeResult result;

    uint16_t origin_data = 0;
    origin_data |= (code >> 2) & 0x1; // D1
    origin_data |= (code >> 3) & 0xE; // D2-D4, 二进制的1110等于十六进制的E
    origin_data |= (code >> 4) & 0x7F0; // D5-D11, 二进制的11111110000等于十六进制的7F0
    result.origin_data = origin_data;

    // 计算校验位
    uint8_t p1 = ((code >> 0) & 1) ^ ((code >> 2) & 1) ^ ((code >> 4) & 1) ^ ((code >> 6) & 1) ^ ((code >> 8) & 1) ^ ((code >> 10) & 1) ^ ((code >> 12) & 1) ^ ((code >> 14) & 1);
    uint8_t p2 = ((code >> 1) & 1) ^ ((code >> 2) & 1) ^ ((code >> 5) & 1) ^ ((code >> 6) & 1) ^ ((code >> 9) & 1) ^ ((code >> 10) & 1) ^ ((code >> 13) & 1) ^ ((code >> 14) & 1);
    uint8_t p4 = ((code >> 3) & 1) ^ ((code >> 4) & 1) ^ ((code >> 5) & 1) ^ ((code >> 6) & 1) ^ ((code >> 11) & 1) ^ ((code >> 12) & 1) ^ ((code >> 13) & 1) ^ ((code >> 14) & 1);
    uint8_t p8 = ((code >> 7) & 1) ^ ((code >> 8) & 1) ^ ((code >> 9) & 1) ^ ((code >> 10) & 1) ^ ((code >> 11) & 1) ^ ((code >> 12) & 1) ^ ((code >> 13) & 1) ^ ((code >> 14) & 1);


    // 检测错误位
    uint16_t error_pos = (p8 << 3) | (p4 << 2) | (p2 << 1) | p1;
    result.error_pos = error_pos; // 设置错误位置

    if (error_pos) {
        // 更正错误位
        code ^= (1 << (error_pos - 1));
    }

    // 提取数据位
    uint16_t corrected_data = 0;
    corrected_data |= (code >> 2) & 0x1; // D1
    corrected_data |= (code >> 3) & 0xE; // D2-D4
    corrected_data |= (code >> 4) & 0x7F0; // D5-D11
    result.corrected_data = corrected_data; // 设置更正后的数据

    return result;
}


#define BATCH_TRANS_BEGIN 2
// #define SPEED_TRANS_BEGIN 0xf3
#define SINGEL_TRANS_BEGIN 0
#define TRANS_OVER 1
#define TRANS_SPLIT 3

// #define BATCH_TRANS_BEGIN 0xf8
// // #define SPEED_TRANS_BEGIN 0xf3
// #define SINGEL_TRANS_BEGIN 0xc0
// #define TRANS_OVER 0xff
// #define TRANS_SPLIT 0xc7

void Set_Purpost_Speed(sint16 Purpost_Speed_New)
{
    if(Purpost_Speed_New == 777)
    {
        Speed_cur_mode = SPEED_CTRL_DISABLE_MOTOR;
    }
    else if(abs(Purpost_Speed_New) > 900)
    {
        Speed_cur_mode = SPEED_CTRL_DIRECT_DUTY;
        if(Purpost_Speed_New >= 0)
        {
            Speed_Duty = 100* (Purpost_Speed_New - 900);
        }
        else
        {
            Speed_Duty = 100* (Purpost_Speed_New + 900);
        }
    }
    else
    {
        Speed_cur_mode = SPEED_CTRL_COMMON;
        Smoothed_Purpost_Speed = 0.3 * Smoothed_Purpost_Speed + 0.7 * Purpost_Speed_New;
        Purpost_Speed = Purpost_Speed_New;
    }
}                

void uart_data_decoder(void)
{
    static HammingDecodeResult result;
    static uint8_t crc_code;
    static uint16_t data_code = 0;
    static val_list Purpost_Speed_Rec[20] = {0};
    static val_list Servo_Duty_Rec[20] = {0};
    static boolean servo_trans_begin = FALSE;
    static boolean speed_trans_begin = FALSE;
    static int trans_index = 0;
    static int speed_index_cnt = 0, angle_index_cnt = 0;
    static split_cnt = 0,batch_start_cnt = 0;
    static int data_cnt = 0;
    sint16 Purpost_Speed_New, Servo_Duty_New;
    boolean speed_found_eq = FALSE, angle_found_eq = FALSE;
    boolean angle_need_update = FALSE;
    boolean speed_need_update = FALSE;
    uint8_t cr_data[2] = {0};
    uint8_t ori_data[2] = {0};
    // unsigned char code = 0;
    // unsigned char bit_cnt = 0;

    if(RX_data & 0xc0)
    {
        goto flag_judgement;
    }   

    if(servo_trans_begin)
    {
        switch (trans_index)
        {
        case 2:
            data_code = (RX_data & 0x3f) << 9;
            trans_index ++;
            break;
        case 3:
            data_code |= ((RX_data & 0x3f) << 3);
            trans_index ++;
            break;
        case 4:
            data_code |= ((RX_data & 0x38) >> 3);
            crc_code = ((RX_data & 0x3) << 6);
            result = hammingDecode(data_code);
            trans_index ++;
            break;
        case 5:
            crc_code |= (RX_data & 0x3f);
            if(result.error_pos <= 15)
            {
                ori_data[0] = result.origin_data >> 8;
                ori_data[1] = result.origin_data & 0xff;
                if(crc8(ori_data,2) == crc_code)
                {
                    Servo_Duty = result.origin_data & 0x400 ?
                        Ui_Servo_Mid - (result.origin_data & 0x3ff) * 2 :
                        Ui_Servo_Mid + (result.origin_data & 0x3ff) * 2;
                    angle_index_cnt = 0;
                    memset(Servo_Duty_Rec, 0, sizeof(Servo_Duty_Rec));
                    Set_Servo_Duty(Servo_Duty);   
                }    
                else
                {
                    cr_data[0] = result.corrected_data >> 8;
                    cr_data[1] = result.corrected_data & 0xff;
                    if(crc8(cr_data,2) == crc_code)
                    {
                        Servo_Duty = result.corrected_data & 0x400 ?
                            Ui_Servo_Mid - (result.corrected_data & 0x3ff) * 2 :
                            Ui_Servo_Mid + (result.corrected_data & 0x3ff) * 2;
                        angle_index_cnt = 0;
                        memset(Servo_Duty_Rec, 0, sizeof(Servo_Duty_Rec));
                        Set_Servo_Duty(Servo_Duty);    
                    }
                    else if(Servo_Duty_Rec[0].cnt >= 3)
                    {
                        Servo_Duty = Servo_Duty_Rec[0].val;
                        angle_index_cnt = 0;
                        memset(Servo_Duty_Rec, 0, sizeof(Servo_Duty_Rec));
                        Set_Servo_Duty(Servo_Duty);
                    }
                }
                
                Servo_Duty_New = result.corrected_data & 0x400 ?
                        Ui_Servo_Mid - (result.corrected_data & 0x3ff) * 2 :
                        Ui_Servo_Mid + (result.corrected_data & 0x3ff) * 2;

                if(angle_index_cnt > 0)
                {
                    for(int i = 0; i < angle_index_cnt; i++)
                    {
                        if(Servo_Duty_New == Servo_Duty_Rec[i].val)
                        {
                            Servo_Duty_Rec[i].cnt++;
                            angle_found_eq = TRUE;
                            if(i >= 1)
                            {
                                for(int j = 0; j < i; j++)
                                {
                                    val_list tmp;
                                    if(Servo_Duty_Rec[i].cnt > Servo_Duty_Rec[j].cnt)
                                    {
                                        tmp =  Servo_Duty_Rec[j];
                                        Servo_Duty_Rec[j] = Servo_Duty_Rec[i];
                                        Servo_Duty_Rec[i] = tmp;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if(!angle_found_eq)
                    {
                        Servo_Duty_Rec[angle_index_cnt].val = Servo_Duty_New;
                        Servo_Duty_Rec[angle_index_cnt].cnt = 1;
                        angle_index_cnt++;
                    }
                }
                else
                {
                    Servo_Duty_Rec[0].val = Servo_Duty_New;
                    Servo_Duty_Rec[0].cnt = 1;
                    angle_index_cnt = 1;
                }
            }
            trans_index ++;
            break;
        case 1:
            trans_index ++;
            goto flag_judgement;
        default:
            goto flag_judgement;
            break;
        }
    }
    else if(speed_trans_begin)
    {
        switch (trans_index)
        {
        case 2:
            data_code = (RX_data & 0x3f) << 9;
            trans_index ++;
            break;
        case 3:
            data_code |= ((RX_data & 0x3f) << 3);
            trans_index ++;
            break;
        case 4:
            data_code |= ((RX_data & 0x38) >> 3);
            crc_code = ((RX_data & 0x3) << 6);
            result = hammingDecode(data_code);
            trans_index ++;
            break;
        case 5:
            crc_code |= (RX_data & 0x3f);
            if(result.error_pos <= 15)
            {
                ori_data[0] = result.origin_data >> 8;
                ori_data[1] = result.origin_data & 0xff;
                if(crc8(ori_data,2) == crc_code)
                {
                    Purpost_Speed_New = result.origin_data & 0x400 ? 
                                        - (result.origin_data & 0x3ff) :
                                        (result.origin_data & 0x3ff);
                    speed_index_cnt = 0;
                    memset(Purpost_Speed_Rec, 0, sizeof(Purpost_Speed_Rec));
                    Set_Purpost_Speed(Purpost_Speed_New);
                }                
                else
                {
                    
                    cr_data[0] = result.corrected_data >> 8;
                    cr_data[1] = result.corrected_data & 0xff;

                    if(crc8(cr_data,2) == crc_code)
                    {
                        Purpost_Speed_New = result.corrected_data & 0x400 ? 
                                            - (result.corrected_data & 0x3ff) :
                                            (result.corrected_data & 0x3ff);
                        speed_index_cnt = 0;
                        memset(Purpost_Speed_Rec, 0, sizeof(Purpost_Speed_Rec));
                        Set_Purpost_Speed(Purpost_Speed_New);
                    }
                    else if(Purpost_Speed_Rec[0].cnt >= 4)
                    {
                        Purpost_Speed_New = Purpost_Speed_Rec[0].val;
                        speed_index_cnt = 0;
                        memset(Purpost_Speed_Rec, 0, sizeof(Purpost_Speed_Rec));
                        Set_Purpost_Speed(Purpost_Speed_New);
                    }
                }
                
                Purpost_Speed_New = result.corrected_data & 0x400 ? 
                                        - (result.corrected_data & 0x3ff) :
                                        (result.corrected_data & 0x3ff);

                if(speed_index_cnt > 0)
                {
                    for(int i = 0; i < speed_index_cnt; i++)
                    {
                        if(Purpost_Speed_New == Purpost_Speed_Rec[i].val)
                        {
                            Purpost_Speed_Rec[i].cnt++;
                            speed_found_eq = TRUE;
                            if(i >= 1)
                            {
                                for(int j = 0; j < i; j++)
                                {
                                    val_list tmp;
                                    if( Purpost_Speed_Rec[i].cnt > Purpost_Speed_Rec[j].cnt)
                                    {
                                        tmp =  Purpost_Speed_Rec[j];
                                        Purpost_Speed_Rec[j] = Purpost_Speed_Rec[i];
                                        Purpost_Speed_Rec[i] = tmp;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    if(!speed_found_eq)
                    {
                        Purpost_Speed_Rec[speed_index_cnt].val = Purpost_Speed_New;
                        Purpost_Speed_Rec[speed_index_cnt].cnt = 1;
                        speed_index_cnt++;
                    }
                }
                else
                {
                    Purpost_Speed_Rec[0].val = Purpost_Speed_New;
                    Purpost_Speed_Rec[0].cnt = 1;
                    speed_index_cnt = 1;
                }
            }
            trans_index ++;
            break;
        case 1:
            trans_index ++;
            goto flag_judgement;
        default:
            goto flag_judgement;
            break;  
        } 
    }
    else
    {
        goto flag_judgement;
    }

    return;

flag_judgement:
    if(RX_data & 0xc0)
    {
        uint8_t con_code = 0,cnt = 0;
        for(int i = 0; i <= 2; i++)
        {
            if(RX_data & (1 << i))
            {
                cnt++;
            }
        }
        if(cnt >= 2)
        {
            con_code |= 1;
        }
        cnt = 0;
        for(int i = 3; i <= 5; i++)
        {
            if(RX_data & (1 << i))
            {
                cnt++;
            }
        }
        if(cnt >= 2)
        {
            con_code |= 2;
        }
        switch (con_code)
        {
        case SINGEL_TRANS_BEGIN:
            // servo_trans_begin = TRUE;
            // speed_trans_begin = FALSE;
            time_out_cnt = 0;
            batch_start_cnt = 0;
            split_cnt = 0;
            trans_index = 2;
            data_cnt++;
            break;
        // case SPEED_TRANS_BEGIN:
        //     servo_trans_begin = FALSE;
        //     speed_trans_begin = TRUE;
        //     time_out_cnt = 0;
        //     batch_start_cnt = 0;
        //     split_cnt = 0;
        //     trans_index = 2;
        //     break;
        // case TRANS_OVER:
        //     trans_index = 1;
        //     break;
        case BATCH_TRANS_BEGIN:
            batch_start_cnt ++;
            if(batch_start_cnt >= 3)
            {
                servo_trans_begin = TRUE;
                speed_trans_begin = FALSE;
                trans_index = 0;
                batch_start_cnt = 0;
                data_cnt = 0;
            }
            break;
        case TRANS_SPLIT:
            split_cnt ++;
            if(split_cnt >= 3)
            {
                servo_trans_begin = FALSE;
                speed_trans_begin = TRUE;
                trans_index = 0;
                split_cnt = 0;
                data_cnt = 0;
            }
            break;
        default:
            // if(trans_index == 5)
            // {
            //     split_cnt = 0;
            //     trans_index = 2;
            // }
            break;
        }  
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
    //        unsigned char receiveBuff[USB_FRAME_LENMAX];//存放数据寄存器
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
    //            unsigned char check = 0;
    //            unsigned char length = USB_FRAME_LENMIN;
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
    RX_data = UART_GetChar(UART2);
    uart_data_decoder();
    /* 用户代码 */
    // UART_PutChar(UART2, UART_GetChar(UART2));
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
