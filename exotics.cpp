// Copyright (c) 2014 Ezequiel Dan Glikstein

#include "exotics.h"
#include <cmath>
#include <iostream>

namespace Exotics{
	void ArithmeticAvg(StepData& step)
	{
		step.step_num++;
		step.state[0] = (step.state[0] * (step.step_num-1) + step.S)/step.step_num;
		//std::cout<<"step["<<step.step_num<<"]: price = "<<step.S<<" vol = "<<step.vol<<" state = "<<step.state[0]<<'\n';
	}
	
	void Vanilla(StepData& step)
	{
		step.state[0] = step.S;
		step.step_num++;
		//std::cout<<"step["<<step.step_num<<"]: price = "<<step.S<<" vol = "<<step.vol<<" state = "<<step.state[0]<<'\n';
		
	}
	
	void GeometricAvg(StepData& step)
	{
		step.step_num++;
		step.state[0] = exp( (log(step.state[0]) * (step.step_num-1) + log(step.S) )/step.step_num ); 
		// pow( pow(step.state[0], step.step_num-1)*step.S, 1.0/step.step_num) overflows easily
		//std::cout<<"step["<<step.step_num<<"]: price = "<<step.S<<" vol = "<<step.vol<<" state = "<<step.state[0]<<'\n';
	}
	
	// etc
	
} 
