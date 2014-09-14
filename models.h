// Copyright (c) 2014 Ezequiel Dan Glikstein

 // defines a namespace for the models of price dynamics. Could likely be used for interest rate models too

// TODO: modify models to accept unif[0,1] rv and let them manipulate the raw rv as they need it.
		// (they currently accept N(0,1) rv)
 
#ifndef MODELS_H
#define MODELS_H

#include "structs.h"	// for the common data structures
#include "mcengine.h"
#include <cmath>
namespace Models
{ 
	static int MAX_RV = 2; // the maximum number of random variables needed for a model
	StepData& BrownianStep(std::vector<double>& randoms, MCEngine& mc);
	StepData& HestonStep(std::vector<double>& randoms, MCEngine& mc);
	StepData& CEVStep(std::vector<double>& randoms, MCEngine& mc);
}
#endif
