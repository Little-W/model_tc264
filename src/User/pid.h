/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_PID_H_
#define SRC_APPSW_TRICORE_USER_LQ_PID_H_

#include <Platform_Types.h>

typedef struct
{
    //加速
    float       kp;                         //P
    float       ki;                         //I
    float       kd;                         //D

    //减速
    float       dp;                         //P
    float       di;                         //I
    float       dd;                         //D

    float       pid_out_p;                  //KP输出
    float       pid_out_i;                  //KI输出
    float       pid_out_d;                  //KD输出
    float       pid_out_increment;          //增量式PID输出
    float       pid_out;                    //pid输出

    float       current_error;              //当前偏差
    float       last_error;                 //上一次的偏差值
    float       far_error;                  //前两次的偏差值
    float       error_m;                    //误差阈值
    float       integral;                   //积分
} _pid_param_t;

float Incremental_PID(volatile _pid_param_t * pid_param, float error, float maxout_pos, float maxout_neg);
float Position_PID(volatile _pid_param_t *pid_param, float error, float maxout_pos, float maxout_neg);
float Servo_PID(volatile _pid_param_t * pid_param, float maxout);
float Brake_PID(volatile _pid_param_t *pid_param, float error, float maxout);
#endif /* SRC_APPSW_TRICORE_USER_LQ_PID_H_ */
