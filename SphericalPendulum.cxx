/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

#include "SphericalPendulum.h"
#include <iostream>

#include <boost/numeric/odeint.hpp>
#include <vector>

using namespace std;
using namespace boost::numeric::odeint;

// Constructor
SphericalPendulum::SphericalPendulum(){
	cout << "Pendlum Painter initiated\n";
};

SphericalPendulum::SphericalPendulum(double pendulumLength, vector<double> init_x, vector<double> dampingCoeff, vector<double> timeSettings) {
	setPendulumLength(pendulumLength);
	setInitState(init_x);
	setDampingCoeff(dampingCoeff);
	setTimeSettings(timeSettings);
	cout << "Pendlum Painter initiated\n";
};

// Deconstructor
SphericalPendulum::~SphericalPendulum() {
	cout << "Objects deleted\n";
}

//-------------------------------    SET FUNCTIONS   -------------------------------------

// Set Pendulum length
void SphericalPendulum::setPendulumLength(double pendulumLength) {
	r = pendulumLength;
}

// Set the initial state from which the integration should start
void SphericalPendulum::setInitState(vector<double> init_x) {
	for (int i = 0; i <= 3; i++) {
		x0[i] = init_x[i];
	}
}

// Set damping coefficients
void SphericalPendulum::setDampingCoeff(vector<double> dampingCoeff) {
	d = dampingCoeff;
}

// Set time settings for solver
void SphericalPendulum::setTimeSettings(vector<double> timeSettings) {
	time = timeSettings;
}

//-------------------------------    ODE FUNCTIONS   -------------------------------------

// outputs the solution of the ODE
void SphericalPendulum::printState(vector<double> t, vector<stateType> x, size_t& s){
	for (size_t i = 0; i <= s; i++)	{
		cout << "t = " << t[i] << '\t' << "phi = " << x[i][0] << '\t' << '\t' << "theta = " << x[i][2] << '\t' << '\n';
	}
}

// Introducing the ODEs as first order ODEs. Apply numerical integration design as the right hand side (RHS) of the equations.
void SphericalPendulum::defineODESystem(const stateType& x, stateType& dxdt, double t) {
	dxdt[0] = x[1];
	dxdt[1] = x[3] * x[3] * sin(x[0]) * cos(x[0]) - g / r * sin(x[0]);
	dxdt[2] = x[3];
	dxdt[3] = -(2 * x[3] * x[1] * cos(x[0])) / sin(x[0]);
}

// Observe the solution during the integration steps
struct push_back_state_and_time {
	vector< stateType >& m_states;
	vector< double >& m_times;

	push_back_state_and_time(vector< stateType >& states, vector< double >& times)
		: m_states(states), m_times(times) { }

	void operator()(const stateType& x, double t) {
		m_states.push_back(x);
		m_times.push_back(t);
	}
};

void SphericalPendulum::integrateODE(){
	// Observe vectors
	vector<stateType> x_vec;
	vector<double> times;

	size_t steps = integrate(defineODESystem, x0, time[0], time[1], time[2], push_back_state_and_time(x_vec, times));

	printState(times, x_vec, steps);
}








