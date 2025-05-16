#include "IMMSMacros.c"
#include "IMMSFunDeclar.c"


signed int IncDec = 0;

unsigned int tstep1 = 1;
unsigned int tstep2 = 1;

void delay(unsigned int a){
	int i, j;		
	for(i = 0;i < a; i++){
	 	for(j =0; j<1000; j++);
	}
}

void lcdConfig(void){
	LPC_GPIO0 -> FIODIR |= RS | EN | DATA;
	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_cmd(0x0c);
	lcd_cmd(0x80);
}

void lcd_cmd(unsigned char ch){
 	LPC_GPIO0->FIOCLR = DATA;
	LPC_GPIO0->FIOSET = (ch<<15);
	LPC_GPIO0->FIOCLR = RS;
	LPC_GPIO0->FIOSET = EN;
	delay(10);
	LPC_GPIO0->FIOCLR = EN;
}

void lcd_data(unsigned char d){

	LPC_GPIO0 -> FIOCLR = DATA;
	LPC_GPIO0 -> FIOSET = (d << 15);
	LPC_GPIO0 -> FIOSET = RS;
	LPC_GPIO0 -> FIOSET = EN;
	delay(10);
	LPC_GPIO0 -> FIOCLR = EN;

}

void lcd_num(unsigned int a){
 	if(a){
	 	lcd_num(a/10);
		lcd_data(a%10 + 0x30); 
		
		if(a == 0){
			lcd_data(a + 0x30);
		}
	}
	
}

void lcd_str(unsigned char *str){
	while(*str != '\0'){
	 	lcd_data(*str);
		str++;
	}
}

void lcd_strScroll(unsigned char *ptr){
	while(1){
		while(*ptr != '\0'){
			lcd_data(*ptr);
			ptr++;
		}
		lcd_cmd(0x18);
		delay(300);
	}
}



/////////////////////////////////////RTC FUNCTIONS//////////////////////////////////////
void RTC_Setup(unsigned int year, unsigned int month, unsigned int dom, unsigned int hr, unsigned int min, unsigned int sec){
	LPC_SC->PCONP |= RTC;

	LPC_RTC -> CIIR = 0;
	LPC_RTC -> CCR = 0;

	LPC_RTC -> YEAR = year;
	LPC_RTC -> MONTH = month;
	LPC_RTC -> DOM = dom;
	LPC_RTC -> HOUR = hr;
	LPC_RTC -> MIN = min;
	LPC_RTC -> SEC = sec;

	LPC_RTC -> CCR = 1;
}

int date(void){
 	return LPC_RTC->DOM;
}

int month(void){
 	return LPC_RTC->MONTH;
}

int year(void){
 	return LPC_RTC->YEAR;
}

int hr(void){
 	return LPC_RTC->HOUR;
}

int min(void){
 	return LPC_RTC->MIN;
}

int sec(void){
 	return LPC_RTC->SEC;
}
extern unsigned int i = 1;
void Date_Time(void){

    lcd_cmd(0x80);
	lcd_str("DATE: ");	
	lcd_num(date());
	lcd_data('/');
	lcd_num(month());
	lcd_data('/');
	lcd_num(year());

	lcd_cmd(0xc0);
	lcd_str("TIME: ");	
	(hr() == 0) ? lcd_data('0'):lcd_num(hr());
	lcd_data(':');
	(min() == 0) ? lcd_data('0'):lcd_num(min());
	lcd_data(':');
	lcd_num(sec());

	if(sec() == 0 && i == 1){
	 	lcd_cmd(0x01);
		i = 2;
	}else if(i == 2 && sec() != 0){
	 	i = 1;
	}	
}


//////////////////BUZZER /////////////////////////////////////

void Buzzer_Config(void){
 	LPC_GPIO1->FIODIR |= Buzzer;
}

void BuzzerOn(void){
	LPC_GPIO1->FIOSET = Buzzer;
}

void BuzzerOff(void){
	LPC_GPIO1->FIOCLR = Buzzer;
}

void Buzzer_trig(void){
 	BuzzerOn();
	LPC_GPIO1->FIOSET = LED;
	delay(300);
	BuzzerOff();
	LPC_GPIO1->FIOCLR = LED;
	delay(50);
	BuzzerOn();
	LPC_GPIO1->FIOSET = LED;
	delay(100);
	BuzzerOff();
	LPC_GPIO1->FIOCLR = LED;
	delay(50);
}

//////////////////////////LED//////////////////////////////
void LED_Config(void){
 	LPC_GPIO1->FIODIR |= LED;
}


////////////////////////MATRIX KEYPAD//////////////////////

int order(unsigned int b, unsigned int a){
 	if(a==1 && b==1){
	 	return 1;
	}else if(a==2 && b==1){
		return 2;
	}else if(a==4 && b==1){
		return 3;
	}else if(a==8 && b==1){
		return 4;
	}else if(a==1 && b==2){
		return 5;
	}else if(a==2 && b==2){
		return 6;
	}else if(a==4 && b==2){
		return 7;
	}else if(a==8 && b==2){
		return 8;
	}else if(a==1 && b==4){
		return 9;
	}else if(a==2 && b==4){
		return 10;
	}else if(a==4 && b==4){
		return 11;
	}else if(a==8 && b==4){
		return 12;
	}else if(a==1 && b==8){
		return 13;
	}else if(a==2 && b==8){
		return 14;
	}else if(a==4 && b==8){
		return 15;
	}else if(a==8 && b==8){
		return 16;
	}
	return '\0';
}

int MatrixOutput(void){
 	unsigned int row;
	unsigned int col;
	unsigned int value;

	LPC_GPIO2 -> FIODIR &= ~ROW;
	LPC_GPIO2 -> FIODIR |= COL;

	row = LPC_GPIO2 -> FIOPIN & ROW;
	row = row >> 4;

	LPC_GPIO2 -> FIODIR |= ROW;
	LPC_GPIO2 -> FIODIR &= ~COL;

	col = LPC_GPIO2 -> FIOPIN & COL;

	value = order(0x0f^row, 0x0f^col);

	return value;

}
////////////////////////////IR //////////////////////////////////////

void IR1_Config(unsigned int a){
 	LPC_GPIO3 -> FIODIR &= a;
	LPC_PINCON -> PINSEL7 = 0X00000001 | 0X00000001;
}

int ir1_read(unsigned int a){
 	return LPC_GPIO3 -> FIOPIN & a;
}


////////////////////////////UART///////////////////////////////////////////

void uart_init(void){							 
 
    LPC_PINCON->PINSEL0 |= (1 << 4); 
    LPC_PINCON->PINSEL0 &= ~(1 << 5);  

    LPC_PINCON->PINSEL0 |= (1 << 6);  
    LPC_PINCON->PINSEL0 &= ~(1 << 7);  
    
    LPC_SC->PCONP |= (1 << 3);  
    
    LPC_SC->CLKSRCSEL = 0x00; 
	
   
    LPC_SC->PCLKSEL0 &= ~((1 << 7) | (1 << 6));  

    LPC_UART0->LCR |= 0x03;  

    LPC_UART0->LCR |= (1 << 7);  

    LPC_UART0->DLM = 0x00; 

    LPC_UART0->DLL = 0x06;  

    LPC_UART0->FDR |= 0xC1;  

    LPC_UART0->FCR |= 0x07; 

    LPC_UART0->LCR &= ~(1 << 7);
}

char Message_receive(void)
{
    
    while ((LPC_UART0->LSR & (1 << 0)) == 0);
    return LPC_UART0->RBR; 
}

void Message_send(char *str)
{
    
    while (*str)
    {
        uart_transmit(*str++); 
    }

    uart_transmit('\r');  
    uart_transmit('\n');  
}

void uart_transmit(char data){
    while (!(LPC_UART0->LSR & (1 << 5)));  
    LPC_UART0->THR = data;
}
////////////////////////////////////////////////////////////////////////////

int inc_dec(unsigned int button){

	if(IncDec == 0 && button == 3){ 
		IncDec = 1;  
	}else if(IncDec == 1 && button != 3){ 
		IncDec = 0; 
	}

	if(IncDec == 0 && button == 2){ 
		IncDec = -1;  
	}else if(IncDec == -1 && button != 2){ 
		IncDec = 0; 
	}

	return IncDec;
}


void IMMS_init(void){
   	Message_send(" **** WELCOME TO INTELLEGENT MEDICINE MENTOR SYSTEM **** ");
	delay(1000);
	uart_transmit('\r');
	Message_send(" 1 -> SET DATE AND TIME COUNT");
	//uart_transmit('\r');
	uart_transmit('\n');
	Message_send(" 2 -> DECREASING COUNT");
	//uart_transmit('\r');
	uart_transmit('\n');
	Message_send(" 3 -> INCREASING COUNT");
	//uart_transmit('\r');
	uart_transmit('\n');
	Message_send(" 4 -> DATA CONFIGURE");
	//uart_transmit('\r');
	uart_transmit('\n');
	Message_send(" 8 -> CLEAR THE CURRENT DATA");
   	//uart_transmit('\r');
	uart_transmit('\n');
	Message_send(" 5 -> DATA RESET");
}



void Servo_Init(void){
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


	 LPC_PWM1->MR2; //	25% DUTY CYCLE
	 LPC_PWM1->LER=(1<<2); //LOAD AND ENABLE VALUE IN MR2
	 delay(1000);
}


