/*
 * ATMEGA128A_Joystick_test.c
 *
 * Created: 2022-07-27 오후 5:14:47
 * Author : KCCI26
 */ 
#define F_CPU 14745600L
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

int val[3] = {0}; // x, y, z value 저장
	
void UART0_transmit_char(char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
/*UDR로 전송할 수 있는 데이터 크기는 8bit가 끝이기 때문에 문자열로 전송이 불가능하다.
때문에 문자열로 받은 데이터를 문자로 하나씩 보내는 것으로 대체 했다.*/
void UART0_transmit_string(char* data)
{
	while(*data != '\n')				// 문자열이 끝날때 까지 반복
	{
		UART0_transmit_char(*data);		// 처음부터 차례대로 문자 하나씩 전송
		data++;							// 시작 주소로부터 하나씩 주소 값 증가
	}
	UART0_transmit_char('\n');			// 마지막에 개행 문자 전송
}
uint8_t UART0_receive(void)
{
	while(!(UCSR0A & (1 << RXC1)));
	return UDR0;
}
unsigned int read_adc(void)
{
	ADCSRA |= 0x40;
	
	while(1){
		if((ADCSRA & 0x10) != 0) break;
	}
	return ADCW;						// ADCW = ADCL, ADCH 값 모두 읽어옴
}
int main(void)
{
	char val_str[100];
	DDRF = 0x00;
	ADCSRA |= (1 << ADEN) |  (1 << ADPS2) | (1 << ADPS1);
	/*ADEN을 ‘1’로 설정하고 첫 번째 변환을 하려면 25주기의 ADC 클럭 주기가 필요하므로 약간의 딜레이를 준다.*/
	_delay_ms(10);
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UBRR0L = 7;			// Baud rate 115200
    /* Replace with your application code */
    while (1) 
    {
		for(int idx = 0 ; idx < 3; idx++)	// ADC0, ADC1, ADC2 차례대로 사용하기 위한 반복문
		{
			ADMUX = idx;					// ADMUX = 0=>ADC0, 1=>ADC1, 2=>ADC2
			val[idx] = read_adc();			// adc 값 읽어서 차례로 저장
			_delay_ms(50);
		}
		//sprintf(val_str, "x : %d, y : %d, z : %d\n", val[0], val[1], val[2]);
		sprintf(val_str, "x : %d, y : %d\n", val[0], val[1]);
		UART0_transmit_string(val_str);
    }
}

