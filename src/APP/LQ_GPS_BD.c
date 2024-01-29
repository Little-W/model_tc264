/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��    д������Ƽ�
@E-mail  ��chiusir@163.com
@����IDE��IAR7.8 KEIL5.24�����ϰ汾
@ʹ��ƽ̨�������������ܿƼ�F3277���İ��ĸ��
@SYS  PLL��120MHz Ƶ��̫�߿����޷�����system_mm32f327x.c
@�����£�2022��02��22�գ��������£����ע���°棡
@���ܽ��ܣ�GPS�ȹܿ�
@�����Ϣ�ο����е�ַ
@��    վ��http://www.lqist.cn
@�Ա����̣�http://longqiu.taobao.com
@
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

extern short   gyro[3], accel[3], sensors;
extern char ReadBuff0[128];
_SaveData Save_Data;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void parseGpsBuffer(void);
@����˵�������ݽ���
@����˵������
@�������أ���
@�޸�ʱ�䣺2022/02/24
@��    ע���������յ������ݷ��ű���ı��浽�ṹ����
@           $GNRMC,130416.000,A,2236.91843,N,11359.19128,E,0.001,306.28,090720,,,A*4F
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void parseGpsBuffer(void)
{
  char *subString;
  char *subStringNext;
  char i = 0;
  if (Save_Data.isGetData)      //����ַ�����Ϊ��
  {
    for (i = 0 ; i <= 6 ; i++)  //ѭ��7��
    {
      if (i == 0)
      {
        if ((subString = strchr(Save_Data.GPS_Buffer, ',')) == NULL)  //û�м�⵽����
          while(1);             //��������
      }
      else  //��⵽���ţ����ض��ŵ�λ��
      {
        subString++;    //λ�ü�1����λ�����ŵĺ�һλ��
        if ((subStringNext = strchr(subString, ',')) != NULL)//��λ��һ�����ŵ�λ��
        {
          char usefullBuffer[2];
          switch(i)
          {
          case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);       //��������֮��Ϊ ʱ����Ϣ ��ת���ɱ���ʱ��
          Save_Data.UTCTime[1] = Save_Data.UTCTime[1]+ 8;
          if(Save_Data.UTCTime[1] > '9')
          {
            Save_Data.UTCTime[0]++;
            if(Save_Data.UTCTime[0] == '3')
              Save_Data.UTCTime[0] = '0';
            Save_Data.UTCTime[1] = (Save_Data.UTCTime[1] % '9') + '0'-1;
          }
          break;//����switch
          case 2:memcpy(usefullBuffer,            subString, subStringNext - subString);break; //�����Ƿ���Ч��־
          case 3:memcpy(Save_Data.latitude,       subString, subStringNext - subString);break;	//��ȡγ����Ϣ
          case 4:memcpy(Save_Data.N_S,            subString, subStringNext - subString);break;	//��ȡN/S
          case 5:memcpy(Save_Data.longitude,      subString, subStringNext - subString);break;	//��ȡ������Ϣ
          case 6:memcpy(Save_Data.E_W,            subString, subStringNext - subString);break;	//��ȡE/W
          default:break;
          }
          subString = subStringNext;    //��һ������λ�ø���һ��ָ�룬
          Save_Data.isParseData = true; //�ֶ�����ֵ���������Ƿ������ɣ�
          //          if(usefullBuffer[0] == 'A')
          //            Save_Data.isUsefull = true;
          //          else if(usefullBuffer[0] == 'V')
          //            Save_Data.isUsefull = false;
        }
        else
        {
          while(1);	//��������
        }
      }
    }
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�char Get_InitData(int* Lon_Z, int* Lon_X, int* Lat_Z, int* Lat_X);
@����˵������γ��ԭʼֵ
@����˵����Lon_Z��������������Lon_X������С������Lat_Z��γ��������,Lat_X��γ��С����
@�������أ���
@�޸�ʱ�䣺2022/02/24
@��    ע�����ṹ���е��ַ���ת�������ݣ������ַ�ʽ��#if 1����С����Ϊ�ֽ硣#if 0���Լ��ֶ�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

char Get_InitData(int* Lon_Z, int* Lon_X, int* Lat_Z, int* Lat_X)
{
#if 1
  //�ж�γ��ֵ���Ƿ��С�.'��:˵�������� ���磺3946.99715  �仯��Χ�ں���λ�б仯�����һλ���ȶ�����ȥ��ȡֵΪ3946��99715
  if((strstr(Save_Data.latitude, ".")) != NULL)
  {
    sscanf(Save_Data.latitude, "%d.%d",&(* Lat_Z), &(* Lat_X));
  }
  else
    return 0;
  //����ԭ��ͬ�� ���磺11628.32198  ȡֵΪ11628 �� 32198
  if((strstr(Save_Data.longitude, ".")) != NULL)
  {
    sscanf(Save_Data.longitude, "%d.%d",&(* Lon_Z), &(* Lon_X));
  }
  else
    return 0;
  return 1;
#else
  unsigned num=0;
  //�ж�γ��ֵ���Ƿ��С�.'��:˵�������� ���磺3946.99715  �仯��Χ�ں���λ�б仯�����һλ���ȶ�����ȥ��
  if((strstr(Save_Data.latitude, ".")) != NULL)
  {
    *Lat_Z = (Save_Data.latitude[0] - '0')*100 + (Save_Data.latitude[1] - '0')*10 + (Save_Data.latitude[2] - '0');
    num    = (Save_Data.latitude[3] - '0')*100000 + (Save_Data.latitude[5] - '0')*10000 + (Save_Data.latitude[6] - '0')*1000 + \
             (Save_Data.latitude[7] - '0')*100 + (Save_Data.latitude[8] - '0')*10 + (Save_Data.latitude[9] - '0');
    *Lat_X = num;       //ȡֵΪ394 �� 699715
  }
  else
    return 0;
  //����ԭ��ͬ�� ���磺11628.32198
  if((strstr(Save_Data.longitude, ".")) != NULL)
  {
    *Lon_Z = (Save_Data.longitude[0] - '0')*1000 + (Save_Data.longitude[1] - '0')*100 + \
             (Save_Data.longitude[2] - '0')*10 + (Save_Data.longitude[3] - '0');
    num    = (Save_Data.longitude[4] - '0')*100000 + (Save_Data.longitude[6] - '0')*10000 + (Save_Data.longitude[7] - '0')*1000 + \
             (Save_Data.longitude[8] - '0')*100 + (Save_Data.longitude[9] - '0')*10 + (Save_Data.longitude[10] - '0');
    *Lon_X = num;       //ȡֵΪ1162 �� 832198
  }
  else
    return 0;
  return 1;
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void Gps_test(void);
@����˵����GPS���Ժ���
@����˵������
@�������أ���
@�޸�ʱ�䣺2022/02/24
@��    ע��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Gps_test(void)
{
  char txt[30];
  int Lon_IZ=0, Lon_IX=0, Lat_IZ=0, Lat_IX=0;

  TFTSPI_Init(1);        //TFT��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ

  //���ڳ�ʼ��Ϊ
  UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
  //�Ƴ�ʼ��
  GPIO_LED_Init();

  while(1)
  {
    parseGpsBuffer();

    sprintf(txt, "T:%s",Save_Data.UTCTime);
    TFTSPI_P8X16Str(0,0,txt, u16RED,u16BLACK);          //ʱ��

    sprintf(txt, "N:%s",Save_Data.latitude);            // γ��ԭʼֵ
    TFTSPI_P8X16Str(0,1,txt, u16RED,u16BLACK);
    sprintf(txt, "E:%s",Save_Data.longitude);           // ����ԭʼֵ
    TFTSPI_P8X16Str(0,2,txt, u16RED,u16BLACK);
    printf("N:%s\nE:%s\n",Save_Data.latitude, Save_Data.longitude);


    if(Get_InitData(&Lon_IZ, &Lon_IX, &Lat_IZ, &Lat_IX) == 0) //��ȡԭʼ��γ��ֵ
    {
      Lon_IZ=0;Lon_IX=0;Lat_IZ=0;Lat_IX=0;
    }
    sprintf(txt, "N:%d.%d",Lat_IZ, Lat_IX);             // ת��������
    TFTSPI_P8X16Str(0,4,txt, u16RED,u16BLACK);
    sprintf(txt, "E:%d.%d",Lon_IZ, Lon_IX);             // ����
    TFTSPI_P8X16Str(0,5,txt, u16RED,u16BLACK);
    LED_Ctrl(LEDALL,RVS);// LED��ת��˸
    delayms(200);
  }
}






