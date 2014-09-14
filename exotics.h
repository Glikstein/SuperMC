// Copyright (c) 2014 Ezequiel Dan Glikstein

// This file defines expansions of the state space that render the price path of a path-dependent option Markovian
#ifndef EXOTICS_H
#define EXOTICS_H

#include "structs.h"	// for StepData

namespace Exotics{
	void ArithmeticAvg(StepData& step);
	void Vanilla(StepData& step);
	void GeometricAvg(StepData& step);
	
}
#endif