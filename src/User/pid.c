#include <Function.h>
#include <myVariables.h>
#include <myUivariables.h>
#include "PID.h"

extern volatile _pid_param_t  Steer_pid;


/*
  * @brief    实现位置式PID
  * @param    pid_param : PID参数结构体
  * @param    error：当前误差
  * @return   控制量
  * @note     需要检查限幅是否合适
  */
float Position_PID(volatile _pid_param_t *pid_param, float error, float maxout_pos, float maxout_neg)
{
    pid_param->current_error = error;//目标-实际
    // if(pid_param->integral == 0)
    // {
    //   pid_param->integral = error;
    // }
    // else
    // {
    //   pid_param->integral = 0.01 * error + 0.99 * pid_param->integral;
    // }
    pid_param->integral = 0.01 * error + 0.99 * pid_param->integral;
    //PID系数计算
    pid_param->pid_out_p = pid_param->kp * pid_param->current_error;
    pid_param->pid_out_i = pid_param->ki * pid_param->integral;
    pid_param->pid_out_d = pid_param->kd * (pid_param->current_error - pid_param->last_error);

    //PID输出的计算和限幅
    pid_param->pid_out = pid_param->pid_out_p + pid_param->pid_out_i + pid_param->pid_out_d;
    pid_param->pid_out = Constrain_Float(pid_param->pid_out,-maxout_neg,maxout_pos);
    //误差更新
    pid_param->last_error = pid_param->current_error;
    //急刹车
    
    // if(pid_param->current_error < (-pid_param->error_m))//限幅
    // {
    //     return (-maxout);
    // }
    // else
    // {
    //     return pid_param->pid_out;
    // }
    
    return pid_param->pid_out;
}

/*
  * @brief    实现增量式PID
  * @param    pid_param : PID参数结构体
  * @param    error：当前误差
  * @return   控制量
  * @note     需要检查限幅是否合适
  * @date     2022 4 20
  */
float Incremental_PID(volatile _pid_param_t *pid_param, float error, float maxout_pos, float maxout_neg)
{
    pid_param->current_error = error;//目标-实际
    //PID系数计算
    pid_param->pid_out_p = pid_param->kp * (pid_param->current_error - pid_param->last_error);
    pid_param->pid_out_i = pid_param->ki * pid_param->current_error;
    pid_param->pid_out_d = pid_param->kd * (pid_param->current_error - 2 * pid_param->last_error + pid_param->far_error);

    //PID输出的计算和限幅
    pid_param->pid_out_increment = pid_param->pid_out_p + pid_param->pid_out_i + pid_param->pid_out_d;
    pid_param->pid_out += pid_param->pid_out_increment;
    pid_param->pid_out = Constrain_Float(pid_param->pid_out,-maxout_neg,maxout_pos);
    //误差更新
    pid_param->far_error = pid_param->last_error;
    pid_param->last_error = pid_param->current_error;
    //急刹车
    
    // if(pid_param->current_error < (-pid_param->error_m))//限幅
    // {
    //     return (-maxout);
    // }
    // else
    // {
    //     return pid_param->pid_out;
    // }
    
    return pid_param->pid_out;
}

float Brake_PID(volatile _pid_param_t *pid_param, float error, float maxout)
{
    pid_param->current_error = error;//目标-实际
    //PID系数计算
    pid_param->pid_out_p = pid_param->kp * -672.8393 * pow(pid_param->current_error,-0.3463) + 270.9887;
    pid_param->pid_out_i = pid_param->ki * pid_param->current_error;
    pid_param->pid_out_d = pid_param->kd * (pid_param->current_error - pid_param->last_error);

    //PID输出的计算和限幅
    pid_param->pid_out_increment = pid_param->pid_out_p + pid_param->pid_out_i + pid_param->pid_out_d;
    pid_param->pid_out += pid_param->pid_out_increment;
     pid_param->pid_out += Ui_Motor_Brake_Bias;
    pid_param->pid_out = Constrain_Float(pid_param->pid_out,-maxout,maxout);
    //误差更新
    pid_param->last_error = pid_param->current_error;
    return pid_param->pid_out;
}


/*
  * @brief    实现位置式PID
  * @param    pid_param : PID参数结构体
  * @return   控制量
  * @note     需要检查限幅是否合适
  * @date     2022 4 20
  */
float Servo_PID(volatile _pid_param_t *pid_param, float maxout)
{
    pid_param->current_error = -(float)Deviation;

    //PID系数计算
    pid_param->pid_out_p = pid_param->kp * pid_param->current_error;
    pid_param->pid_out_d = pid_param->kd * (pid_param->current_error - pid_param->last_error);

    //PID输出的计算和限幅
    pid_param->pid_out = pid_param->pid_out_p + pid_param->pid_out_d;
    pid_param->pid_out = Constrain_Float(pid_param->pid_out,-maxout,maxout);

    //误差更新
    pid_param->last_error = pid_param->current_error;

    return pid_param->pid_out;
}

