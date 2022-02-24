/*----------------------------------------------------------------------------------------------

 Name: Patrick Holzer
 Date: 02-2022
 Project: VIS3IL - Pendulum Painter

 ---------------------------------------------------------------------------------------------*/

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
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkArrowSource.h>

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
		virtual void pushButtonResetView();

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

		vtkNew<vtkSphereSource>sphere;
		vtkNew<vtkPlaneSource>plane;
		vtkNew<vtkConeSource>cone;
		vtkNew<vtkCylinderSource>cylinder;
		vtkNew <vtkAssembly> assembly;
		
		vtkNew <vtkPolyDataMapper>cylinderMapper;
		vtkNew <vtkPolyDataMapper>coneMapper;
		vtkNew <vtkPolyDataMapper>planeMapper;
		vtkNew <vtkPolyDataMapper>sphereMapper;
		vtkNew <vtkPolyDataMapper>line3DMapper;
		vtkNew <vtkPolyDataMapper>line2DMapper;

		vtkNew <vtkActor>cylinderActor;
		vtkNew <vtkActor>cylinderActor2;
		vtkNew <vtkActor>sphereActor;
		vtkNew <vtkActor>line3DActor;
		vtkNew <vtkActor>line2DActor;
		vtkNew <vtkActor>coneActor;
		vtkNew <vtkActor>planeActor;

		vtkNew<vtkRenderer> ren;
		vtkNew<vtkRenderer> ren2D;

		vtkNew<vtkCamera> cam;
		vtkNew<vtkCamera> cam2D;

		// Internal Class Functions
		void SimUpdate3D();
		void SimUpdate2D();
		void initSim();
		void init3DActors();
		void runCalSphericalPendulum();
		};

#endif // PendulumPainter_H
