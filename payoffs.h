// Copyright (c) 2014 Ezequiel Dan Glikstein

// this file defines a namespace for payoff functions

#ifndef PAYOFFS_H
#define PAYOFFS_H
#define max(A,B) A>B?A:B
#include "structs.h"

// TODO: Perhaps it might be better design to always use lastStep.state and let the function of Exotics:: decide what to keep in the state...

namespace Payoffs
{
	double K;
	double callPayoff(StepData& lastStep)
	{
		//	cout<<"Terminal asset price: "<<lastStep.S<<" option paid off "<<max(lastStep.S-K,0.0)<<endl;
		return max(lastStep.state[0] - K,0.0);
	}
	
	double putPayoff(StepData& lastStep)
	{
		//	cout<<"Terminal asset price: "<<lastStep.S<<" option paid off "<<max(K-lastStep.S,0.0)<<endl;
		return max(K - lastStep.state[0], 0.0);
	}
	
	double VIXcallPayoff(StepData& lastStep)
	{
		return max(lastStep.vol - K, 0.0);
	}
	
}
#endif