void delay(unsigned int a);
void lcdConfig(void);
void lcd_cmd(unsigned char ch);
void lcd_data(unsigned char d);
void lcd_str(unsigned char *str);
void lcd_num(unsigned int a);
void lcd_strScroll(unsigned char *ptr);


void RTC_Setup(unsigned int year, unsigned int month, unsigned int dom, unsigned int hr, unsigned int min, unsigned int sec);
int date(void);
int month(void);
int year(void);
int hr(void);
int min(void);
int sec(void);
void Date_Time(void);

void Buzzer_Config(void);
void BuzzerOn(void);
void BuzzerOff(void);
void Buzzer_trig(void);

void LED_Config(void);

int MatrixOutput(void);
int order(unsigned int b,unsigned int a);

int inc_dec(unsigned int button);

void IR1_Config(unsigned int a);
int ir1_read(unsigned int a);

void uart_init(void);            
char Message_receive(void);      
void Message_send(char *data);
void uart_transmit(char data);

void IMMS_init(void);

void Date_Time_Set(int val);
void ButtonSelection(int button);

void Servo_Init(void);


