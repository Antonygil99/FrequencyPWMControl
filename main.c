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

int i = 0;

CY_ISR_PROTO(duty);

uint32_t MAP(uint8_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
    return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}
int main(void)
{
    
    uint16 output;
    
    I2COLED_Start();
   display_init(DISPLAY_ADDRESS);
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_duty_StartEx(duty);
    isr_duty_ClearPending();
    

    //display_init(DISPLAY_ADDRESS);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //.................................................
    //.........PWM INIT ENABLE MODULES.................
    
    PWM_1_Start();
    PWM_2_Start();
    OPAMP_Start();
    ADC_Start();
    ADC_StartConvert();
   
    
    
    for(;;)
    {
        // From line 43 to line 55 the code runs the frequency of the PWM
        if(ADC_IsEndConversion(ADC_RETURN_STATUS))
        {
            output = ADC_GetResult8();
            
            uint16 freq = MAP(output,0,256,1332,132);            
             /*Saturate ADC result to positive numbers. */
            if(output < 0)
            {
                output = 0;
            }

            Clock_SetDividerRegister(freq,0);
         
     
       }
       uint16 devregclock = Clock_GetDividerRegister();
    
     char data[50];
       sprintf(data,"%u",devregclock);
       display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println(data);
       display_update();
       CyDelay(200);
       
       // From line  to  the code runs the duty cycle of the pwm 
       
    }
}


CY_ISR(duty)
{

    CyDelay(300);
    i++;
    
    if(i==1)        // DUTY 10%
    {
      PWM_1_WritePeriod(25); 
      
        
      
       display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println("10%");
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(60,20);
       gfx_println("duty");
       display_update();
    }
    else if(i == 2) // DUTY 20%
    {
      PWM_1_WritePeriod(51); 
     
    
       display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println("20%");
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(60,20);
       gfx_println("duty");
       display_update();
    }
    else if(i == 3) // DUTY 30%
    {
      PWM_1_WritePeriod(77); 
     
    display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println("30%");
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(60,20);
       gfx_println("duty");
       display_update();
    }
    else if(i == 4) // DUTY 40%
    {
      PWM_1_WritePeriod(102); 
      
      display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println("40%");
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(60,20);
       gfx_println("duty");
       display_update();
    }
    else if(i == 5) // DUTY 50%
    {
      PWM_1_WritePeriod(127); 
      
      display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println("50%");
       gfx_setTextSize(1);
       gfx_setTextColor(WHITE);
       gfx_setCursor(60,20);
       gfx_println("duty");
       display_update();
    }
    
    
    if(i>5)
    {
        i = 0;
    }
    duty_ClearInterrupt();
    
    
}

/* [] END OF FILE */
