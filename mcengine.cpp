// Copyright (c) 2014 Ezequiel Dan Glikstein


// This file both implements MCEngine and defines various price-path modelling functions
#include "mcengine.h"


///////////////////////////////////
///// MCEngine IMPLEMENTATION /////
///////////////////////////////////

// constructors

// default constructor
MCEngine::MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& initialStep ) 
: m_intervalSize(intervalSize), m_numberofSteps(numberofSteps), m_numberofSimulations(numberofSimulations), m_currentStep(initialStep), m_lastStep(initialStep), m_initStep(initialStep)
{
	payoff = ptrPayoff;
	model = Models::BrownianStep;	// set brownian motion by default
	m_sum = m_sqrSum = 0;
	D = 1.0;	// discounting factor at time 0
	m_stepSize =  intervalSize / (double)numberofSteps;
	exotic = Exotics::Vanilla;
}

// custom model constructor
MCEngine::MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& (*ptrModel)(std::vector<double>&, MCEngine&), StepData& initialStep ) 
: m_intervalSize(intervalSize), m_numberofSteps(numberofSteps), m_numberofSimulations(numberofSimulations), m_currentStep(initialStep), m_lastStep(initialStep), m_initStep(initialStep)
{
	
	payoff = ptrPayoff;
	model = ptrModel;
	m_sum = m_sqrSum = 0.0;
	D = 1.0;	// discounting factor at time 0	
	m_stepSize =  intervalSize / (double)numberofSteps;
	exotic = Exotics::Vanilla;
	std::cout<<"Constructed Engine with: S0 = "<<m_initStep.S<<" Maturity: "<<m_intervalSize<<" vol = "<<m_initStep.vol<<" r = "<<m_initStep.r<<" stepsize = "<<m_stepSize<<" number of steps = "<<m_numberofSteps<<std::endl;
	
}

// custom model constructor for path-dependent options
MCEngine::MCEngine(double intervalSize, long numberofSteps, long numberofSimulations, double (*ptrPayoff)(StepData&), StepData& (*ptrModel)(std::vector<double>&, MCEngine&), void (*ptrExotic)(StepData&), StepData& initialStep ) 
: m_intervalSize(intervalSize), m_numberofSteps(numberofSteps), m_numberofSimulations(numberofSimulations), m_currentStep(initialStep), m_lastStep(initialStep), m_initStep(initialStep)
{
	
	payoff = ptrPayoff;
	model = ptrModel;
	exotic = ptrExotic;
	m_sum = m_sqrSum = 0.0;
	D = 1.0;	// discounting factor at time 0	
	m_stepSize =  intervalSize / (double)numberofSteps;
	std::cout<<"Constructed Engine with: S0 = "<<m_initStep.S<<" Maturity: "<<m_intervalSize<<" vol = "<<m_initStep.vol<<" r = "<<m_initStep.r<<" stepsize = "<<m_stepSize<<" number of steps = "<<m_numberofSteps<<std::endl;
	std::cout<<"m_lastStep.S = "<<m_lastStep.S<<" .vol = "<<m_lastStep.vol<<" .volvol =  "<<m_lastStep.volvol<<" .r = "<<m_lastStep.r<<std::endl; 
}



MCEngine::~MCEngine()
{
	// no mem alloc'd
}




// calculators


void MCEngine::Simulate()
{
	using namespace boost::math;
	
	// setting up the RNG

	boost::mt19937 RNG1;
	boost::normal_distribution<> normal(0,1);
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > N1	(RNG1, normal);

	RNG1.seed(1203);	// fixed seed to obtain reproducible results

	std::cout<<"Simulating "<<m_numberofSimulations<<" price paths...\n";
	
	// setting up the randoms vector
	std::vector<double> randoms;
	randoms.resize(Models::MAX_RV);
	for(int r = 0; r < Models::MAX_RV; r++)
		randoms[r] = N1();
	
	// simulate!
	for(long sim = 0; sim < m_numberofSimulations; sim++)
	{	
		for(long i = 0; i<m_numberofSteps; i++)
		{
			for(int r = 0; r<Models::MAX_RV; r++)
			{
				if(randoms[r] == 0.0) 
					randoms[r] = N1();	// only generate what you need
//				std::cout<<"randoms["<<r<<"] = "<<randoms[r]<<'\t';
			}
			model(randoms, *this);
			exotic(m_lastStep);
		}
		double payout = payoff(m_currentStep) * D;
		m_sum += payout;
		m_sqrSum += payout*payout;
		
		Reset();	// reset steps, discount factor
	}
}


// getters

std::vector<double> MCEngine::getStats() const
{
	std::vector<double> foo;
	foo.push_back( getMean());
	foo.push_back( getVariance());
	foo.push_back( getStdDev());
	foo.push_back( getStdError());
	return foo;
}

double MCEngine::getStdDev() const
{
	double M = m_numberofSimulations; // for readability
	return 1.0/(M-1.0) * sqrt( m_sqrSum - 1.0/M * m_sum*m_sum );
}