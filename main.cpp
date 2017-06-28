#include "main.h"

int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_analog_converter led(
		32.0 * MHz,			//clock speed
		16,				//bits
		10.0,				//min_voltage (dac output at transmitting 0}
		-10.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		-0.01,				//min_voltage_constrain
		5,				//max_voltage_contrain
		0				//Chip select
		);

analog_digital_converter photodiode(
		0.5 * MHz,			//clock speed
		10,				//bits
		0.0,				//min_voltage (dac output at transmitting 0}
		3.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		1				//Chip select
		);


int photodiode_value = 0;
double led_value = 0.0;
double led_step = 0.5;

int goal = 20;

void setup(){
}

void loop(){
	photodiode_value = photodiode.read();
//	std::cout << "photdiode = " << photodiode_value << std::endl;
	if(photodiode_value < goal){
		led_value += led_step;
	}
	if(photodiode_value > goal){
		led_value -= led_step;
	}
//	std::cout << "LED       = " << led_value << std::endl;
	led.transmit_voltage(led_value);
//	delay(1000);
}

int main (void){
	setup();
	for (;;){
			loop();
	}
	return 0 ;
}


