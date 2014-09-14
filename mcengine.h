/* MCEngine is designed to be a flexible, extensible base class for Monte-Carlo simulations of
	financial products. While it currently is designed to work with 1-factor models, it could
	easily be extended to work with multi-factor models. 
	Additionally, this class demonstrates the flexibility of function pointers which are used 
	to both calculate the payoff of an option as well as to select the price-path generation 
	model. We provide a geometric brownian motion, a constant elasticity of volatility and a Heston 
	stochastic volatility model in namespace Models found in models.h.
	To keep this class flexible, the Simulate() method advances one step at the time on
	the discretized path.
	
	This class is ready to accomodate stochastic interest rate models
	TODO: Call an interest rate model before calling the price path model in Simulate()
	
	TODO: Add variance reduction techniques (antithetic)
	
*/

// Copyright (c) 2014 Ezequiel Dan Glikstein

#include <vector>
#include "boost/math/distributions/normal.hpp" // for the Normal distribution
#include "boost/random.hpp" // RNG
#include <cmath>	// for log, exp, etc
#include "structs.h"	// useful data structures  
#include "models.h"	// the price path models
#include "exotics.h"	

#ifndef MCENGINE_H
#define MCENGINE_H



class MCEngine
{
public:	// WARNING: BE CAREFUL DIRECTLY ACCESSSING/MODIFYING THIS DATA
	double m_stepSize, m_intervalSize;
	long m_numberofSteps, m_numberofSimulations;
	double m_sum, m_sqrSum; // for statistics; sum accumulates observations, sqrSum accumulates their square
	double (*payoff) (StepData&);			// pointer to payoff function
	StepData& (*model) (std::vector<double>&, MCEngine&);	// pointer to price-path model function; the model accepts a (double) random variable, giving control of RNG to MCEngine::Simulate()
	double D; // the discounting factor, D = exp(-r * t) where r is the interest rate of period [0,t].
						// if the interest rate is stochastic, we cannot discount from the expiration date
						// with a constant rate
	StepData m_initStep, m_currentStep, m_lastStep;	// for 1-step marching models
	void (*exotic) (StepData&);	// pointer to function to determine the state of a step for path-dependent option
	
public:
	// constructors
	MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& initialStep );
			// ^^ constant volatility of volatility, constant interest rate
	MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& (*ptrModel)(std::vector<double>&, MCEngine&), StepData& initalStep);
			// ^^ constant interest rate
	MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& (*ptrModel)(std::vector<double>&, MCEngine&), void (*ptrExotic)(StepData&), StepData& initialStep );
			// ^^ constant interest rate, path dependent option
	
	//	MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), double (*ptrInterestModel)(double), StepData& (*ptrModel)(std::vector<double>&), StepData& initialStep);
	
	
	
	~MCEngine();
	
	// calculators
	void Simulate();
	// TODO:
	// void SimulateAntiThetic();	// reduce variance with antithetic random variables
	// void SimulateConditional();// reduce variance with conditional MC
	// void SimulateDependent();	// simulate a path-dependent option
	
	
	// getters
	std::vector<double> getStats() const;
	double getMean() const {return m_sum / m_numberofSimulations;}	// MC sample mean
	double getVariance() const{};
	double getStdDev() const;
	double getStdError() const {return getStdDev()/sqrt(m_numberofSimulations);}


	// setters
	void setPayoff(double (*ptrPayoff)(StepData&)) {payoff=ptrPayoff;}
	void setModel(StepData& (*ptrModel)(std::vector<double>&, MCEngine&)){model = ptrModel;}
	void setStepNumber(long num) {m_numberofSteps = num; m_stepSize = m_intervalSize/(double)num;}
	void setIntervalSize(double sz){m_intervalSize = sz; m_stepSize = m_intervalSize/(double)m_numberofSteps;}
	
	// utilities
	void Reset() {m_lastStep = m_currentStep = m_initStep; D = 1;}
	void ResetStats(){m_sum = m_sqrSum = 0;}
	
	
};

	
#endif