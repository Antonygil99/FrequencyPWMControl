/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "C:\Users\hp\Documents\Intership\PSoC\library\Oled-Psoc5-master\OLED.cydsn\ssd1306.h"

#define  DISPLAY_ADDRESS 0x3c

//.......global variables.....
//............................
uint8 i = 0;        //interruption flag
uint16 output[2]; //ADC gotten value
uint16 freq = 0;  //Varable for the frequency mapping
uint16 mapfreq = 0;

long delaytime = 0;//measures the trigger time


//..............................
//.....M A P F U N C T I O N....
//..............................
long map(long x, long in_min, long in_max, long out_min, long out_max);

//..............................
//.....interruotion dimmer......
//.............................

CY_ISR_PROTO(angle_set);

//..............................
//.....ADC FREQUENCY LECT......
//.............................

void adc_frequency();
void adc_triggerLect();



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_triggerAngle_StartEx(angle_set);
    isr_triggerAngle_ClearPending();    
    PWM_1_Start();
    
    
    OPAMP_Start();
    ADC_Start();
    AMux_Start();
    I2COLED_Start();
    display_init(DISPLAY_ADDRESS);
   
    for(;;)
    {
       adc_frequency();
       adc_triggerLect();
        
       
    }
}



CY_ISR(angle_set)
{
    CyDelay(delaytime);
    dimmerout_Write(1);
    CyDelayUs(10);
    dimmerout_Write(0);
    
    triggerAngle_ClearInterrupt();
    
}

void adc_frequency()
{
    AMux_FastSelect(0);
        ADC_StartConvert();
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        output[0] = ADC_GetResult8();
        ADC_StopConvert();
            
            freq = map(output[0],0,127,132,1332);
            mapfreq = map(output[0],0,127,700,70);
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
       char data[50];
       sprintf(data,"%u",mapfreq);
       display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(10,10);
       gfx_println(data);
       gfx_setTextSize(2);
       gfx_setTextColor(WHITE);
       gfx_setCursor(65,20);
       gfx_println("Hz");
       display_update();
}

void adc_triggerLect()
{
    AMux_FastSelect(1);
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    output[1] = ADC_GetResult8();
    ADC_StopConvert();
            
    delaytime = map(output[1],0,255,0,8);   
}


long map(long x, long in_min, long in_max, long out_min, long out_max) {
    long divisor = (in_max - in_min);
    if(divisor == 0){
        return -1; //AVR returns -1, SAM returns 0
    }
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
}

/* [] END OF FILE */

