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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_CCU6.h>



 /*************************************************************************
 void LQ_CCU6_Timer (void)
 *  功能说明：测试程序
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  备    注：核心板上的LED灯闪烁，中断时P10.5/P10.6闪灯
 *************************************************************************/
 void LQ_CCU6_Timer (void)
  {
     CCU6_InitConfig(CCU60, CCU6_Channel0, 200000);//CCU6初始化
     CCU6_InitConfig(CCU61, CCU6_Channel0, 400000);//CCU6初始化
     CCU6_InitConfig(CCU60, CCU6_Channel1, 500000);//CCU6初始化
     CCU6_InitConfig(CCU61, CCU6_Channel1, 1000000);//CCU6初始化
#pragma warning 557         // 屏蔽警告
	//中断服务函数中翻转LED
  	while(1)
  	{
    	//LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
//		delayms(500);              //延时等待
  	}
  }
