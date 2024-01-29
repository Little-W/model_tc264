/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编    写：龙邱科技
@E-mail  ：chiusir@163.com
@编译IDE：IAR7.8 KEIL5.24及以上版本
@使用平台：北京龙邱智能科技F3277核心板和母板
@SYS  PLL：120MHz 频率太高可能无法启动system_mm32f327x.c
@最后更新：2022年02月22日，持续更新，请关注最新版！
@功能介绍：GPS先管库
@相关信息参考下列地址
@网    站：http://www.lqist.cn
@淘宝店铺：http://longqiu.taobao.com
@
@软件版本：V1.0 版权所有，单位使用请先联系授权
@程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

extern short   gyro[3], accel[3], sensors;
extern char ReadBuff0[128];
_SaveData Save_Data;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void parseGpsBuffer(void);
@功能说明：数据解析
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：将串口收到的数据分门别类的保存到结构体中
@           $GNRMC,130416.000,A,2236.91843,N,11359.19128,E,0.001,306.28,090720,,,A*4F
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void parseGpsBuffer(void)
{
  char *subString;
  char *subStringNext;
  char i = 0;
  if (Save_Data.isGetData)      //如果字符串不为空
  {
    for (i = 0 ; i <= 6 ; i++)  //循环7次
    {
      if (i == 0)
      {
        if ((subString = strchr(Save_Data.GPS_Buffer, ',')) == NULL)  //没有检测到逗号
          while(1);             //解析错误
      }
      else  //检测到逗号，返回逗号的位置
      {
        subString++;    //位置加1（定位到逗号的后一位）
        if ((subStringNext = strchr(subString, ',')) != NULL)//定位下一个逗号的位置
        {
          char usefullBuffer[2];
          switch(i)
          {
          case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);       //两个逗号之间为 时间信息 并转换成北京时间
          Save_Data.UTCTime[1] = Save_Data.UTCTime[1]+ 8;
          if(Save_Data.UTCTime[1] > '9')
          {
            Save_Data.UTCTime[0]++;
            if(Save_Data.UTCTime[0] == '3')
              Save_Data.UTCTime[0] = '0';
            Save_Data.UTCTime[1] = (Save_Data.UTCTime[1] % '9') + '0'-1;
          }
          break;//结束switch
          case 2:memcpy(usefullBuffer,            subString, subStringNext - subString);break; //数据是否有效标志
          case 3:memcpy(Save_Data.latitude,       subString, subStringNext - subString);break;	//获取纬度信息
          case 4:memcpy(Save_Data.N_S,            subString, subStringNext - subString);break;	//获取N/S
          case 5:memcpy(Save_Data.longitude,      subString, subStringNext - subString);break;	//获取经度信息
          case 6:memcpy(Save_Data.E_W,            subString, subStringNext - subString);break;	//获取E/W
          default:break;
          }
          subString = subStringNext;    //下一个逗号位置给第一个指针，
          Save_Data.isParseData = true; //手动给真值，（数据是否解析完成）
          //          if(usefullBuffer[0] == 'A')
          //            Save_Data.isUsefull = true;
          //          else if(usefullBuffer[0] == 'V')
          //            Save_Data.isUsefull = false;
        }
        else
        {
          while(1);	//解析错误
        }
      }
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：char Get_InitData(int* Lon_Z, int* Lon_X, int* Lat_Z, int* Lat_X);
@功能说明：经纬度原始值
@参数说明：Lon_Z（经度整数），Lon_X（经度小数），Lat_Z（纬度整数）,Lat_X（纬度小数）
@函数返回：无
@修改时间：2022/02/24
@备    注：将结构体中的字符串转换成数据，有两种方式，#if 1：以小数点为分界。#if 0：自己手动调节
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

char Get_InitData(int* Lon_Z, int* Lon_X, int* Lat_Z, int* Lat_X)
{
#if 1
  //判断纬度值中是否有‘.'有:说明有数据 例如：3946.99715  变化范围在后六位中变化，最后一位不稳定可舍去。取值为3946和99715
  if((strstr(Save_Data.latitude, ".")) != NULL)
  {
    sscanf(Save_Data.latitude, "%d.%d",&(* Lat_Z), &(* Lat_X));
  }
  else
    return 0;
  //经度原理同上 例如：11628.32198  取值为11628 和 32198
  if((strstr(Save_Data.longitude, ".")) != NULL)
  {
    sscanf(Save_Data.longitude, "%d.%d",&(* Lon_Z), &(* Lon_X));
  }
  else
    return 0;
  return 1;
#else
  unsigned num=0;
  //判断纬度值中是否有‘.'有:说明有数据 例如：3946.99715  变化范围在后六位中变化，最后一位不稳定可舍去。
  if((strstr(Save_Data.latitude, ".")) != NULL)
  {
    *Lat_Z = (Save_Data.latitude[0] - '0')*100 + (Save_Data.latitude[1] - '0')*10 + (Save_Data.latitude[2] - '0');
    num    = (Save_Data.latitude[3] - '0')*100000 + (Save_Data.latitude[5] - '0')*10000 + (Save_Data.latitude[6] - '0')*1000 + \
             (Save_Data.latitude[7] - '0')*100 + (Save_Data.latitude[8] - '0')*10 + (Save_Data.latitude[9] - '0');
    *Lat_X = num;       //取值为394 和 699715
  }
  else
    return 0;
  //经度原理同上 例如：11628.32198
  if((strstr(Save_Data.longitude, ".")) != NULL)
  {
    *Lon_Z = (Save_Data.longitude[0] - '0')*1000 + (Save_Data.longitude[1] - '0')*100 + \
             (Save_Data.longitude[2] - '0')*10 + (Save_Data.longitude[3] - '0');
    num    = (Save_Data.longitude[4] - '0')*100000 + (Save_Data.longitude[6] - '0')*10000 + (Save_Data.longitude[7] - '0')*1000 + \
             (Save_Data.longitude[8] - '0')*100 + (Save_Data.longitude[9] - '0')*10 + (Save_Data.longitude[10] - '0');
    *Lon_X = num;       //取值为1162 和 832198
  }
  else
    return 0;
  return 1;
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Gps_test(void);
@功能说明：GPS测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Gps_test(void)
{
  char txt[30];
  int Lon_IZ=0, Lon_IX=0, Lat_IZ=0, Lat_IX=0;

  TFTSPI_Init(1);        //TFT初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕

  //串口初始化为
  UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
  //灯初始化
  GPIO_LED_Init();

  while(1)
  {
    parseGpsBuffer();

    sprintf(txt, "T:%s",Save_Data.UTCTime);
    TFTSPI_P8X16Str(0,0,txt, u16RED,u16BLACK);          //时间

    sprintf(txt, "N:%s",Save_Data.latitude);            // 纬度原始值
    TFTSPI_P8X16Str(0,1,txt, u16RED,u16BLACK);
    sprintf(txt, "E:%s",Save_Data.longitude);           // 经度原始值
    TFTSPI_P8X16Str(0,2,txt, u16RED,u16BLACK);
    printf("N:%s\nE:%s\n",Save_Data.latitude, Save_Data.longitude);


    if(Get_InitData(&Lon_IZ, &Lon_IX, &Lat_IZ, &Lat_IX) == 0) //获取原始经纬度值
    {
      Lon_IZ=0;Lon_IX=0;Lat_IZ=0;Lat_IX=0;
    }
    sprintf(txt, "N:%d.%d",Lat_IZ, Lat_IX);             // 转化成数据
    TFTSPI_P8X16Str(0,4,txt, u16RED,u16BLACK);
    sprintf(txt, "E:%d.%d",Lon_IZ, Lon_IX);             // 数据
    TFTSPI_P8X16Str(0,5,txt, u16RED,u16BLACK);
    LED_Ctrl(LEDALL,RVS);// LED翻转闪烁
    delayms(200);
  }
}






