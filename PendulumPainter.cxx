/*----------------------------------------------------------------------------------------------

 Name: Patrick Holzer
 Date: 02-2022
 Project: VIS3IL - Pendulum Painter

 ---------------------------------------------------------------------------------------------*/

// VTK includes
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkLegendScaleActor.h>
#include <vtkAxesActor.h>
#include <vtkInteractorStyleImage.h>

// Qt includes
#include <QFileDialog.h>
#include <QColorDialog>
#include <QColor>
#include <QApplication>

// Other includes
#include <iostream>
#include <vector>
#include "PendulumPainter.h"
#include "ui_PendulumPainter.h"
#include "SphericalPendulum.h"

//**********************************************************************************************
//                                  Construktor / Destructor
//**********************************************************************************************

// Construktor
PendulumPainter::PendulumPainter()
{
	// Initialize Class Member Variables
 
	//-------------------------------  Default Values ------------------------------------------
	// General
	numIncr = 1;			// saves incrementation step of simulation as class member variable
	run = false;			// status about simulation 
	computing = false;
	simSpeedms = 5;			// defines the simulation speed (must be given in ms)
	timerms = 500;			// Time in ms when Qtimer is firering (asken if button is pressed or not)
	linewidth = 4;			// Define linewith of line drawn on the 2D/3D Visulization
	pendulumLength = 50;
	simTime = 400;			// Lenght of time periode for what calculation is performed
	dampingCoeff = 0.1;

	// Start Values
	vStartx = 0;			// Start velocity of Pendulum in start position in x position
	vStarty = 15;			// Start velocity of Pendulum in start position in x position
	phi0 = 30;				// Start Position in rad of angle phi
	theta0 =0;				// Start Position in rad of angle theta

	//Colour
	lineColor[0] = 34;		// Default RGB Color values for Drawing Line
	lineColor[1] = 139;
	lineColor[2] = 34;

	// Geometry 
	resolution = 30;		// Resolution of Geometry
	ConeGroundDist = 3;		// Distance from Ground to Cone apex
	CylRadius = 0.05;		
	ConeRadius = 3;
	ConeHeight = 7;

	// Progressbar
	int simSpeedMax = 0;	 //inverse logic (0 ms wait timer)
	int simSpeedMin = 100;	
	int simSpeedStepSize = 10;

	//-------------------------------    Qt   --------------------------------------------------
	this->ui = new Ui_PendulumPainter;
	this->ui->setupUi(this);

	// set UI input values to default velues
	this->ui->lineEditSimulationTime->setText(QString::number(simTime));
	this->ui->lineEditLineWidth->setText(QString::number(linewidth));
	this->ui->lineEditPendulumLength->setText(QString::number(pendulumLength));
	this->ui->lineEditDampingCoeff->setText(QString::number(dampingCoeff));
	this->ui->lineEditStart_vx->setText(QString::number(vStartx));
	this->ui->lineEditStart_vy->setText(QString::number(vStarty));
	this->ui->lineEditStart_phi0->setText(QString::number(phi0));
	this->ui->lineEditStart_theta0->setText(QString::number(theta0));

	this->ui->comboBox->addItem("Background 3D");
	this->ui->comboBox->addItem("Background 2D");
	this->ui->comboBox->addItem("Drawing Paper");
	this->ui->comboBox->addItem("Painting Color");
  
 	//-------------------------------    GEOMETRY ----------------------------------------------

	// Initialize an Parameterize Pendulum according current value form UI
	init3DActors();

	// Initialize Colours;
	changeColorDefault();

	// Legend Scale Actor
	vtkNew<vtkLegendScaleActor> legendScaleActor;
	legendScaleActor->SetRightAxisVisibility(0);
	legendScaleActor->SetTopAxisVisibility(0);

	vtkNew<vtkLegendScaleActor> legendScaleActor2D;
	legendScaleActor2D->SetRightAxisVisibility(0);
	legendScaleActor2D->SetTopAxisVisibility(0);


	//-------------------------------    VTK QT RENDERER   -------------------------------------
	// 3D VTK Renderer 
	ren->AddActor(legendScaleActor);
	ren->AddActor(assembly);
	ren->AddActor(planeActor);
	ren->SetBackground(colors->GetColor3d("LightGrey").GetData());
	ren->GetActiveCamera()->Elevation(30);
	ren->GetActiveCamera()->Azimuth(10);
	ren->GetActiveCamera()->SetPosition(0,0,30);
	ren->ResetCamera();

	// 3D VTK/Qt wedded
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	this->ui->qvtkWidget3D->setRenderWindow(renderWindow);
	this->ui->qvtkWidget3D->renderWindow()->AddRenderer(ren);
  
	// 2D VTK Renderer / Camera Options
	ren2D->AddActor(legendScaleActor2D);
	ren2D->GetActiveCamera()->Elevation(90);
	ren2D->GetActiveCamera()->Zoom(0.15);
	ren2D->SetBackground(colors->GetColor3d("LightGrey").GetData());
  
	// 2D VTK/Qt wedded
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow2D;
	this->ui->qvtkWidget2D->setRenderWindow(renderWindow2D); 
	this->ui->qvtkWidget2D->renderWindow()->AddRenderer(ren2D);
	
	vtkNew<vtkRenderWindowInteractor>iren2D;
	//iren2D->SetRenderWindow(renderWindow2D);
	
	vtkNew<vtkInteractorStyleImage> style2D;
	iren2D->SetInteractorStyle(style2D);
	this->ui->qvtkWidget2D->interactor()->SetInteractorStyle(style2D);
	
  
	// Coordinate System
	vtkNew<vtkAxesActor> axis;
	axis->SetTotalLength(10, 10, 10);
	axis->SetShaftType(0);
	axis->SetAxisLabels(0);
	axis->SetCylinderRadius(0.02);
	ren->AddActor(axis);

	//-------------------------------    SLOTS Connections   -----------------------------------
	// Set up action signals and slots
	connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
	connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotExit()));
	connect(this->ui->pushButtonSim, SIGNAL(clicked()), this, SLOT(pushButtonSim()));
	connect(this->ui->pushButtonInitialize, SIGNAL(clicked()), this, SLOT(initialize()));
	connect(this->ui->SliderSimSpeed, SIGNAL(sliderReleased()), this, SLOT(getSliderValue()));
	connect(this->ui->actionPrint, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(changeColor()));
	connect(this->ui->pushButton_2, SIGNAL(clicked()), this, SLOT(changeColorDefault()));

	//-------------------------------    Timer   -----------------------------------------------
	// Timer Setup (timer will be fired every *ms)
	timer = new QTimer(this);
	timer->setInterval(timerms);
	timer->setSingleShot(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerslot()));

	//-------------------------------    Progressbar   -----------------------------------------
	this->ui->progressBar->setValue(0);

	//-------------------------------    Slider   ----------------------------------------------
	this->ui->SliderSimSpeed->setMaximum(simSpeedMin);
	this->ui->SliderSimSpeed->setMinimum(simSpeedMax);
	this->ui->SliderSimSpeed->setSingleStep(simSpeedStepSize);
	this->ui->SliderSimSpeed->setValue(simSpeedMin - simSpeedms);

	//-------------------------------    SimulationButton   ------------------------------------
	// Disable Simulation button while computation
	this->ui->pushButtonSim->setEnabled(false);
	this->ui->pushButton->repaint();
	QCoreApplication::processEvents();

	std::cout << "Pendulum Painter Initiated\n" << endl;
};

// Destructor
PendulumPainter::~PendulumPainter()
{
  // VTK: Smart Pointer is used 
  // Qt:
  delete timer;
	
  std::cout << "Objects deleted" << endl;
}

//************************************************************************************************
//                                  Class Function Definitions   
//************************************************************************************************

// Set Data from Calculation Class
void PendulumPainter::setCalData(matrix& matCal) {
   /**--------------------------------------------------------------------------------------------
	* Desciption:	Receives the calculaiton Data from the class ShericalPendulum and
	*				sets it. 
	 *--------------------------------------------------------------------------------------------*/
	
	matCalData = matCal;

	std::cout << "Calculation Data received and updated!" << endl;
};

// Get Method: GUI Data
vector<double> PendulumPainter::getDataGUI() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	Returns a vector of 6 componets which is necessary to run the 
	 *				calculaiton in the ShericalPendulum class
	 *--------------------------------------------------------------------------------------------*/

	vector<double>fVec(6);
	fVec = { phi0,
			theta0,
			vStartx,
			vStarty,
			pendulumLength,
			ConeGroundDist };

	return fVec;

	std::cout << "GUI Data received!" << endl;
};

// 2D: Single Simulation Increment for Pendulum Motion and Drawing 
void PendulumPainter::SimUpdate2D() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	2D: A single simulaiton increment for the pendulum motion and drawing will 
	 *				updated and sent to render window 
	 *--------------------------------------------------------------------------------------------*/

	// (1) First Increment
	if (numIncr < 2)
	{
		// (1.1)  DRAWING 
		// Draw: Starting Points (y,x)
		points2D->InsertNextPoint(matCalData[0][0], 0, matCalData[0][1]);
		points2D->InsertNextPoint(matCalData[1][0], 0, matCalData[1][1]);

		// Draw: Line
		line2D->SetPoints(points2D);

		// Draw: Mapper
		line2DMapper->SetInputConnection(line2D->GetOutputPort());

		// Draw: Actor in scene
		line2DActor->SetMapper(line2DMapper);
		line2DActor->GetProperty()->SetColor(lineColor[0], lineColor[1], lineColor[2]);
		line2DActor->GetProperty()->SetLineWidth(linewidth*0.7);

		ren2D->AddActor(line2DActor);

		std::cout << "2D Update Initialized." << endl;
	}

	// (2) Further Increments
	else
	{
		// (2.1)  DRAWING 
		points2D->InsertNextPoint(matCalData[numIncr][0], 0, matCalData[numIncr][1]);
		line2D->Modified(); // for updating linesource
	}
}

// 3D: Single Simulation Increment for Pendulum Motion and Drawing 
void PendulumPainter::SimUpdate3D() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	3D: A single simulaiton increment for the pendulum motion and drawing will 
	 *				updated and sent to render window 
	 *--------------------------------------------------------------------------------------------*/

	// (1) First Increment
	if (numIncr < 2)
	{
		// (1.1)  DRAWING 
		// Draw: Starting Points
		points3D->InsertNextPoint(matCalData[0][0], 0, matCalData[0][1]);
		points3D->InsertNextPoint(matCalData[1][0], 0, matCalData[1][1]);

		// Draw: Line
		line3D->SetPoints(points3D);

		// Draw: Mapper
		line3DMapper->SetInputConnection(line3D->GetOutputPort());

		// Draw: Actor in scene
		line3DActor->SetMapper(line3DMapper);
		line3DActor->GetProperty()->SetColor(lineColor[0], lineColor[1], lineColor[2]);
		//line3DActor->GetProperty()->SetColor(66, 36, 122);
		line3DActor->GetProperty()->SetLineWidth(linewidth);


		// Draw: VTK Renderer
		ren->AddActor(line3DActor);

		assembly->SetOrientation(0, 0, 0);
		assembly->RotateY(-matCalData[1][3]);
		assembly->RotateZ(matCalData[1][2]);


		std::cout << "3D Update Initialized."  << endl;
	}

	// (2) Further Increments
	else 
	{		
		// (2.1)  DRAWING 
		points3D->InsertNextPoint(matCalData[numIncr][0], 0, matCalData[numIncr][1]);
		line3D->Modified(); // for updating linesource

		// (2.2)  PENDULUM ROTATION 
		assembly->SetOrientation(0, 0, 0);
		assembly->RotateY(-matCalData[numIncr][3]);
		assembly->RotateZ(matCalData[numIncr][2]);
	}
}

// Initialize Simulation loop (Increments, assign new GUI values)
void PendulumPainter::initSim() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	The global simulation loop will be initialized
	 *					- Increments
	 *					- default/ new GUI value are set
	 */

	// Read InputData from Ui
	simTime = this->ui->lineEditSimulationTime->text().toDouble();
	linewidth = this->ui->lineEditLineWidth->text().toInt();
	pendulumLength = this->ui->lineEditPendulumLength->text().toDouble();
	dampingCoeff = this->ui->lineEditDampingCoeff->text().toDouble();

	// Start Values: Take attention on sign (coordinate system)
	phi0 = this->ui->lineEditStart_phi0->text().toDouble();
	theta0 = this->ui->lineEditStart_theta0->text().toDouble();
	vStartx = (-1) * this->ui->lineEditStart_vx->text().toDouble();
	vStarty = (-1) * this->ui->lineEditStart_vy->text().toDouble();

	// Reset Simulaiton 
	numIncr = 1;
	run = false;

	// Reset Widgets
	this->ui->pushButtonSim->setText("Start");

	std::cout << "Data received from Ui updated" << endl;
};

// Initialize/ Reset all actors 
void PendulumPainter::init3DActors() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	All geometry objects, actors, assemblies are initialized / reset
	 *--------------------------------------------------------------------------------------------*/

	// 1) Cylinder
	cylinder->SetCenter(0, 0, 0);
	cylinder->SetHeight(pendulumLength);
	cylinder->SetRadius(CylRadius);
	cylinder->SetResolution(resolution);

	// Mapper
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	// Actor in scene
	cylinderActor->SetPosition(0, 0, 0);
	cylinderActor->SetOrientation(0, 0, 0);
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->AddPosition(sin(asin(ConeRadius / pendulumLength)) * pendulumLength / 2, 0, 0);
	cylinderActor->RotateZ(asin(ConeRadius / pendulumLength) * 180 / 3.1415);

	cylinderActor2->SetPosition(0, 0, 0);
	cylinderActor2->SetOrientation(0, 0, 0);
	cylinderActor2->SetMapper(cylinderMapper);
	cylinderActor2->AddPosition(-sin(asin(ConeRadius / pendulumLength)) * pendulumLength / 2, 0, 0);
	cylinderActor2->RotateZ(-asin(ConeRadius / pendulumLength) * 180 / 3.1415);
	
	// 2) Cone
	//cone->SetCenter(0, 0, 0);
	cone->SetCenter(*(cylinder->GetCenter()) + cylinder->GetHeight() / 2 + ConeHeight / 2,
		*(cylinder->GetCenter() + 1),
		*(cylinder->GetCenter() + 2));

	cone->SetHeight(ConeHeight);
	cone->SetRadius(ConeRadius);
	cone->SetResolution(resolution);

	// Mapper
	coneMapper->SetInputConnection(cone->GetOutputPort());

	// Actor in scene
	coneActor->SetMapper(coneMapper);
	coneActor->SetOrientation(0, 0, 0);
	coneActor->RotateX(180);
	coneActor->RotateY(0);
	coneActor->RotateZ(90);

	// (3) Sphere
	sphere->SetCenter(0, 0, 0);
	sphere->SetRadius(1);

	// Mapper
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	// Actor in scene
	sphereActor->SetMapper(sphereMapper);
	sphereActor->SetPosition(0, 0, 0);
	sphereActor->AddPosition(0, *(cylinder->GetCenter()) + cylinder->GetHeight() / 2, 0);

	// Create PENDULUM ASSEMBLY
	assembly->AddPart(cylinderActor);
	assembly->AddPart(coneActor);
	assembly->AddPart(sphereActor);
	assembly->AddPart(cylinderActor2);
	assembly->SetPosition(0, 0, 0);
	assembly->SetOrientation(0, 0, 0);
	assembly->AddPosition(0, (cylinder->GetHeight() / 2 + ConeGroundDist + cone->GetHeight()), 0);
	assembly->SetOrigin(0, *cylinder->GetCenter() + cylinder->GetHeight() / 2, 0); 

	
	// Pendulum Position at t=0
	assembly->RotateY(-theta0);
	//assembly->RotateWXYZ(-theta0, 0, 1, 0);
	assembly->RotateZ(phi0);

	// (4) -- Plane
	double PlaneSize = 1.5 * pendulumLength;

	plane->SetOrigin(0, 0, 0);
	plane->SetPoint1(PlaneSize, 0.0, 0);
	plane->SetPoint2(0, 0, PlaneSize);
	plane->SetResolution(100, 100);

	// Mapper
	planeMapper->SetInputConnection(plane->GetOutputPort());

	// Actor in scene
	planeActor->SetPosition(0, 0, 0);
	planeActor->SetMapper(planeMapper);
	planeActor->AddPosition(-PlaneSize / 2, 0, -PlaneSize / 2);
	std::cout << "All actors reset and initialized." << endl;
};


// Calculation of pendulum trajectory (ODE Solver) 
void PendulumPainter::runCalSphericalPendulum() {
	/**--------------------------------------------------------------------------------------------
	 * Desciption:	Functions triggers the calculation of the pendulum trajectory as well as
	 *				the drawing coordinates which is performed by the SphericalPendulum class.
	 *				After the data is generated, it will be forwarded to the PendulumPainter class.
	 *--------------------------------------------------------------------------------------------*/

	 // Disable Simulation button while computation
	this->ui->pushButtonSim->setEnabled(false);
	this->ui->pushButton->repaint();
	this->statusBar()->showMessage("Pendulum trajectory computation is running - please be patient!");
	QCoreApplication::processEvents();

	vector<double> timeSettings = { 0, simTime, 0.1 };
	SphericalPendulum mySphericalPendulum(this->getDataGUI(), dampingCoeff, timeSettings);

	vector<double> vecTime;
	vector<double> refVecTime = vecTime;
	matrix matX;
	matrix& refMatX = matX;
	matrix matVTK;
	matrix& refMatVTK = matVTK;

	//mySphericalPendulum.integrateODE(refMatX, refVecTime);
	mySphericalPendulum.integrateODE(refMatX, refVecTime);
	matVTK = mySphericalPendulum.getMatVTK(refMatX);

	this->setCalData(refMatVTK);

	// Activate pushbutton
	this->ui->pushButtonSim->setEnabled(true);
	this->ui->pushButton->repaint();
	this->statusBar()->showMessage("Computation finised!");
	QCoreApplication::processEvents();
};


//**************************************************************************************************
//                                  SLOT Function Definitions    
//**************************************************************************************************

// Initialize Simulation and Render Window
void PendulumPainter::initialize() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Global initialize method. All sub-initializations are called within this
	 *				function. 
	 * Overview:	1. initSim()
	 *				2. init3DActors()
	 *				3. runCalSphericalPendulum();
	 * 
	 * Button:		Initialize
	 * -------------------------------------------------------------------------------------------*/
	 

	// Call gloabl init method
	initSim();

	// Start Calucaltion with sphericalPendulum class
	runCalSphericalPendulum();

	//Initialize Actors
	points3D->Initialize();
	points2D->Initialize();
	ren->RemoveActor(line3DActor);						
	ren2D->RemoveActor(line2DActor);

	//Initialize ProgressBar
	this->ui->progressBar->reset();
	this->ui->progressBar->setMinimum(1);
	this->ui->progressBar->setMaximum(matCalData.size());
	this->ui->progressBar->setValue(0);

	// Create new Actors in 3D view according to current Ui Data
	init3DActors();

	//assembly->RotateZ(10);
	std::cout << "orientation:" << *assembly->GetOrientation() << endl;

	//Update Qt Window
	this->ui->qvtkWidget3D->renderWindow()->Render();
	this->ui->qvtkWidget2D->renderWindow()->Render();

	QCoreApplication::processEvents();

	std::cout << "GIU data initialized!\n" << endl;
}

// SIMULATION Start-Stop (currently only for Pendulum Motion)
void PendulumPainter::pushButtonSim() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Activates and deactivates the TIMER - timerslot() which continually calls the  
	 *				simulation increment update
	 * 
	 * Button:		Start/Stop
	 * -------------------------------------------------------------------------------------------*/
	 
	if (run == false) {
		this->ui->pushButtonSim->setText("Stop");
		run = true;
		this->timer->start();
		std::cout << "Timer active, running  " << endl;

	}
	else {
		run = false;
		this->ui->pushButtonSim->setText("Start");
		std::cout << "Timer active, NOT running " << endl;
	}
}

// Simulation LOOP called by QTimer 
void PendulumPainter::timerslot() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Timer call continually the simulaiton increment update to render the 2D and 3D 
	 *				window. Timer is active als long as there is no interruption by pushButtonSim()
	 *				or the end of the simulaition data. 
	 *--------------------------------------------------------------------------------------------*/
		
	while (run) {
		if (numIncr < matCalData.size()-1)
		{
			SimUpdate3D();
			SimUpdate2D();
			this->ui->qvtkWidget2D->renderWindow()->Render();
			this->ui->qvtkWidget3D->renderWindow()->Render();

			this->ui->progressBar->setValue(numIncr+2);
			Sleep(simSpeedms);
			QCoreApplication::processEvents();
			numIncr++;

			/*std::cout << "Step: "	<< numIncr 
									<< "\tAngleIncr.: [" << matCalData[numIncr][2] << ", "
														 << matCalData[numIncr][3] << "] " 
									<< "\tCoord.: [X: " << matCalData[numIncr][0] << " | Y: " 
														<< matCalData[numIncr][1] << "]" << endl;*/
		}
		else
		{
			std::cout << "Simulation finished with incement:  " << numIncr << endl;
			this->timer->stop();
			run = false;
		}	
	}
}

// Exit the Program
void PendulumPainter::slotExit()
{
	qApp->exit();
}


// Message Box
void PendulumPainter::getSliderValue()
{
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Function sets the value of the current slider bar postition from the GUI
	 *--------------------------------------------------------------------------------------------*/

	simSpeedms = 100 - (this->ui->SliderSimSpeed->value());
	std::cout << "Slider Value changed: " << simSpeedms << endl;
}


void PendulumPainter::saveImage() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Function to save the drawn 2D image. 
	 * 
	 * Button:		Print-Button 
	 *--------------------------------------------------------------------------------------------*/
	
	// Select directory where to save .png
	QString imageFilePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",
														      QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	imageFilePath.append("/PendulumPainter.png");
	
	// Screenshot
	vtkNew<vtkWindowToImageFilter> windowToImageFilter;
	windowToImageFilter->SetInput(this->ui->qvtkWidget2D->renderWindow());
	windowToImageFilter->ReadFrontBufferOff();       // read from the back buffer
	windowToImageFilter->Update();

	//writing
	vtkNew<vtkPNGWriter> writer;;
	writer->SetFileName(imageFilePath.toLocal8Bit().data());
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();
	
	// Textbox
	QMessageBox msgBox;
	msgBox.setText("Image Saved!");
	msgBox.exec();

	std::cout << "Image Saved to :  " << imageFilePath.toStdString() << endl;

}


void PendulumPainter::changeColor() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Function to change the color with a qt colour dialog of following objects:
	 *					- Background 3D
	 *					- Background 2D
	 *					- Drawing Paper
	 *					- Painting Color
	 *--------------------------------------------------------------------------------------------*/
	QColor color = QColorDialog::getColor(QColor(64, 171, 89), 0, "Select color");

	string objSel = this->ui->comboBox->currentText().toStdString();


	if (objSel == "Background 3D")
	{
		ren->SetBackground(color.redF(), color.greenF(), color.blueF());
		
	}
	else if (objSel == "Background 2D")
	{
		ren2D->SetBackground(color.redF(), color.greenF(), color.blueF());
	}
	else if (objSel == "Drawing Paper")
	{
		planeActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
	}
	else if (objSel == "Painting Color")
	{
		// assign colour to global color array for draw lines
		lineColor[0] = color.redF();
		lineColor[1] = color.greenF();
		lineColor[2] = color.blueF();
		
		std::cout << "Color 1:  " << lineColor[0] << endl;
		std::cout << "Color 2:  " << lineColor[1] << endl;
		std::cout << "Color 3:  " << lineColor[2] << endl;

		// Color of cone actor
		coneActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
	}
		
	this->statusBar()->showMessage("Color successfully changed!");

	this->ui->qvtkWidget3D->renderWindow()->Render();
	this->ui->qvtkWidget2D->renderWindow()->Render();
	QCoreApplication::processEvents();

	std::cout << "Color set." << endl;
};


void PendulumPainter::changeColorDefault() {
	/**---------------------------------------------------------------------------------------------
	 * Desciption:	Function sets the colour of all objects to a default value
	 *--------------------------------------------------------------------------------------------*/

	// Drawing Line (rgb but scaled to 1!!)
	lineColor[0] = 0.960784;
	lineColor[1] = 0.870588;
	lineColor[2] = 0.701961;

	// Drawing Paper
	planeActor->GetProperty()->SetColor(colors->GetColor3d("FloralWhite").GetData());

	//2D 3D Backrounds
	ren->SetBackground(colors->GetColor3d("LightGrey").GetData());
	ren2D->SetBackground(colors->GetColor3d("LightGrey").GetData());

	// Cone
	coneActor->GetProperty()->SetColor(colors->GetColor3d("Wheat").GetData());

	this->statusBar()->showMessage("Welcome to the Pendlulum Painter.");

	this->ui->qvtkWidget3D->renderWindow()->Render();
	this->ui->qvtkWidget2D->renderWindow()->Render();
	QCoreApplication::processEvents();

	std::cout << "Color set to default." << endl;
};
