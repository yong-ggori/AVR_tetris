/*
 * ATMEGA128A_LCD_test.c
 *
 * Created: 2022-07-27 오후 4:12:25
 * Author : KCCI26
 */ 
#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>

#define RS_PIN			0		// 명령을 담고 있는 레지스터
#define RW_PIN			1
#define E_PIN			2	

#define CLEAR_DISPLAY	0x01
#define LCD_8_BIT_MODE	0x38	// 8bit. 2 line, 5x8 폰트

void LCD_pulse_enable(void)
{
	PORTC |= (1 << E_PIN);		// E -> High
	_delay_ms(1);
	PORTC &= ~(1 << E_PIN);		// E -> Low
	_delay_ms(1);
}
void LCD_write_data(uint8_t data)
{
	PORTC |= (1 << RS_PIN);		// 문자 출력 : RS = 1
	PORTA = data;				// 출력할 문자 데이터
	LCD_pulse_enable();			// 문자 출력
	_delay_ms(2);
}
void LCD_write_command(uint8_t command)
{
	PORTC &= ~(1 << RS_PIN);	// 명령어 실행 : RS = 0
	PORTA = command;
	LCD_pulse_enable();
	_delay_ms(2);
}
void LCD_clear(void)
{
	LCD_write_command(CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_init(void)
{
	_delay_ms(50);				// 초기 구동 시간
	
	DDRA = 0xff;
	PORTA = 0x00;
	DDRC |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN);
	
	// RW 핀 LOW를 출력하여 쓰기 전용으로 사용
	PORTC &= ~(1 << RW_PIN);
	LCD_write_command(LCD_8_BIT_MODE);
	/*display on/off control, 화면 on, 커서 off, 커서 깜빡임 off*/
	uint8_t command = 0x08 | (1 << 2);
	LCD_write_command(command);
	
	LCD_clear();
	
	// 출력 후 커서를 오른쪽으로 옮김. 즉 DDRAM의 주소가 증가하여 화면 이동은 없음
	LCD_write_command(0x06);
}

void LCD_write_string(char *string)
{
	uint8_t idx;
	for(idx = 0; string[idx]; idx++) LCD_write_data(string[idx]);
}
void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %= 16;			// 0, 15
	row %= 2;			// 0, 1
	
	// 첫째 라인 시작 주소는 0x00, 둘째 라인 시작 주소는 0x40
	uint8_t address = (0x40 * row) + col;
	uint8_t command	= 0x80 + address;
	
	LCD_write_command(command);
}
int main(void)
{
	LCD_init();
	LCD_write_string("Hello HUN");
	_delay_ms(1000);
	LCD_clear();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

