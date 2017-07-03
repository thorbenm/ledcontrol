#include "main.h"

int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_analog_converter led(
		32.0 * MHz,			//clock speed
		16,				//bits
		10.0,				//min_voltage (dac output at transmitting 0}
		-10.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		-2.1,				//min_voltage_constrain
		9.1,				//max_voltage_contrain
		0				//Chip select
		);

analog_digital_converter photodiode(
		0.5 * MHz,			//clock speed
		10,				//bits
		0.0,				//min_voltage (dac output at transmitting 0}
		3.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		1				//Chip select
		);

void stabilize(){
	int refresh_rate = 10;
	int goal = 30;
	pid control(	0,	//kprop
			200, 	//kint
			0,	//kdiff
			1.0 * refresh_rate / 1000.0	//dt
			);
	int photodiode_value = 0;
	double led_value = 0.0;
	led.transmit_voltage(0);
	control.set_goal(goal);
	control.set_value(led_value);
	std::cout << "Goal\tIn\tOut" << std::endl; 
	for(;;){
		photodiode_value = photodiode.read();
		led_value = .1 * control.update(photodiode_value);
		std::cout << goal << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
		delay(refresh_rate);
	}
};






int main (void){
	stabilize();
	return 0 ;
}


