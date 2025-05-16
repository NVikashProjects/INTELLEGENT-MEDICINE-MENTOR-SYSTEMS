#include <lpc17xx.h>
#include "IMMSFunDefine.c"
#include "IMMSVariables.c"

unsigned int i;
unsigned int button;
unsigned int val;
		
unsigned int No_Date = 0;
unsigned int No_Hours = 0;
unsigned int No_Minutes = 0;
unsigned int No_Seconds = 0;
	
unsigned int sec_count = 0;
unsigned int current_sec;
	
unsigned int totalTime = 0;


int main(){

 	lcdConfig();
	Buzzer_Config();
	LED_Config();
	uart_init();

	LPC_GPIO2 -> FIODIR &= ~Switch;

	i = 1;
	val = 0;

	RTC_Setup(2024, 10, 14, 10, 59, 55);
	Servo_Init();

	current_sec = sec();

	IMMS_init();

	while(1){

		button =  MatrixOutput();
		if(button == 1 && i == 1){
			val = val + 1;
			i = 2;
		}else{
			if(val != 0){
				if(button){
					Date_Time_Set(val);
					ButtonSelection(button);
				}
				 delay(270);
			}else{
				Date_Time();
				IR1_Config(ir1);

				if(sec() != current_sec){ sec_count += 1; current_sec = sec();}
				if(sec_count == totalTime && totalTime){
					lcd_cmd(0x01);
					lcd_cmd(0x80);
					lcd_str("INTAKE THE PILLS");
					uart_transmit('\r');
					uart_transmit('\n');
					Message_send("INTAKE THE PILLS");


					////////////////////servo operation//////////////////////////////

					 LPC_PWM1->MR2=-2500; //75% DUTY CYCLE
					 LPC_PWM1->LER=(1<<2);
					 delay(10);
					
					 LPC_PWM1->MR2=0; //50 % DUTY CYCLE
					 LPC_PWM1->LER=(1<<2);
					 delay(1000);

					///////////////////////////////////////////////////////////////

					sec_count = 0;
					for(i=0; i<4; i++){
						Buzzer_trig	();
					}
					lcd_cmd(0x01);
				}else if(!ir1_read(ir1)){
					lcd_cmd(0x01);
					lcd_cmd(0x80);
					lcd_str("TAKE THE PILLS");
					uart_transmit('\r');
					uart_transmit('\n');
					Message_send("TAKE THE PILLS");
					sec_count = 0;
					for(i=0; i<4; i++){
						Buzzer_trig	();
					}
					lcd_cmd(0x01);
				}
			}
			i = 1;
		}
	}
}



void Date_Time_Set(int val){
	lcd_cmd(0x01);
	lcd_cmd(0x80);
					
	switch(val){
		case 2:
			lcd_str("HOURS: ");
			No_Hours += inc_dec(button);
			if(No_Hours < 24){(No_Hours != 0) ? lcd_num(No_Hours): lcd_data('0'); }else{ No_Hours = 0;}
			if(current_sec != sec()){ sec_count = 0; }
			break;
		case 3:
			lcd_str("MINUTES: ");
			No_Minutes += inc_dec(button);
			if(No_Minutes < 60){(No_Minutes != 0) ? lcd_num(No_Minutes): lcd_data('0');}else{ No_Minutes = 0;}
			if(current_sec != sec()){ sec_count = 0; }
			break;
		case 4:
			lcd_str("SECONDS: ");
			No_Seconds += inc_dec(button);
			if(No_Seconds < 60){(No_Seconds != 0) ? lcd_num(No_Seconds): lcd_data('0');}else{ No_Seconds = 0;}
			if(current_sec != sec()){ sec_count = 0; }
			break;
		default:
			val = 1;
			lcd_str("NO OF DAYS: ");
			No_Date += inc_dec(button);
			if(No_Date < 31){(No_Date != 0) ? lcd_num(No_Date): lcd_data('0');}else{ No_Date = 0;}
			if(current_sec != sec()){ sec_count = 0; }
			break;
	}
}


void ButtonSelection(int button){
	if(button == 8){
		lcd_cmd(0x01);
		lcd_cmd(0x80);
		lcd_str("DATA CLEARED");
		uart_transmit('\r');
		uart_transmit('\n');
		Message_send("DATA CLEARED");
		No_Date = 0;
		No_Hours = 0;
		No_Minutes = 0;
		No_Seconds =0;
		val = 0;
		delay(1000);
	}
					
	if(button == 5){
		lcd_cmd(0x01);
		lcd_cmd(0x80);
		lcd_str("DATA RESET");
		uart_transmit('\r');
		uart_transmit('\n');
		Message_send("DATA RESET");
		No_Date = 0;
		No_Hours = 0;
		No_Minutes = 0;
		No_Seconds =0;
		val = 0;
	 	totalTime = 0;
		delay(1000);				 
	}			 	
	 if(button == 4){
	  	 lcd_cmd(0x01);
		 lcd_cmd(0x80);
		 lcd_str("DATA CONFIGURED");
		 uart_transmit('\r');
		 uart_transmit('\n');
		 Message_send("DATA CONFIGURED");
		 val = 0; 
		 
		 delay(1000);
		 
		 if(No_Minutes){totalTime += (No_Minutes * 60);}
	 	 if(No_Seconds){totalTime += (No_Seconds * 1);}
	 	 if(No_Hours){totalTime += (No_Hours * 3600);}
		 if(No_Date){totalTime += (No_Date * 3600 * 24);}
	 }
}
