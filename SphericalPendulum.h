/*=========================================================================

  Program:   Visualization Toolkit
  Module:    PendulumPainter.h
  Language:  C++

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef SphericalPendulum_H
#define SphericalPendulum_H

#include <vector>

// The type of container used to hold the state vector 
typedef vector<double> stateType;

class SphericalPendulum {

public:
	// Constructor/Destructor
	SphericalPendulum();
	SphericalPendulum(double pendulumLength, vector<double> init_x, vector<double> dampingCoeff, vector<double> timeSettings);
	~SphericalPendulum();

	// functions
	void setInitState(vector<double> init_x);
	void setPendulumLength(double pendulumLength);
	void setDampingCoeff(vector<double> dampingCoeff);
	void setTimeSettings(vector<double> timeSettings);
	void printState(vector<double> t, vector<stateType> x, size_t& s);
	void integrateODE();

private:
	double r = 1;
	const double g = 9.81;					//Gravity
	vector<double> d = { 0.0, 0.0 };		//Damping coefficients phi and theta
	stateType x0;							//Initial state vector
	vector<double> time = { 0, 10, 0.1 };	//Contains t_start, t_end and delta_t

	//functions
	void defineODESystem(const stateType& x, stateType& dxdt, double t);
};

#endif // PendulumPainter_H
