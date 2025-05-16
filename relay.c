#include <LPC17xx.h>
#include "IMMSFunDefine.c"

#define TRIG_PIN (1 << 27)
#define ECHO_PIN (1 << 28)  

void delay_us(unsigned int time) {
    LPC_TIM0->TCR = 0x02; 
    LPC_TIM0->TCR = 0x01;
    //while (LPC_TIM0->TC < time){lcd_cmd(0x01);lcd_num(LPC_TIM0->TC);delay(1000);}
}

int main() {
    LPC_PINCON->PINSEL1 &= ~(0x3<<4); 
    LPC_PINCON->PINSEL1 &= ~(0x3<<12);

    LPC_GPIO0->FIODIR |= TRIG_PIN;    
    LPC_GPIO0->FIODIR &= ~ECHO_PIN;    

	lcdConfig();						
    
    while (1) {

		unsigned int duration;
        LPC_GPIO0->FIOSET = TRIG_PIN; 
        delay_us(10);                 
        LPC_GPIO0->FIOCLR = TRIG_PIN;
        
        while (!(LPC_GPIO0->FIOPIN & ECHO_PIN));
        LPC_TIM0->TCR = 0x01;         
        while (LPC_GPIO0->FIOPIN & ECHO_PIN);
        duration = LPC_TIM0->TC;  


		lcd_cmd(0x01);
		lcd_cmd(0x80);
		lcd_str("OUTPUT : ");
		lcd_num(duration);
		delay(200);

      
        LPC_TIM0->TCR = 0x02;  
    }
}
