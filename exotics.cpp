// Copyright (c) 2014 Ezequiel Dan Glikstein

#include "exotics.h"
#include <iostream>

namespace Exotics{
	void ArithmeticAvg(StepData& step)
	{
		step.step_num++;
		step.state[0] = (step.state[0] * (step.step_num-1) + step.S)/step.step_num;
		//std::cout<<"step["<<step.step_num<<"]: price = "<<step.S<<" state = "<<step.state[0]<<'\t';
	}
	
	void Vanilla(StepData& step)
	{
		step.state[0] = step.S;
		step.step_num++;
//	std::cout<<"step["<<step.step_num<<"]: price = "<<step.S<<" vol = "<<step.vol<<" state = "<<step.state[0]<<'\t';
		
	}
	
	void GeometricAvg(StepData& step)
	{
	}
	
	// etc
	
} 
