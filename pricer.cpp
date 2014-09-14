// This is a light-weight MC pricer that comes with price-path model selection
// as well as a very flexible payoff design allowing for an infinite amount of
// 1-factor path-independent payoff structures.

// Copyright (c) 2014 Ezequiel Dan Glikstein

#include <iostream>
#include <cmath>
#include <vector>
// boost
#include <boost/shared_ptr.hpp>
// data encapsulation
#include "structs.h"
// MC
#include "mcengine.h" 
#include "models.h"	// price path models
#include "payoffs.h"	// payoff functions
#include "exotics.h"	// state calculation for path-dependent options


using namespace std;
int main()
{
	long stepnum; long simnum;
	char type;
	StepData initialStep;
	double T;
	double (*ptrPayoff)(StepData&);
	StepData& (*ptrModel)(std::vector<double>&, MCEngine&);
	void (*ptrExotic)(StepData&);
	cout<<"Underlying's price: $"; cin>>initialStep.S;
	cout<<"Strike: $"; cin>>Payoffs::K;
	cout<<"Maturity: ";cin>>T;
	cout<<"Volatility: ";cin>>initialStep.vol;
	cout<<"riskfree: ";cin>>initialStep.r;
	cout<<"number of steps: ";cin>>stepnum;
	cout<<"number of simulations: "; cin>>simnum;

	cout<<"Model? [_b_rownian|_h_eston|_c_VE]?";cin>>type;
	switch(type)
	{
		case 'b':
			ptrModel = Models::BrownianStep; break;
		case 'h':
			ptrModel = Models::HestonStep; 
			cout<<"Volatility of volatility: ";cin>>initialStep.volvol;
			//Models::theta = initialStep.vol; 	// set long-run vol to entered vol
			break;
		case 'c':
			ptrModel = Models::CEVStep; break;
		default:
			ptrModel = Models::BrownianStep; break;
	}
	cout<<"Exotic? [_v_anilla | _a_rithmetic Asian | etc]";cin>>type;
	switch(type)
	{
		case 'a':
			ptrExotic = Exotics::ArithmeticAvg;initialStep.state.resize(1); goto pc;
			break;
		case 'v':
			ptrExotic = Exotics::Vanilla; initialStep.state.resize(1); goto pc;
			break;
		default:
			ptrExotic = Exotics::Vanilla;initialStep.state.resize(1); goto pc;
			break;
	}
	pc:	// choose put or call
	cout<<"Type? (p/c): ";cin>>type;
	switch(type)
	{
		case 'p': ptrPayoff = Payoffs::putPayoff; break;
		case 'c': ptrPayoff = Payoffs::callPayoff; break;
		default: return 1; break;
	}
	
	initialStep.step_num = 0;
	std::fill(initialStep.state.begin(), initialStep.state.end(), 0.0);	// fill the state vector with 0's
	MCEngine* testMC = new MCEngine(T, stepnum, simnum, ptrPayoff, ptrModel, ptrExotic, initialStep);
	testMC->Simulate();
	cout<<"We find a crude MC estimate for the mean to be: "<<testMC->getMean()<<"+- "<<testMC->getStdDev()<<" for "<<simnum<<" simulations\n";
	delete testMC;
	
	return 0;
}
