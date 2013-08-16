/******************************************************************************/
/****本程序将采集到的电压电流值进行校准转化，输出真实电压电流值，方便之后计算*************/
/******************************************************************************/

#ifndef DSP_ADC_C                  
#define DSP_ADC_C

//-----------------------------------------------------------------------------
// Adding Include Stuff

#include "DSP281x_Device.h"
#include "GlobalParameter.h"
#include "DSP_Adc.h"

#define CPU_CLOCK_SPEED      6.6667L   // for a 150MHz CPU clock speed
#define ADC_usDELAY 		 5000L
#define ADC_usDELAY1 		 20L

//////////////DelayUs初始化程序/////////////////////////////////////
//extern void DELAY_US(volatile Uint16 Usec)
//{ 
//   while (Usec--)
//  {
//    asm(" RPT#149||NOP");
//
//  }
//}
//////////////DelayUs初始化程序/////////////////////////////////////
//-------------------------------------------------------------------
void sequence(int16 in[],int16 out[],int n)
{
   int i=0;
   int j=0;
      
   for(i=0;i<n;i++)
   {
      for(j=i+1;j<=n;j++) 
      {
         if(in[i]>in[j]) //如果a[i]比a[j]大，则交换数据，前面的总是保存较小的数据
           {
            out[i]=in[j]; 
            out[j]=in[i];
           }
		 else
		   {
            out[i]=in[i]; 
            out[j]=in[j];
           }
      }
   } 
}
//-------------------------------------------------------------------
void adc_read(ADCVALSB *p)
{   
   	int16 Sum[8];
    _iq Va_temp,Vb_temp,Ia_temp,Ib_temp,Ic_temp,Vdc_temp,Isc_temp; 
    

    sequence(p->Ch0,p->Orh0,p->Num);
	sequence(p->Ch1,p->Orh1,p->Num);
	sequence(p->Ch2,p->Orh2,p->Num);
	sequence(p->Ch3,p->Orh3,p->Num);
	sequence(p->Ch4,p->Orh4,p->Num);
	sequence(p->Ch5,p->Orh5,p->Num);
	sequence(p->Ch6,p->Orh6,p->Num);
	sequence(p->Ch7,p->Orh7,p->Num);     
    Sum[0]=p->Orh0[2]+p->Orh0[3]+p->Orh0[4];
	Sum[1]=p->Orh1[2]+p->Orh1[3]+p->Orh1[4];
	Sum[2]=p->Orh2[2]+p->Orh2[3]+p->Orh2[4];
	Sum[3]=p->Orh3[2]+p->Orh3[3]+p->Orh3[4];
	Sum[4]=p->Orh4[2]+p->Orh4[3]+p->Orh4[4];
	Sum[5]=p->Orh5[2]+p->Orh5[3]+p->Orh5[4];
	Sum[6]=p->Orh6[2]+p->Orh6[3]+p->Orh6[4];
	Sum[7]=p->Orh7[2]+p->Orh7[3]+p->Orh7[4];	  

/*
 Sum[0]= p->Ch0[0]+p->Ch0[1]+p->Ch0[2]+p->Ch0[3]+p->Ch0[4]+p->Ch0[5]+p->Ch0[6];
 Sum[1]= p->Ch1[0]+p->Ch1[1]+p->Ch1[2]+p->Ch1[3]+p->Ch1[4]+p->Ch1[5]+p->Ch1[6];
 Sum[2]= p->Ch2[0]+p->Ch2[1]+p->Ch2[2]+p->Ch2[3]+p->Ch2[4]+p->Ch2[5]+p->Ch2[6];
 Sum[3]= p->Ch3[0]+p->Ch3[1]+p->Ch3[2]+p->Ch3[3]+p->Ch3[4]+p->Ch3[5]+p->Ch3[6];
 Sum[4]= p->Ch4[0]+p->Ch4[1]+p->Ch4[2]+p->Ch4[3]+p->Ch4[4]+p->Ch4[5]+p->Ch4[6];
 Sum[5]= p->Ch5[0]+p->Ch5[1]+p->Ch5[2]+p->Ch5[3]+p->Ch5[4]+p->Ch5[5]+p->Ch5[6];
 Sum[6]= p->Ch6[0]+p->Ch6[1]+p->Ch6[2]+p->Ch6[3]+p->Ch6[4]+p->Ch6[5]+p->Ch6[6];
 Sum[7]= p->Ch7[0]+p->Ch7[1]+p->Ch7[2]+p->Ch7[3]+p->Ch7[4]+p->Ch7[5]+p->Ch7[6];
*/


 //线电压转换成相电压，电流转换成电流(从int16格式转换成IQ格式)，直流电压值转换成直流电压值；
    Va_temp=_IQmpy(_IQ(TWO_THIRD),_Q15toIQ(Sum[0]))+_IQmpy(_IQ(ONE_THIRD),_Q15toIQ(Sum[1]));//未放大时的A相电压值；
	Vb_temp=_IQmpy(_IQ(ONE_THIRD),_Q15toIQ(Sum[1]))-_IQmpy(_IQ(ONE_THIRD),_Q15toIQ(Sum[0]));//未放大时的B相电压值；
	Ia_temp=_Q15toIQ(Sum[2]);                //IQ格式的未放大的A相电流值
	Ib_temp=_Q15toIQ(Sum[3]);                //IQ格式的未放大的B相电流值
	Ic_temp=_Q15toIQ(Sum[4]);                //IQ格式的未放大的B相电流值
	Vdc_temp=_Q15toIQ(Sum[5]);               //IQ格式的未放大的直流侧电压值
	Isc_temp=_Q15toIQ(Sum[6]);

	//将传感器采到的电压电流值还原为真实值
    p->Va=_IQmpy(_IQ(103.6307),Va_temp);
	p->Vb=_IQmpy(_IQ(102.8823),Vb_temp);
	p->Ia=_IQmpy(_IQ(1),Ia_temp);
	p->Ib=_IQmpy(_IQ(11.2),Ib_temp);
	p->Ic=_IQmpy(_IQ(15.57),Ic_temp);
	p->Vdc=_IQmpy(_IQ(594),Vdc_temp);
	p->Isc=_IQmpy(_IQ(2.66),Isc_temp);
}
//-------------------------------------------------------------------
#endif //DSP_ADC_C
