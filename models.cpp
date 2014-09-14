// Copyright (c) 2014 Ezequiel Dan Glikstein

#include "models.h"

/* whenever a model "consumes" a random variable, it should set it to 0 in order
 * to make the engine generate a new random var at the next iteration
 * This is to allow for models with an arbitrarily large number of random vars
 * while being economical in the RNG
 */

namespace Models
{ 
	 
	 ////// GEOMETRIC BROWINIAN MOTION /////
	 
	 StepData& BrownianStep(std::vector<double>& randoms, MCEngine& mc)
	 {
		 double z = randoms[0]; //use the first random number in the random vector
		 randoms[0] = 0.0;	// "consume" rv
		 // for readability
		 double S = mc.m_lastStep.S; double r = mc.m_lastStep.r;
		 double vol = mc.m_lastStep.vol; double dt = mc.m_stepSize;
		 
		 mc.m_currentStep = mc.m_lastStep;
		 // take a step
		 mc.m_currentStep.S = S * exp( (r-0.5*vol*vol)*dt + vol*sqrt(dt)*z);
		 
		 // update
		 mc.D *= exp(-r*dt); // update discount factor
		 mc.m_lastStep = mc.m_currentStep;	// update step data
		 return mc.m_lastStep;
	 }
	 
	 ////// HESTON STOCHASTIC VOLATILITY /////
	 double rho = -0.7 ; // instantaneous correlation of u1 and u2
	 double kappa = 2; // speed of mean reversion
	 double theta = 0.03; // long-run average volatility (mean-reversion level)
	 
	 StepData& HestonStep(std::vector<double>& randoms, MCEngine& mc)
	 {
		 double u1 = randoms[0]; double u2 = randoms[1]; // take the first two random numbers in vector
		 // parameters
		 randoms[0] = randoms[1] = 0.0;
		 // for readability
		 double sigma = mc.m_lastStep.volvol; // the volatility of volatility
		 double vol = mc.m_lastStep.vol;
		 double r = mc.m_lastStep.r;
		 double S = mc.m_lastStep.S;
		 double dt = mc.m_stepSize;
		 double x = log(S);
		 double newvol;
		 
		 mc.m_currentStep = mc.m_lastStep;
		 // take a step
		 mc.m_currentStep.S = exp( x + (r-0.5*vol)*dt + sqrt(vol)*( (rho * sqrt(dt)*u1) + sqrt(1.0-rho*rho)*(sqrt(dt)*u2) ));
		 newvol = vol + kappa*(theta - vol)*dt + sigma*sqrt(vol)*(sqrt(dt)*u1);
		 if(newvol<=0)
			 newvol = 0;
		 
		 mc.m_currentStep.vol = newvol;
		 // update
		 /*
		 mc.m_currentStep.volvol = mc.m_lastStep.volvol;
		 mc.m_currentStep.r = r;
		 mc.m_currentStep.state = mc.m_lastStep.state;
		 mc.m_currentStep.step_num = mc.m_lastStep.step_num;
		 */
		 
		 mc.D *= exp(-r*dt);
		 mc.m_lastStep = mc.m_currentStep;
//		 std::cout<<"m_currentStep.S = "<<mc.m_currentStep.S<<" .vol = "<<mc.m_currentStep.vol<<" .volvol =  "<<mc.m_currentStep.volvol<<" .r = "<<mc.m_currentStep.r<<" .state[0] ="<<mc.m_currentStep.state[0]<<std::endl;
//		 std::cout<<"m_lastStep.S = "<<mc.m_lastStep.S<<" .vol = "<<mc.m_lastStep.vol<<" .volvol =  "<<mc.m_lastStep.volvol<<" .r = "<<mc.m_lastStep.r<<" .state[0] ="<<mc.m_lastStep.state[0]<<std::endl;
		 
		 return mc.m_lastStep;
	 }
	 
	 
	 ///// CONSTANT ELASTICITY OF VOLATILITY /////
	 // parameters
	double beta = 1.0;
	 StepData& CEVStep(std::vector<double>& randoms, MCEngine& mc)
	 {
		 double z = randoms[0]; //use the first random number in the random vector
		 randoms[0] = 0.0;	// "consume" rv
		 
		 // for readability
		 double S = mc.m_lastStep.S; double r = mc.m_lastStep.r;
		 double vol = mc.m_lastStep.vol; double dt = mc.m_stepSize; // for readability
		 
		 mc.m_currentStep = mc.m_lastStep;
		 // take a step
		 mc.m_currentStep.S = S + r*S*dt + sqrt(dt)*vol*pow(S,beta)*z;
		 
		 // update
		 mc.D *= exp(-r*dt); // update discount factor
		 mc.m_lastStep = mc.m_currentStep;	// update step data
		 return mc.m_lastStep;
	 }
}
