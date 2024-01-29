/*
 * Function.c
 *
 *  Created on: 2023年3月25日
 *      Author: user
 */

#include <Function.h>

/*浮点限幅函数*/
float Constrain_Float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}





