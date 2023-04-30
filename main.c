/*
 * TempAndHumdity.c
 *
 * Created: 4/28/2023 4:51:49 PM
 * Author : Xinyi Yuan
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL	// 16 MHz clock speed
#endif

#include "LCD.h"
#include "DHT11sensor v1.0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void lcdSetUp();
void changeTemp(double temp[1]);
void changeHumi(double hum[1]);
void printSumError();
void printTimeError();
void printNA();

int buttonFlag = 0;		// off

//ISR(PCINT2_vect){
	
//}


ISR(INT0_vect) {
	/*
	unsigned char temp;
	temp = PORTB;
	temp = temp | 0b00000010;
	PORTB = temp;
	_delay_ms(1000);
	temp = PORTB;
	temp = temp &~(0b00000010);
	PORTB = temp;
	*/
	DHT11DisplayTemperature();
	DHT11DisplayHumidity();
	_delay_ms(5000);
	printNA();
}


int main(void)
{	
	
	//PCICR |= 0b00000001;
	//PCMSK0 |= 0b00000100;
	
	//PCICR |= 0b00000100;
	//PCMSK2 |= 0b00000100;
	cli();
	EICRA = EICRA | 0x02; // INT0 if high --> low
	EIMSK = EIMSK | 1 << INT0; // Enable INT0
	
	// LCD setup
	lcdSetUp();
	sei();
	// DHT11 setup
	int8_t DHTreturnCode;
	while(1) {
		DHTreturnCode = DHT11ReadData();
		if (DHTreturnCode == 1) {
			// need to clear lcd from (0, 6) to (0, 13) here
			/* add code here to clear */
			//DHT11DisplayTemperature();
			//DHT11DisplayHumidity();
			//printNA();
		}
		else {
			if (DHTreturnCode == -1) {
				printSumError();	// print out error starting on (0, 6)
			}
			else {
				printTimeError();	// print out error starting on (0, 6)
			}
		}
	}
	
}

void lcdSetUp(){
	_delay_ms(2);
	lcd_init();
	_delay_ms(1);
	lcd_clear();
	_delay_ms(2);
	lcd_write_word("Temp: NA");
	lcd_goto_xy(1,0);
	lcd_write_word("Humidity: NA");
	_delay_ms(1);
	lcd_send_command(0x0E);
	_delay_ms(2);
	lcd_send_command(0x0C);
}

void printNA() {
	lcd_goto_xy(0,0);
	_delay_ms(2);
	lcd_clear();
	_delay_ms(2);
	lcd_write_word("Temp: NA");
	lcd_goto_xy(1,0);
	lcd_write_word("Humidity: NA");
	_delay_ms(1);
}

void changeTemp(double temp[1]) {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	char strTemp[3];
	sprintf(strTemp, "%f", temp[0]);
	lcd_write_word(strTemp);
	_delay_ms(1);
}

void changeHumi(double hum[1]) {
	lcd_goto_xy(1, 10);
	_delay_ms(1);
	char strHum[2];
	sprintf(strHum, "%f", hum[0]);
	lcd_write_word(strHum);
	_delay_ms(1);
}

void printSumError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("SError");
	_delay_ms(1);
}

void printTimeError() {
	lcd_goto_xy(0, 6);
	_delay_ms(1);
	lcd_write_word("TError");
	_delay_ms(1);
}
