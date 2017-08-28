#include "main.h"

std::chrono::time_point<std::chrono::system_clock> start, now;



int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_analog_converter led(
		32.0 * MHz,			//clock speed
		16,				//bits
		10.0,				//min_voltage (dac output at transmitting 0}
		-10.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		0,				//min_voltage_constrain
		10,				//max_voltage_contrain
		0				//Chip select
		);

analog_digital_converter photodiode(
		3.6 * MHz,			//clock speed
		10,				//bits
		0.0,				//min_voltage (dac output at transmitting 0}
		3.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		1				//Chip select
		);

void oscillate(double rate){
	for(int j=0;;j++){
//		led.transmit_voltage(5.0 * (j%11) / 10.0);
//		std::cout << photodiode.read() << "\t" << 5.0 * (j%11) / 10.0 << " V" << std::endl; 
//		led.transmit_voltage(0);
//		usleep(rate);
		led.transmit_voltage(rate);
//		usleep(rate);
	}
}

void read_adc(double rate){
	led.transmit_voltage(9);
	for(;;){
		std::cout << photodiode.read() << std::endl; 
		usleep(rate);
	}
}

void stabilize(double kp, double ti, double td, double offset){
	pid control(	kp,	//kprop
			ti, 	//tint
			td,	//tdiff
			offset	//offset
				);
	control.set_goal(500);

	int photodiode_value = 0;
	double led_value = 0.0;
	led.transmit_voltage(led_value);
	usleep(3000000);
	control.set_value(led_value);
	std::cout << "Time\tGoal\tIn\tOut" << std::endl; 
	start = std::chrono::system_clock::now();	
	for(int j = 0; j<5000 ; j++){
		photodiode_value = photodiode.read();
		led_value = control.update(photodiode_value);
		//time:	
		now = std::chrono::system_clock::now();	
		std::chrono::duration<double> difference = now - start;
		std::cout << difference.count() << "\t";

		std::cout << control.read_goal() << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
	}
};

void ziegler_nichols(double kp){
	pid control(	kp,	//kprop
			0, 	//tint
			0	//tdiff
				);
	control.set_goal(500);

	int photodiode_value = 0;
	double led_value = 0.0;
	led.transmit_voltage(0);
	usleep(3000000);
	control.set_value(led_value);
	std::cout << "Time\tGoal\tIn\tOut" << std::endl; 
	start = std::chrono::system_clock::now();	
	for(int j = 0; j<10000 ; j++){
		photodiode_value = photodiode.read();
		led_value = control.update(photodiode_value);
		//time:	
		now = std::chrono::system_clock::now();	
		std::chrono::duration<double> difference = now - start;
		std::cout << difference.count() << "\t";

		std::cout << control.read_goal() << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
	}
};

void autotune(){
	int initial_goal = 500;
	int h = 400;
	pid control(	.01,	//kprop
			0, 	//tint
			0,	//tdiff
			.5
				);
	control.set_goal(initial_goal);

	int photodiode_value = 0;
	double led_value = 0.0;
	led.transmit_voltage(0);
	usleep(3000000);
	control.set_value(led_value);
	std::cout << "Time\tGoal\tIn\tOut" << std::endl; 
	double avg = 0;
	start = std::chrono::system_clock::now();	
	for(int j = 0; j<10000 ; j++){
		photodiode_value = photodiode.read();
		led_value = control.update(photodiode_value);
		//time:	
		now = std::chrono::system_clock::now();	
		std::chrono::duration<double> difference = now - start;
		std::cout << difference.count() << "\t";

		std::cout << control.read_goal() << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
	//	usleep(1000);
		if(j<8000){
			avg = photodiode_value; 
		}else{
			avg = .99 * avg + .01 * photodiode_value; 
		}
	}
//	std::cout << std::endl << "avg = " << avg << std::endl;
	for(int j = 0; j<5000 ; j++){
		photodiode_value = photodiode.read();
		//autotune method:
		if( (double) photodiode_value > avg){
			control.set_goal(avg - h);
		}
		if( (double) photodiode_value < avg){
			control.set_goal(avg + h);
		}
		//
		led_value = control.update(photodiode_value);
		//time:	
		now = std::chrono::system_clock::now();	
		std::chrono::duration<double> difference = now - start;
		std::cout << difference.count() << "\t";

		std::cout << control.read_goal() << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
	}
};

void cohen_coon(){
	pid control(	.005,	//kprop
			0, 	//tint
			0	//tdiff
				);
	control.set_goal(500);

	int photodiode_value = 0;
	double led_value = 0.0;
	led.transmit_voltage(0);
	usleep(1000000);
	control.set_value(led_value);
	std::cout << "Time\tGoal\tIn\tOut" << std::endl; 
	start = std::chrono::system_clock::now();	
	for(int j = 0; j<1000 ; j++){
		photodiode_value = photodiode.read();
		led_value = control.update(photodiode_value);
		//time:	
		now = std::chrono::system_clock::now();	
		std::chrono::duration<double> difference = now - start;
		std::cout << difference.count() << "\t";

		std::cout << control.read_goal() << "\t" << photodiode_value << "\t" << led_value << std::endl;
		led.transmit_voltage(led_value);
	}
};

int main (int argc, char *argv[]){
	oscillate(std::stod(argv[1]));
//	read_adc(std::stod(argv[1]));
//	stabilize(std::stod(argv[1]),std::stod(argv[2]),std::stod(argv[3]),std::stod(argv[4]));
//	ziegler_nichols(std::stod(argv[1]));
//	cohen_coon();
//	autotune();
	return 0 ;
}


