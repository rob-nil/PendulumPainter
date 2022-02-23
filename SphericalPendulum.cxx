/*------------------------------------------------

 Name: Julian Kastenhuber
 Date: 02-2022
 Project: VIS3IL - Pendulum Painter

 ------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS

// includes
#include "SphericalPendulum.h"
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <math.h>

// Basics
using namespace std;
using namespace boost::numeric::odeint;
typedef vector<double> stateType;		// The type of container used to hold the state vector 
typedef vector<vector<double>> matrix;	// Matrix

// Constructors
SphericalPendulum::SphericalPendulum(){
	cout << "Spherical Pendulum initiated.\n";
};

SphericalPendulum::SphericalPendulum(vector<double> inputSettings, double dampingCoeff, vector<double> timeSettings) {
	setInputSettings(inputSettings);
	setDampingCoeff(dampingCoeff);
	setTimeSettings(timeSettings);
	cout << "Spherical Pendulum initiated and start values initaliced.\n";
};

// Deconstructor
SphericalPendulum::~SphericalPendulum() {
	cout << "Object deleted.\n";
}

//-------------------------------    SET FUNCTIONS   -------------------------------------

// Set input settings
void SphericalPendulum::setInputSettings(vector<double> inputSettings) {
	x0.push_back(inputSettings[0] * degToRad);
	x0.push_back(inputSettings[2] / r);
	x0.push_back(inputSettings[1] * degToRad);
	x0.push_back(inputSettings[3] / r);
	r = inputSettings[4];
	l = inputSettings[5];
}

// Set the initial state from which the integration should start
void SphericalPendulum::setInitState(vector<double> init_x) {
	for (int i = 0; i <= 3; i++) {
		x0[i] = init_x[i];
	}
}

// Set damping coefficients
void SphericalPendulum::setDampingCoeff(double dampingCoeff) {
	d = dampingCoeff;
}

// Set time settings for solver
void SphericalPendulum::setTimeSettings(vector<double> timeSettings) {
	timeSet = timeSettings;
}

//-------------------------------    ODE FUNCTIONS   -------------------------------------

// Introducing the ODEs as first order ODEs. Apply numerical integration design as the right hand side (RHS) of the equations.
void SphericalPendulum::defineODESystem(const stateType& x, stateType& dxdt, double t) {
	dxdt[0] = x[1];
	dxdt[1] = x[3] * x[3] * sin(x[0]) * cos(x[0]) - g / r * sin(x[0]) - d * x[1] / m;
	dxdt[2] = x[3];
	dxdt[3] = -((2 * x[3] * x[1] * cos(x[0])) / sin(x[0])) - d * x[3] / m;
}

// Observe the solution during the integration steps (Standard from boost example: harmonic oscilator)
struct push_back_state_and_time {
	vector< stateType >& m_states;
	vector< double >& m_times;

	push_back_state_and_time(vector< stateType >& states, vector< double >& times) : m_states(states), m_times(times) { }

	void operator()(const stateType& x, double t) {
		m_states.push_back(x);
		m_times.push_back(t);
	}
};

// Solving the ODE and outputs a result matrix and a time vector
// Matrix: each row is a step; a row contains in this order: phi, phi-dot, theta, theta-dot
void SphericalPendulum::integrateODE(matrix& matX, vector<double>& vecTime){
	// observe vectors
	vector<stateType> fMatX;
	vector<double> fVecTime;

	size_t steps = integrate([this](auto const& x, auto& dxdt, auto t) {this->defineODESystem(x, dxdt, t); }, 
				   x0, timeSet[0], timeSet[1], timeSet[2], push_back_state_and_time(fMatX, fVecTime) );
	
	// Output calculated data
	/*printState(fVecTime, fMatX, steps);*/

	// Retunr values
	matX = fMatX;
	vecTime = fVecTime;
}

// Calculates the values for the VTK simulation and give them back in a matrix
// Each row is a step; a row contains in this order: x & y for drawing line and angle change of phi and theta
matrix SphericalPendulum::getMatVTK(matrix& matX) {
	int rows = matX.size();
	vector<double> fVec(4);
	matrix fMat;
	for (int i = 0; i < (rows-1); i++) {
		fVec[0] = sin(matX[i][0])*(r + l) * cos(matX[i][2]);
		fVec[1] = sin(matX[i][0])*(r + l) * sin(matX[i][2]);
		fVec[2] = matX[i][0] / degToRad;
		fVec[3] = matX[i][2] / degToRad;
		fMat.push_back(fVec);
	}
	// Output calculated data
	/*printVTKMatrix(fMat);*/
	return fMat;
}



//-------------------------------    OTHER FUNCTIONS   -------------------------------------

// outputs the solution of the ODE
void SphericalPendulum::printState(vector<double> t, vector<stateType> x, size_t& s) {
	cout << "\n>>>> ODE output! ------------------------------------------------------\n\n";
	cout << " t        | phi (deg)   | theta (deg) | phi. (s-1)  | theta. (s-1) |\n";
	for (size_t i = 0; i <= s; i++) 
		printf("% .6f | % .6f  | % .6f   | % .6f   | % .6f   |\n", 
			   t[i], x[i][0] / degToRad, x[i][2] / degToRad, x[i][1], x[i][3]);
}

// Output the x/y-drawing coordinates and the phi/theta-angular increment 
void SphericalPendulum::printVTKMatrix(matrix mat) {
	int rows = mat.size();
	int columns = mat[0].size();
	cout << "\n>>>> Values for GUI! ------------------------------------------------------\n";
	cout << "x         | y         | dPhi (deg)   | dTheta (deg)   |\n";
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%.6f  | ", mat[i][j]);
		}
	cout << '\n';
	}
}



