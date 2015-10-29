/*
 * LCD.h
 *
 * Created: 10/25/2015 9:37:05 PM
 *  Author: Gerald Chapman
 */ 

#define Red_Output OCR0A	//Red PWM Pin
#define Green_Output OCR0B	//Green PWM Pin
#define Blue_Output OCR2B	//Blue PWM Pin

void SetBacklight(uint8_t,uint8_t,uint8_t);