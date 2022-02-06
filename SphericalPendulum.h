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

using namespace std;

// The type of container used to hold the state vector 
typedef vector<double> stateType;
typedef vector<vector<double>> matrix;

class SphericalPendulum {
	public:
		// Constructor/Destructor
		SphericalPendulum();
		SphericalPendulum(vector<double> inputSettings, vector<double> dampingCoeff, vector<double> timeSettings);
		~SphericalPendulum();

		// Set / Get functions
		void setInitState(vector<double> init_x);
		void setInputSettings(vector<double> inputSettings);
		void setDampingCoeff(vector<double> dampingCoeff);
		void setTimeSettings(vector<double> timeSettings);
		matrix getMatVTK(matrix& matX);

		// ODE functions
		void integrateODE(matrix& matx, vector<double>& vectime);
		//void integrateODE1(matrix& matX, vector<double>& vecTime);

		// Other functions
		void printState(vector<double> t, vector<stateType> x, size_t& s);
		void printAnyMatrix(matrix mat);

	private:
		double r = 100;							// Pendulum length
		double l = 3;							// Distance pendulum to drawing plane
		double m = 10;							// Mass
		const double g = 9.81;					// Gravity
		const double degToRad = 3.1415926535 / 180;		// Converting from degree to radiant
		vector<double> d = { 0.0, 0.0 };		// Damping coefficients phi and theta
		stateType x0;							// Initial state vector
		vector<double> timeSet = { 0, 10, 0.1 };	// Contains t_start, t_end and delta_t

		// ODE functions
		void defineODESystem(const stateType& x, stateType& dxdt, const double t);
		//void operator()(const stateType& x, stateType& dxdt, const double t);
	};

#endif // PendulumPainter_H