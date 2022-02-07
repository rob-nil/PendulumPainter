/*------------------------------------------------

 Name: Julian Kastenhuber
 Date: 02-2022
 Project: VIS3IL - Pendulum Painter

 ------------------------------------------------*/

#ifndef SphericalPendulum_H
#define SphericalPendulum_H

// Includes
#include <vector>

// Basics
using namespace std;
typedef vector<double> stateType;		// The type of container used to hold the state vector 
typedef vector<vector<double>> matrix;	// Matrix

class SphericalPendulum {
	public:
		// Constructor/Destructor
		SphericalPendulum();
		SphericalPendulum(vector<double> inputSettings, double dampingCoeff, vector<double> timeSettings);
		~SphericalPendulum();

		// Set / Get functions
		void setInitState(vector<double> init_x);
		void setInputSettings(vector<double> inputSettings);
		void setDampingCoeff(double dampingCoeff);
		void setTimeSettings(vector<double> timeSettings);
		matrix getMatVTK(matrix& matX);

		// ODE functions
		void integrateODE(matrix& matx, vector<double>& vectime);
		//void integrateODE1(matrix& matX, vector<double>& vecTime);

		// Other functions
		void printState(vector<double> t, vector<stateType> x, size_t& s);
		void printVTKMatrix(matrix mat);

	private:
		double r = 100;							// Pendulum length
		double l = 3;							// Distance pendulum to drawing plane
		double m = 10;							// Mass
		const double g = 9.81;					// Gravity
		const double degToRad = 3.141592 / 180;	// Converting from degree to radiant
		double d = 0.0;							// Damping coefficients phi and theta
		stateType x0;							// Initial state vector
		vector<double> timeSet = { 0, 10, 0.1 };// Contains t_start, t_end and delta_t

		// ODE functions
		void defineODESystem(const stateType& x, stateType& dxdt, const double t);
		//void operator()(const stateType& x, stateType& dxdt, const double t);
	};

#endif // ShericalPendulum_H