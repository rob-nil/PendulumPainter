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
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <QFileDialog.h>


// Constructor
PendulumPainter::PendulumPainter()
{
  this->ui = new Ui_PendulumPainter;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

  // Geometry
  vtkNew<vtkVectorText> text;
  text->SetText("Cone and Sphere");
  vtkNew<vtkElevationFilter> elevation;
  elevation->SetInputConnection(text->GetOutputPort());
  elevation->SetLowPoint(0, 0, 0);
  elevation->SetHighPoint(5, 0, 0);

  vtkSphereSource* sphere = vtkSphereSource::New();
  sphere->SetCenter(10, -5.0, 0.0);
  sphere->SetRadius(1.0);
  sphere->SetThetaResolution(30);
  sphere->SetPhiResolution(30);

  vtkConeSource* cone = vtkConeSource::New();
  cone->SetCenter(0.0, -5.0, 0.0);
  cone->SetHeight(3.0);
  cone->SetRadius(1.0);
  cone->SetResolution(50);


  // Mapper
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(elevation->GetOutputPort());
 
  vtkPolyDataMapper* sphereMapper = vtkPolyDataMapper::New();
  sphereMapper->SetInputConnection(sphere->GetOutputPort());

  vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInputConnection(cone->GetOutputPort());

  // Actor in scene
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  
  vtkActor* sphereActor = vtkActor::New();
  sphereActor->SetMapper(sphereMapper);

  vtkActor* coneActor = vtkActor::New();
  coneActor->SetMapper(coneMapper);


  // VTK Renderer
  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderer> ren2;

  // Add Actor to renderer
  ren->AddActor(actor);

  ren2->AddActor(sphereActor);
  ren2->AddActor(coneActor);

  // VTK/Qt wedded
  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
  this->ui->qvtkWidget->setRenderWindow(renderWindow);
  this->ui->qvtkWidget->renderWindow()->AddRenderer(ren);

  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow2;
  this->ui->qvtkWidget2->setRenderWindow(renderWindow2);
  this->ui->qvtkWidget2->renderWindow()->AddRenderer(ren2);
  

  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
  vtkNew<vtkDataObjectToTable> toTable;
  toTable->SetInputConnection(elevation->GetOutputPort());
  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

  // Here we take the end of the VTK pipeline and give it to a Qt View
  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotExit()));
  connect(this->ui->pushButtonMsgBox, SIGNAL(clicked()), this, SLOT(msgBox()));
  connect(this->ui->actionMsgBox, SIGNAL(triggered()), this, SLOT(msgBox()));
  connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(cC()));

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


void PendulumPainter::slotExit()
{
  qApp->exit();
}

void PendulumPainter::msgBox()
{
	QMessageBox msgBox;
	msgBox.setText("Cone and Sphere");
	msgBox.exec();

}

void PendulumPainter::cC()
{
	this->ui->centralwidget->setStyleSheet("background-color: black");
	
}




