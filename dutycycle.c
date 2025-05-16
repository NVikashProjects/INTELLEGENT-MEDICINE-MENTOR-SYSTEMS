								   //Program to vary the duty cycle through single edge PWM via p1.20 led
#include<lpc17xx.h>
void delay(unsigned int a);
int main()
{
	LPC_GPIO1->FIODIR|=(0XFF<<19); //CONFIGURE LED AS O/P
	 //PWM CONFIGURATION


	 LPC_SC->PCONP|=(1<<6); //ENABLE THE POWER
	 LPC_SC->PCLKSEL0|=(1<<12); //ENABLE CLK

	
	 LPC_PINCON->PINSEL3 = (1<<9); //CONFIGURE P1.20 AS PWM
	
	 LPC_PWM1->PR=3; //LOAD THE PR TO 3
	 LPC_PWM1->MR0=10000;//LOAD THE TOTAL TIME
	 LPC_PWM1->MCR=(1<<1);  //RESET
	 LPC_PWM1->LER=(1<<0); //LOAD THE ENABLE VALUE IN MR0

	 LPC_PWM1->PCR&=~(1<<2); //COMFIGURE PWM2 AS SINGLE EDGE
	 LPC_PWM1->PCR|=(1<<10); //CONFIGURE PWM2 AS O/P

	 LPC_PWM1->TCR=(1<<0)|(1<<3); //ENABLE THE COUNTER AND PWM

	 while(1)
	 {
		 LPC_PWM1->MR2; //	25% DUTY CYCLE
		 LPC_PWM1->LER=(1<<2); //LOAD AND ENABLE VALUE IN MR2
		 delay(1000);
		
		 LPC_PWM1->MR2=5000; //50 % DUTY CYCLE
		 LPC_PWM1->LER=(1<<2);
		 delay(1000);
		
		 LPC_PWM1->MR2=7500; //75% DUTY CYCLE
		 LPC_PWM1->LER=(1<<2);
		 delay(1000);
		
		 LPC_PWM1->MR2=10000; //100% DUTY CYCLE
		 LPC_PWM1->LER=(1<<2);
		 delay(1000);
	 }
}
void delay(unsigned int a){
	int i, j;		
	for(i = 0;i < a; i++){
	 	for(j =0; j<1000; j++);
	}
}
