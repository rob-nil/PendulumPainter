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
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkPlaneSource.h>
#include <QFileDialog.h>

#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>

#include <vtkAssembly.h>

#include <iostream>
#include <vector>

#include <vtkLineSource.h>

#include <vtkNamedColors.h>
#include <vtkProperty.h>
using namespace std;

//------------------------------------------------------------------------------------------
//                                  Constuctor   
//------------------------------------------------------------------------------------------
PendulumPainter::PendulumPainter()
{
  // Initialize Class Member Variables
  int amount = 0;
  vec3D* arrDiffAngle = NULL;
  double* arrTime = NULL;

  this->ui = new Ui_PendulumPainter;
  this->ui->setupUi(this);

   // to use named colours
  //-------------------------------    Parameter   -------------------------------------
  int resolution = 30;
  int ConeGroundDist = 3; 
  
  // cylinder paramerter
  double CylRadius = 0.3;
  double CylLength = 50;
  double CylPosition[3] = {0,0,0};
  double CylRotation[3] = {0,0,0};

  // Cone paramerter
  double ConeRadius = 1.5;
  double ConeHeight = 3;
  double ConePosition[3] = { 0,0,0 };
  double ConeRotation[3] = {180,0,90 };

  // Sphere paramerter
  double SphRadius = 1;
  double SphPositionCenter[3] = { 0,0,0 };
  double SphPositionTop[3] = { 0,0,CylLength + ConeGroundDist };

  // Plane Parameter
  double PlaneSize = 2 * CylLength;

  //-------------------------------    Polydata (Geometry)   -------------------------------------
 
  // 1) Cylinder
  vtkCylinderSource* cylinder = vtkCylinderSource::New();
  cylinder->SetCenter(CylPosition[0], CylPosition[1], CylPosition[2]);
  cylinder->SetHeight(CylLength);
  cylinder->SetRadius(CylRadius);
  cylinder->SetResolution(resolution);

  // Mapper
  vtkPolyDataMapper* cylinderMapper = vtkPolyDataMapper::New();
  cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

  // Actor in scene
  vtkActor* cylinderActor = vtkActor::New();
  cylinderActor->SetMapper(cylinderMapper);
  cylinderActor->RotateX(CylRotation[0]);
  cylinderActor->RotateY(CylRotation[1]);
  cylinderActor->RotateZ(CylRotation[2]);


  // 2) Cone
  vtkConeSource* cone = vtkConeSource::New();
  cone->SetCenter(*(cylinder->GetCenter()) + cylinder->GetHeight() / 2 ,
				  *(cylinder->GetCenter() + 1) ,
				  *(cylinder->GetCenter() + 2));

  cone->SetHeight(ConeHeight);
  cone->SetRadius(ConeRadius);
  cone->SetResolution(resolution);

  // Mapper
  vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInputConnection(cone->GetOutputPort());

  // Actor in scene
  vtkActor* coneActor = vtkActor::New();
  coneActor->SetMapper(coneMapper);
  coneActor->RotateX(ConeRotation[0]);
  coneActor->RotateY(ConeRotation[1]);
  coneActor->RotateZ(ConeRotation[2]);

  // 3) Plane
  vtkPlaneSource* plane = vtkPlaneSource::New();
  plane->SetOrigin(0, 0, 0);
  plane->SetPoint1(PlaneSize, 0.0, 0);
  plane->SetPoint2(0, 0, PlaneSize);
  plane->SetResolution(100, 100);

  // Mapper
  vtkPolyDataMapper* planeMapper = vtkPolyDataMapper::New();
  planeMapper->SetInputConnection(plane->GetOutputPort());

  // Actor in scene
  vtkActor* planeActor = vtkActor::New();
  planeActor->SetMapper(planeMapper);
  planeActor->AddPosition(-PlaneSize/2, 0, -PlaneSize / 2);



  //-------------------------------    Assembly (Pendulum)   -------------------------------------
  assembly->AddPart(cylinderActor);
  assembly->AddPart(coneActor);
  assembly->AddPosition(0, (cylinder->GetHeight() / 2 + ConeGroundDist + cone->GetHeight()), 0);
  
  assembly->SetOrigin(0,*cylinder->GetCenter() + cylinder->GetHeight() / 2,0);
  
  //-------------------------------    Renderer   -------------------------------------
  // VTK Renderer
  ren->AddActor(assembly);
  ren->AddActor(planeActor);
  ren->AddActor(makeSphere(SphRadius, SphPositionCenter));

  // VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
  this->ui->qvtkWidget3D->setRenderWindow(renderWindow);
  this->ui->qvtkWidget3D->renderWindow()->AddRenderer(ren);

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(render3D()));
  
};

//------------------------------------------------------------------------------------------
//                                  Function Definitions    
//------------------------------------------------------------------------------------------

// Destructor
PendulumPainter::~PendulumPainter()
{
  // The smart pointers should clean up for up
}

// Set Data Function (Data from Motion Calc Class)
void PendulumPainter::setData(vector<vec3D> vecDiffAngle, vector<vec3D> vecDraw) {
	
	vecDiffAngleP = vecDiffAngle;
	std::cout << "Angle Data updated!" << endl;
	vecDrawP = vecDraw;
	std::cout << "Drawing Data updated!" << endl;
}

// Create sphere
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

// start Angle simulation of pendulum
void PendulumPainter::posUpdate3D(int i)
{
	
		assembly->RotateX(vecDiffAngleP[i].x);
		assembly->RotateZ(vecDiffAngleP[i].z);
		//this->ui->qvtkWidget3D->renderWindow()->Render();
		std::cout << "Pos increment" << vecDiffAngleP[i].x << endl;

}

void PendulumPainter::Draw3D() {
	vtkNew<vtkNamedColors> colors;

	vtkNew<vtkLineSource> line;
	vtkNew<vtkPoints> points;

	// Starting Points
	points->InsertNextPoint(vecDrawP[0].x, vecDrawP[0].y, vecDrawP[0].z);
	points->InsertNextPoint(vecDrawP[1].x, vecDrawP[1].y, vecDrawP[1].z);

	// Draw Line
	line->SetPoints(points);

	// Mapper
	vtkPolyDataMapper* lineMapper = vtkPolyDataMapper::New();
	lineMapper->SetInputConnection(line->GetOutputPort());

	// Actor in scene
	vtkActor* lineActor = vtkActor::New();
	lineActor->SetMapper(lineMapper);
	lineActor->GetProperty()->SetColor(colors->GetColor3d("blue").GetData());
	lineActor->GetProperty()->SetLineWidth(5);

	// VTK Renderer
	ren->AddActor(lineActor);

	// Create remaining Points: 
	for (int i = 2; i < vecDrawP.size()-1; i++)
	{
		// Drawing
		std::cout << "Coordinates:   [" << vecDrawP[i].x << ", "
										<< vecDrawP[i].y << ", "
										<< vecDrawP[i].z << "] " << endl;

		points->InsertNextPoint(vecDrawP[i].x, vecDrawP[i].y, vecDrawP[i].z);

		line->Modified(); // for updating linesource

		std::cout << "Amount of Points:  " << points->GetNumberOfPoints() << endl;
		
		posUpdate3D(i);
		// Pendulum

		this->ui->qvtkWidget3D->renderWindow()->Render();
		QCoreApplication::processEvents();
	}

}



//-------------------------------   SLOT Functions   -------------------------------------

// start position update of pendulum
void PendulumPainter::render3D()
{
	//posUpdate3D();
	Draw3D();
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

/*
void PendulumPainter::cC()
{
	this->ui->centralwidget->setStyleSheet("background-color: black");
	
}
*/




