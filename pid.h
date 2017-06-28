#pragma omp once

class pid{
public:
	void set_value(double input = 0);
	void set_goal(double input = 0);
	double update(double input = 0);
	pid(double kprop2 = 1, double kint2 = 0, double kdiff2 = 0);
private:
	double kprop;
	double kint;
	double kdiff;
	double goal;
	double value;
	double error;
	double int_error;
	double diff_error;
	double prev_error;
};
