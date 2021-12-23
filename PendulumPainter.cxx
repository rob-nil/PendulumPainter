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
#include <QFileDialog.h>


// Constructor
PendulumPainter::PendulumPainter()
{
  this->ui = new Ui_PendulumPainter;
  this->ui->setupUi(this);

  //---------------------------------------------
  // Geometry
  //---------------------------------------------
  
  // ------------ (1) Cone -----------------------



  // PARAMETERS
  int resolution = 30;
  int ConeGroundDist = 10; 
  
  // cylinder paramerter
  double CylRadius = 0.05;
  double CylLength = 50;
  double CylPosition[3] = { 0,(CylLength / 2 + ConeGroundDist),0};
  double CylRotation[3] = { 90,0,0 };

  // Cone paramerter
  double ConeRadius = 1;
  double ConeHeight = 3;
  double ConePosition[3] = { -(ConeGroundDist + ConeHeight / 2),0,0 };
  double ConeRotation[3] = { 0,90,0 };

  // Sphere paramerter
  double SphRadius = 1;
  double SphPositionCenter[3] = { 0,0,0 };
  double SphPositionTop[3] = { 0,0,CylLength + ConeGroundDist };


//---------------------------------------------
// Renderer
//---------------------------------------------

  // VTK Renderer
  vtkNew<vtkRenderer> ren;

  // Add Actor to renderer
  ren->AddActor(makeCone(ConeRadius, ConeHeight, ConePosition, ConeRotation, resolution));
  ren->AddActor(makeCylinder(CylRadius, CylLength, CylPosition, CylRotation, resolution));
  ren->AddActor(makeSphere(SphRadius, SphPositionCenter));
  ren->AddActor(makeSphere(SphRadius, SphPositionTop));

  // VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
  this->ui->qvtkWidget3D->setRenderWindow(renderWindow);
  this->ui->qvtkWidget3D->renderWindow()->AddRenderer(ren);


  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotExit()));

};

PendulumPainter::~PendulumPainter()
{
  // The smart pointers should clean up for up
}

// Open File (filter for png files)
void PendulumPainter::slotOpenFile() {
	QString selfilter = tr("PNG (*.png)");
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open your File"), "",("Images (*.png)"));
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
/////////////////////////////
//        GEOMETRIE
/////////////////////////////

// (1) Cone
vtkActor* PendulumPainter::makeCone(double radius, double heigth, double position[3], double rotation[3], int resolution)
{
	vtkConeSource* cone = vtkConeSource::New();
	cone->SetCenter(position[0], position[1], position[2]);
	cone->SetHeight(heigth);
	cone->SetRadius(radius);
	cone->SetResolution(resolution);

	// Mapper
	vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());

	// Actor in scene
	vtkActor* coneActor = vtkActor::New();
	coneActor->SetMapper(coneMapper);
	coneActor->RotateX(rotation[0]);
	coneActor->RotateY(rotation[1]);
	coneActor->RotateZ(rotation[2]);

	return coneActor;
}

// (2) Cylinder 
vtkActor* PendulumPainter::makeCylinder(double radius, double heigth, double position[3], double rotation[3], int resolution)
{
	vtkCylinderSource* cylinder = vtkCylinderSource::New();
	cylinder->SetCenter(position[0], position[1], position[2]);
	cylinder->SetHeight(heigth);
	cylinder->SetRadius(radius);
	cylinder->SetResolution(resolution);

	// Mapper
	vtkPolyDataMapper* cylinderMapper = vtkPolyDataMapper::New();
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	// Actor in scene
	vtkActor* cylinderActor = vtkActor::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->RotateX(rotation[0]);
	cylinderActor->RotateY(rotation[1]);
	cylinderActor->RotateZ(rotation[2]);

	return cylinderActor;
}

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








/*
void PendulumPainter::cC()
{
	this->ui->centralwidget->setStyleSheet("background-color: black");
	
}
*/




