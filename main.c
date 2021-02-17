
#include "project.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "C:\Users\hp\Documents\Intership\PSoC\library\Oled-Psoc5-master\OLED.cydsn\ssd1306.h"

#define  DISPLAY_ADDRESS 0x3c

//.......global variables.....
//............................
uint8 i = 0;        //interruption flag
uint16 output = 0; //ADC gotten value
uint16 freq = 0;  //Varable for the frequency mapping
char data[50];   


//..............................
//.....M A P F U N C T I O N....
//..............................

uint32_t MAP(uint8_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
    return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}
//PROTO FUNCTION FOR SWITCH INTERRUPT
CY_ISR_PROTO(duty);// Duty cycles from PWM1 and PWM2 are modified


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_duty_StartEx(duty);
    isr_duty_ClearPending();
    
    PWM_1_Start();
    PWM_2_Start();
    OPAMP_Start();
    ADC_Start();
    ADC_StartConvert();
    
    I2COLED_Start();
    display_init(DISPLAY_ADDRESS);
   
    for(;;)
    {
        
        if(ADC_IsEndConversion(ADC_RETURN_STATUS))
        {
            output = ADC_GetResult8();
            
            freq = MAP(output,0,256,1332,132);            
             /*Saturate ADC result to positive numbers. */
            if(output < 0)
            {
                output = 0;
            }

            Clock_SetDividerRegister(freq,0);//sets the devider register from 1132(70Hz) to 132(700Hz) 
            //modifies the period of both PWM1 and PWM2
     
       }
        
       uint16 devregclock = Clock_GetDividerRegister();//gets the divider register value
        
       //this part of the code corresponds to the OLED display I2C module 
       //which has some bugs those which I am Working yet.
       
       sprintf(data,"%u",devregclock);
       display_clear();    
       display_update();    
       gfx_setTextSize(3);
       gfx_setTextColor(WHITE);
       gfx_setCursor(2,5);
       gfx_println(data);
       display_update();
      // CyDelay(200);
        
       
    }
}


CY_ISR(duty)
{

    CyDelay(300);
    i++;
    
    if(i==1)        // DUTY 10%
    {
      PWM_1_WritePeriod(25); 
      PWM_2_WritePeriod(25); 
      
        
      
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
      PWM_2_WritePeriod(51); 
    
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
      PWM_2_WritePeriod(77);
     
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
      PWM_2_WritePeriod(102); 
      
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
      PWM_2_WritePeriod(127); 
      
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
