/*------------------------------------------------

 Name: Patrick Holzer
 Date: 02-2022
 Project: VIS3IL - Pendulum Painter

 ------------------------------------------------*/

#ifndef PendulumPainter_H
#define PendulumPainter_H

// VTK includes
#include "vtkSmartPointer.h" // Required for smart pointer internal ivars.
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkAssembly.h>
#include <vtkNamedColors.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>
// Qt includes
#include <QProgressBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>	// Start Stop Event
// Other includes
#include "math.h"
#include <vector>
#include <string>

// Basics
using namespace std;
typedef vector<vector<double>> matrix;

// Forward Qt class declarations
class Ui_PendulumPainter;

class PendulumPainter : public QMainWindow {
	Q_OBJECT

	public:
		// Constructor
		PendulumPainter();

		// Destructor
		~PendulumPainter() override;

		// Get/Set Functions
		void setCalData(matrix& matCal);
		vector<double> getDataGUI();

	public slots:
		//Public Functions
		virtual void slotExit();		//Close Program
		virtual void pushButtonSim();
		virtual void timerslot();
		virtual void initialize();	// reads and assigns the values from the UI
		virtual void getSliderValue();
		virtual void saveImage();
		virtual void changeColor();
		virtual void changeColorDefault();

	protected:
		

	protected slots:

	private:
		// Designer form
		Ui_PendulumPainter* ui;

		// General Variables
		bool run;
		bool computing;
		int numIncr;
		int simSpeedms;
		int timerms;
		int linewidth;
		string imageFilePath;
		double lineColor[3];
		double simTime;
		double dampingCoeff;

		// Starting Values t=0
		double vStartx;
		double vStarty;
		double phi0;
		double theta0;

		// Geometry parameter
		int resolution;
		double ConeGroundDist;
		double CylRadius;
		double pendulumLength;
		double ConeRadius;
		double ConeHeight;

		// Variables for data exchange between classes
		vector<double>vecDataGUI = { 0,0,0,0,0,0 };
		matrix matCalData;

		// VTK variables
		QTimer* timer;

		vtkNew<vtkNamedColors> colors;

		vtkNew<vtkPoints> points2D;
		vtkNew<vtkPoints> points3D;

		vtkNew<vtkLineSource> line3D;
		vtkNew<vtkLineSource> line2D;

		vtkConeSource* cone;
		vtkPlaneSource* plane;
		vtkSphereSource* sphere;
		vtkCylinderSource* cylinder;

		vtkAssembly* assembly;

		vtkPolyDataMapper* cylinderMapper;
		vtkPolyDataMapper* coneMapper;
		vtkPolyDataMapper* planeMapper;
		vtkPolyDataMapper* sphereMapper;
		vtkPolyDataMapper* line3DMapper;
		vtkPolyDataMapper* line2DMapper;

		vtkActor* cylinderActor;
		vtkActor* cylinderActor2;
		vtkActor* sphereActor;
		vtkActor* line3DActor;
		vtkActor* line2DActor;
		vtkActor* coneActor;
		vtkActor* planeActor;

		vtkNew<vtkRenderer> ren;
		vtkNew<vtkRenderer> ren2D;

		// Internal Class Functions
		void SimUpdate3D();
		void SimUpdate2D();
		void initSim();
		void init3DActors();
		void runCalSphericalPendulum();
		
};

#endif // PendulumPainter_H
