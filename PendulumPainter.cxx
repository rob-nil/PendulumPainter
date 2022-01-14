/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */

#include "PendulumPainter.h"
#include "ui_PendulumPainter.h"

#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>


#include <QFileDialog.h>

#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>

#include <vtkAssembly.h>

#include <iostream>
#include <vector>

#include <vtkProperty.h>

#include <vtkCamera.h>
#include <vtkPNGWriter.h>

#include <vtkWindowToImageFilter.h>

#include <vtkSTLReader.h>
#include <QColorDialog>
#include <QColor>
#include <vtkActor.h>

//------------------------------------------------------------------------------------------
//                                  CONSTRUCTOR   
//------------------------------------------------------------------------------------------
PendulumPainter::PendulumPainter()
{
  // Initialize Class Member Variables
 
  //Default Values
  numIncr = 1;			// saves incrementation step of simulation as class member variable
  run = false;			// status about simulation 
  simSpeedms = 5;		// defines the simulation speed (must be given in ms)
  timerms = 500;		// Time in ms when Qtimer is firering (asken if button is pressed or not)
  linewidth = 4;		// Define linewith of line drawn on the 2D/3D Visulization
  pendulumLength = 50;
  vStart = 0;			// Start velocity of Pendulum in start position
  xyzStart = {0, 0, 0};	// Start Position of Pendulum 

  resolution = 30;
  ConeGroundDist = 3;
  CylRadius = 0.05;
  ConeRadius = 4;
  ConeHeight = 7;

 
  cupscale = 2.5;
  cupRadius = 1.2 * cupscale;
  cupHeight = 3.8 * cupscale;

  int simSpeedMax = 0;	 //inverse logic (0 ms wait timer)
  int simSpeedMin = 100;	
  int simSpeedStepSize = 10;

  //-------------------------------    Qt -------------------------------------
  this->ui = new Ui_PendulumPainter;
  this->ui->setupUi(this);

  // set UI input values to default
  this->ui->lineEditLineWidth->setText(QString::number(linewidth));
  this->ui->lineEditPendulumLength->setText(QString::number(pendulumLength));
  this->ui->lineEditStart_v->setText(QString::number(vStart));
  this->ui->lineEditStart_x->setText(QString::number(xyzStart.x));
  this->ui->lineEditStart_y->setText(QString::number(xyzStart.y));
  this->ui->lineEditStart_z->setText(QString::number(xyzStart.z));

  this->ui->comboBox->addItem("Background 3D");
  this->ui->comboBox->addItem("Background 2D");
  this->ui->comboBox->addItem("Drawing Paper");
  this->ui->comboBox->addItem("Painting Color");
  
  //-------------------------------    GEOMETRY -------------------------------------
  // (1) Pendulum
  // create Pointers
  cylinder = vtkCylinderSource::New();
  cylinderMapper = vtkPolyDataMapper::New();
  cylinderActor = vtkActor::New();
  cylinderActor2 = vtkActor::New();
  cone = vtkConeSource::New();
  coneMapper = vtkPolyDataMapper::New();
  coneActor = vtkActor::New();
  assembly = vtkAssembly::New();
  plane = vtkPlaneSource::New();
  planeMapper = vtkPolyDataMapper::New();
  planeActor = vtkActor::New();
  sphere = vtkSphereSource::New();
  sphereMapper = vtkPolyDataMapper::New();
  sphereActor = vtkActor::New();
  
  stlReaderCup = vtkSTLReader::New();
  cupMapper = vtkPolyDataMapper::New();
  cupActor = vtkActor::New();

  // Initialize an Parameterize Pendulum according current value form UI
  init3DActors();



  //-------------------------------    VTK QT RENDERER   -------------------------------------
  // 3D VTK Renderer 
  ren->AddActor(assembly);
  ren->AddActor(planeActor);
  ren->SetBackground(colors->GetColor3d("snow").GetData());
    ren->GetActiveCamera()->Elevation(30);
  ren->GetActiveCamera()->Azimuth(10);
  //ren->GetActiveCamera()->SetPosition(0,30,200);
  ren->ResetCamera();

  // 3D VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
  this->ui->qvtkWidget3D->setRenderWindow(renderWindow);
  this->ui->qvtkWidget3D->renderWindow()->AddRenderer(ren);
  

  // 2D VTK Renderer / Camera Options
  ren2D->GetActiveCamera()->Elevation(90);
  ren2D->GetActiveCamera()->Zoom(0.02);

  ren2D->SetBackground(colors->GetColor3d("snow").GetData());
  
  // 2D VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow2D;
  this->ui->qvtkWidget2D->setRenderWindow(renderWindow2D); 
  this->ui->qvtkWidget2D->renderWindow()->AddRenderer(ren2D);

  // 2D Interactor
  vtkRenderWindowInteractor* iren2D = vtkRenderWindowInteractor::New();
  iren2D->SetRenderWindow(renderWindow2D);
  iren2D->Disable();
 
  //-------------------------------    SLOTS Connections   ---------------------------
  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonSim, SIGNAL(clicked()), this, SLOT(pushButtonSim()));
  connect(this->ui->pushButtonInitialize, SIGNAL(clicked()), this, SLOT(initialize()));
  connect(this->ui->SliderSimSpeed, SIGNAL(sliderReleased()), this, SLOT(getSliderValue()));
  connect(this->ui->actionPrint, SIGNAL(triggered()), this, SLOT(saveImage()));
  connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(changeColor()));

  //-------------------------------    Timer   -------------------------------------
  // Timer Setup (timer will be fired every *ms)
  timer = new QTimer(this);
  timer->setInterval(timerms);
  timer->setSingleShot(false);
  connect(timer, SIGNAL(timeout()), this, SLOT(timerslot()));

  //-------------------------------    Progressbar   -------------------------------------
  this->ui->progressBar->setValue(0);

  //-------------------------------    Slider   -------------------------------------
  this->ui->SliderSimSpeed->setMaximum(simSpeedMin);
  this->ui->SliderSimSpeed->setMinimum(simSpeedMax);
  this->ui->SliderSimSpeed->setSingleStep(simSpeedStepSize);
  this->ui->SliderSimSpeed->setValue(simSpeedMin - simSpeedms);
};

//------------------------------------------------------------------------------------------
//                                  Class Function Definitions    
//------------------------------------------------------------------------------------------

// Destructor
PendulumPainter::~PendulumPainter()
{
  // The smart pointers should clean up for up
}

// Set Data Function (Data from Motion Calc Class)
void PendulumPainter::setData(vector<vec3D> vecDiffAngle, vector<vec3D> vecDraw) {
	
	vecDiffAngleP = vecDiffAngle;
	std::cout << "Angle Data initialized!\n";
	vecDrawP = vecDraw;
	std::cout << "Drawing Data initialized!\n\n";
}

// Function to update Pendulum Position (Angle) 
void PendulumPainter::rotUpdate3D(int i)
{
		assembly->RotateX(vecDiffAngleP[i].x);
		assembly->RotateZ(vecDiffAngleP[i].z);
		std::cout << "Pos increment" << vecDiffAngleP[i].x << endl;
}

// 2D: Single Simulation Increment for Pendulum Motion and Drawing 
void PendulumPainter::SimUpdate2D() {

	// (1) First Increment
	if (numIncr < 2)
	{
		// (1.1)  DRAWING 
		// Draw: Starting Points
		points2D->InsertNextPoint(vecDrawP[0].x, vecDrawP[0].y, vecDrawP[0].z);
		points2D->InsertNextPoint(vecDrawP[1].x, vecDrawP[1].y, vecDrawP[1].z);

		// Draw: Line
		line2D->SetPoints(points2D);

		// Draw: Mapper
		line2DMapper = vtkPolyDataMapper::New();
		line2DMapper->SetInputConnection(line2D->GetOutputPort());

		// Draw: Actor in scene
		line2DActor = vtkActor::New();
		//vtkActor* lineActor = vtkActor::New();
		line2DActor->SetMapper(line2DMapper);
		line2DActor->GetProperty()->SetColor(colors->GetColor3d("green").GetData());
		line2DActor->GetProperty()->SetLineWidth(linewidth*0.7);

		ren2D->AddActor(line2DActor);

		std::cout << "2D Update Initialized." << endl;
	}

	// (2) Further Increments
	else
	{
		// (2.1)  DRAWING 
		std::cout << "Coordinates:   [" << vecDrawP[numIncr].x << ", "
			<< vecDrawP[numIncr].y << ", "
			<< vecDrawP[numIncr].z << "] " << endl;

		points2D->InsertNextPoint(vecDrawP[numIncr].x, vecDrawP[numIncr].y, vecDrawP[numIncr].z);
		line2D->Modified(); // for updating linesource


		std::cout << "Increment updated: " << numIncr << " / " << points2D->GetNumberOfPoints() << endl;
	}
}

// 3D: Single Simulation Increment for Pendulum Motion and Drawing 
void PendulumPainter::SimUpdate3D() {

	// (1) First Increment
	if (numIncr < 2)
	{
		// (1.1)  DRAWING 
		// Draw: Starting Points
		points3D->InsertNextPoint(vecDrawP[0].x, vecDrawP[0].y, vecDrawP[0].z);
		points3D->InsertNextPoint(vecDrawP[1].x, vecDrawP[1].y, vecDrawP[1].z);

		// Draw: Line
		line3D->SetPoints(points3D);

		// Draw: Mapper
		line3DMapper = vtkPolyDataMapper::New();
		line3DMapper->SetInputConnection(line3D->GetOutputPort());

		// Draw: Actor in scene
		line3DActor = vtkActor::New();
		//vtkActor* lineActor = vtkActor::New();
		line3DActor->SetMapper(line3DMapper);
		line3DActor->GetProperty()->SetColor(colors->GetColor3d("blue").GetData());
		line3DActor->GetProperty()->SetLineWidth(linewidth);

		//2D
		//line2DActor = vtkActor::New();
		//line2DActor->ShallowCopy(line3DActor);


		// Draw: VTK Renderer
		ren->AddActor(line3DActor);

		// (1.2)  PENDULUM ROTATION 

		rotUpdate3D(1);

		std::cout << "3D Update Initialized."  << endl;
	}

	// (2) Further Increments
	else 
	{		
		// (2.1)  DRAWING 
		std::cout << "Coordinates:   [" << vecDrawP[numIncr].x << ", "
										<< vecDrawP[numIncr].y << ", "
										<< vecDrawP[numIncr].z << "] " << endl;

		points3D->InsertNextPoint(vecDrawP[numIncr].x, vecDrawP[numIncr].y, vecDrawP[numIncr].z);
		line3D->Modified(); // for updating linesource

		// (2.2)  PENDULUM ROTATION 
		rotUpdate3D(numIncr);

		std::cout << "Increment updated: " << numIncr << " / "<< points3D->GetNumberOfPoints() << endl;
	}
}

void PendulumPainter::initSim() {
	// Read InputData from Ui
	linewidth = this->ui->lineEditLineWidth->text().toInt();
	pendulumLength = this->ui->lineEditPendulumLength->text().toDouble();
	xyzStart = { this->ui->lineEditStart_x->text().toDouble(),
				 this->ui->lineEditStart_y->text().toDouble(),
				 this->ui->lineEditStart_z->text().toDouble() };
	vStart = this->ui->lineEditStart_v->text().toDouble();

	// Reset Simulaiton 
	numIncr = 1;
	run = false;

	// Reset Widgets
	this->ui->pushButtonSim->setText("Start");

	std::cout << "Data received from Ui updated" << endl;
};

void PendulumPainter::init3DActors() {

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
	cylinderActor->RotateZ(asin(ConeRadius / pendulumLength)*180/3.1415);

	cylinderActor2->SetPosition(0, 0, 0);
	cylinderActor2->SetOrientation(0, 0, 0);
	cylinderActor2->SetMapper(cylinderMapper);
	cylinderActor2->AddPosition(-sin(asin(ConeRadius / pendulumLength)) * pendulumLength / 2, 0, 0);
	cylinderActor2->RotateZ(-asin(ConeRadius / pendulumLength) * 180 / 3.1415);

	std::cout << "Rotate Z:" << asin(ConeRadius / pendulumLength) << endl;
	std::cout << "AddPositioin :" << sin(asin(ConeRadius / pendulumLength)) * pendulumLength / 2 << endl;
	
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
	coneActor->GetProperty()->SetColor(colors->GetColor3d("Wheat").GetData());
	// (3)Sphere
	sphere->SetCenter(0, 0, 0);
	sphere->SetRadius(1);

	// Mapper
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	// Actor in scene
	sphereActor->SetMapper(sphereMapper);
	sphereActor->SetPosition(0, 0, 0);
	sphereActor->AddPosition(0, *(cylinder->GetCenter()) + cylinder->GetHeight() / 2, 0);


	// TESTING (4) STL Cup
	stlReaderCup->SetFileName("C:/tools/Src/PendulumPainter/PendulumPainter/Plastic_Cup.stl");
	stlReaderCup->Update();

	cupMapper->SetInputConnection(stlReaderCup->GetOutputPort());

	cupActor->SetMapper(cupMapper);
	cupActor->GetProperty()->SetColor(colors->GetColor3d("blue").GetData());

	cupActor->SetPosition(0, 0, 0);
	cupActor->RotateX(-90);
	cupActor->SetScale(cupscale, cupscale, cupscale);


	// Create PENDULUM ASSEMBLY
	//assembly->AddPart(cupActor);
	assembly->AddPart(cylinderActor);
	assembly->AddPart(coneActor);
	assembly->AddPart(sphereActor);
	assembly->AddPart(cylinderActor2);
	assembly->SetPosition(0, 0, 0);
	assembly->SetOrientation(0, 0, 0);
	assembly->AddPosition(0, (cylinder->GetHeight() / 2 + ConeGroundDist + cone->GetHeight()), 0);
	assembly->SetOrigin(0, *cylinder->GetCenter() + cylinder->GetHeight() / 2, 0);

	
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
	planeActor->GetProperty()->SetColor(colors->GetColor3d("FloralWhite").GetData());
	std::cout << "Pendulum and Plane new initialized" << endl;
};

// Function to create Spehere and return Actor
vtkActor* PendulumPainter::makeSphere(double radius, double position[3])
{
	vtkSphereSource* sphere = vtkSphereSource::New();
	sphere->SetCenter(position[0], position[1], position[2]);
	sphere->SetRadius(radius);

	// Mapper
	vtkPolyDataMapper* sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	// Actor in scene
	vtkActor* sphereActor = vtkActor::New();
	sphereActor->SetMapper(sphereMapper);

	return sphereActor;
}

//------------------------------------------------------------------------------------------
//                                  SLOT Function Definitions    
//------------------------------------------------------------------------------------------

// Function to initialize Simulation and Render Window
void PendulumPainter::initialize() {

	// Call gloabl init method
	initSim();

	//Initialize Actors
	points3D->Initialize();
	points2D->Initialize();
	ren->RemoveActor(line3DActor);						
	ren2D->RemoveActor(line2DActor);

	//Initialize ProgressBar
	this->ui->progressBar->setValue(0);
	this->ui->progressBar->setMinimum(1);
	this->ui->progressBar->setMaximum(vecDrawP.size());
	
	// Create new Actors in 3D view according to current Ui Data
	init3DActors();

	//Update Qt Window
	this->ui->qvtkWidget3D->renderWindow()->Render();
	this->ui->qvtkWidget2D->renderWindow()->Render();
	QCoreApplication::processEvents();

	std::cout << "UI data assigned and itialized!\n" << endl;
}


// SIMULATION Start-Stop (currently only for Pendulum Motion)
void PendulumPainter::pushButtonSim() {

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
	while (run) {
		if (numIncr < vecDiffAngleP.size())
		{
			SimUpdate3D();
			SimUpdate2D();
			this->ui->qvtkWidget2D->renderWindow()->Render();
			this->ui->qvtkWidget3D->renderWindow()->Render();

			this->ui->progressBar->setValue(numIncr+2);
			std::cout << "Increment:   " << numIncr << endl;
			Sleep(simSpeedms);
			QCoreApplication::processEvents();
			numIncr++;
		}
		else
		{
			std::cout << "Simulation finished with incement:  " << numIncr << endl;
			this->timer->stop();
			run = false;
		}	
	}
}


// Open File (filter for png files)
void PendulumPainter::slotOpenFile() {
	QString selfilter = tr("PNG (*.png)");
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open your File"), "", ("Images (*.png)"));
}

// Exit the Program
void PendulumPainter::slotExit()
{
	qApp->exit();
}

// Message Box
void PendulumPainter::msgBox()
{
	QMessageBox msgBox;
	msgBox.setText("Cone and Sphere");
	msgBox.exec();

}
 ;

// Message Box
void PendulumPainter::getSliderValue()
{
	simSpeedms = 100 - (this->ui->SliderSimSpeed->value());
	std::cout << "Slider Value changed: " << simSpeedms << endl;
}


// Get Methods for class interaction
vec3D PendulumPainter::getStartVec() {
	return xyzStart;
};
double PendulumPainter::getStartV() {
	return vStart;
};
double PendulumPainter::getPendulumLenght() {
	return pendulumLength;
};

void PendulumPainter::saveImage() {
	
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
	QColor color = QColorDialog::getColor(QColor(64, 171, 89), 0, "Select color");
};

/*
void PendulumPainter::cC()
{
	this->ui->centralwidget->setStyleSheet("background-color: black");
	
}
*/

//assembly->SetPosition(initPosAss[0], initPosAss[1], initPosAss[2]);
//assembly->SetOrientation(initOriAss[0], initOriAss[1], initOriAss[2]);
// 
//ren->RemoveActor(assembly);

	// Store Initial Position and Orientation
	//initPosAss[0] = *(assembly->GetPosition());
	//initPosAss[1] = *(assembly->GetPosition() + 1);
	//initPosAss[2] = *(assembly->GetPosition() + 2);