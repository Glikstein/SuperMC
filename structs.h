// Copyright (c) 2014 Ezequiel Dan Glikstein

// this file contains useful structs to hold data.


#ifndef STRUCTS_H
#define STRUCTS_H
#include <vector>

struct OptionData
{
	double K; // strike price
	double T; // time to expiry, in years
	double vol;// market volatility
	double r;	// market risk-free rate
	double b;	// cost of carry
};

struct GreekData
{
	double delta;
	double gamma;
	double vega;
	double theta;
	double rho;
};

class StepData
{
public:
	double S;	// underlying asset price
	double vol;	// volatility
	double volvol; // volatility of volatility
	double r;	// risk-free rate
	std::vector<double> state; 	// state of the step
	long step_num;	// keep track of what step in a path this StepData corresponds to
	StepData& operator = (const StepData& source)
	{
		if(this == &source)
			return *this;
		this->S = source.S;
		this->vol = source.vol;
		this->volvol = source.volvol;
		this->r = source.r;
		for(int i = 0; i<state.size(); i++)
			this->state[i] = source.state[i];
		this->step_num = source.step_num;
		return *this;
	}
};

#endif
