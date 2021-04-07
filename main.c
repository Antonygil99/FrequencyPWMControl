#include "project.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "E:\Documentos\INTERSHIP\PSoC\PSoC\library\Oled-Psoc5-master\OLED.cydsn\ssd1306.h"

#define  DISPLAY_ADDRESS 0x3c

#define NDecimas 1000 

//.......global variables.....
//............................
uint8 i = 0;        //interruption flag
uint16 output[2]; //ADC gotten value
uint16 freq = 0;  //Varable for the frequency mapping
float64 realfreq = 0;

long delaytime = 0;//measures the trigger time

uint8 selection = 0;
//..............................
//.....M A P F U N C T I O N....
//..............................
long map(long x, long in_min, long in_max, long out_min, long out_max);

//..............................
//.....interruotion dimmer......
//.............................

CY_ISR_PROTO(angle_set);
CY_ISR_PROTO(select_set);

//..............................
//.....ADC FREQUENCY LECT......
//.............................

void adc_frequency();
void adc_triggerLect();

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_triggerAngle_StartEx(angle_set);
    isr_sele_StartEx(select_set);
    isr_sele_ClearPending();
    isr_triggerAngle_ClearPending();    
    PWM_1_Start();
    
    
    OPAMP_Start();
    ADC_Start();
    AMux_Start();
    I2COLED_Start();
    display_init(DISPLAY_ADDRESS);
   
       display_clear();    
       display_update();    
       gfx_setTextSize(2);
       gfx_setTextColor(WHITE);
       gfx_setCursor(20,10);
       gfx_println("Press SW");
       display_update();
    CyDelay(1000);
    
    
    for(;;)
    {
       
       
       if(selection == 1)
       {
         adc_frequency();
       }
       else if(selection == 0)
       {
        adc_triggerLect();
       }
    
       if(selection > 1)
    {
        selection = 0;
    }
      
       
        
       
    }
}



CY_ISR(angle_set)
{
    CyDelayUs(delaytime);
    dimmerout_Write(1);
    CyDelayUs(10);
    dimmerout_Write(0);
    
    triggerAngle_ClearInterrupt();
    
}

CY_ISR(select_set)
{
    selection++;
}

void adc_frequency()
{
    AMux_FastSelect(0);
        ADC_StartConvert();
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        output[0] = ADC_GetResult16();
        ADC_StopConvert();
            
            freq = map(output[0],0,1024,132,1332);
            realfreq = 91312*powf(freq,-0.997);
             /*Saturate ADC result to positive numbers. */
            if(output[0] <= 0)
            {
                output[0] = 0;
            }
       Clock_SetDividerRegister(freq,1);//sets the devider register from 1132(70Hz) to 132(700Hz) 
       //modifies the period of both PWM1 and PWM2
      // uint16 devregclock = Clock_GetDividerRegister();//gets the divider register value
        
       //this part of the code corresponds to the OLED display I2C module 
       //which has some bugs those which I am Working yet.
       char data[100];
       //sprintf(data,"%.2f",realfreq);
        
    
   
 
        int Entero=realfreq;
        unsigned int Decimal =(int)(realfreq*NDecimas)%NDecimas;
 
        sprintf(data,"%u,%02u",Entero,Decimal);
        //sprintf(data,"%i",Clock_GetDividerRegister());
       display_clear();    
       display_update();    
       gfx_setTextSize(2);
       gfx_setTextColor(WHITE);
       gfx_setCursor(5,20);
       gfx_println(data);
       gfx_setTextSize(2);
       gfx_setTextColor(WHITE);
       gfx_setCursor(90,20);
       gfx_println("Hz");
       display_update();
}

void adc_triggerLect()
{
    char data2[20];
    
    AMux_FastSelect(1);
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    output[1] = ADC_GetResult16();
    ADC_StopConvert();
    
    delaytime = map(output[1],0,1024,0,8000);
    sprintf(data2,"%ld",delaytime);
       display_clear();    
       display_update();    
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(0,0);
       gfx_println(data2);
       
       display_update();
    
}

  
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    long divisor = (in_max - in_min);
    if(divisor == 0){
        return -1; //AVR returns -1, SAM returns 0
    }
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
}

/* [] END OF FILE */
