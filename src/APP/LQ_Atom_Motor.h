/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_MT_H_
#define __LQ_MT_H_

#define ATOMSERVO1       IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2       IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

//#define ATOMPWM0Pin         IfxGtm_ATOM0_2_TOUT107_P10_5_OUT //����PWM����ܽţ�ͨ��P10.5���PWM�������ƺ���
#define ATOMPWM0         IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define ATOMPWM1         IfxGtm_ATOM0_5_TOUT40_P32_4_OUT
#define ATOMPWM2         IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define ATOMPWM3         IfxGtm_ATOM0_4_TOUT50_P22_3_OUT
#define ATOMPWM4         IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
#define ATOMPWM5         IfxGtm_ATOM0_1_TOUT54_P21_3_OUT
#define ATOMPWM6         IfxGtm_ATOM0_7_TOUT64_P20_8_OUT
#define ATOMPWM7         IfxGtm_ATOM0_3_TOUT56_P21_5_OUT

void LQ_Atom_Motor_8chPWM(void);
void LQ_ATom_Servo_2chPWM(void);
void LQ_Atom_8chPWM(void);

#endif
