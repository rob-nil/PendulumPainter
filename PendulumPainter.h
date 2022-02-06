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
#ifndef PendulumPainter_H
#define PendulumPainter_H

#include "vtkSmartPointer.h" // Required for smart pointer internal ivars.
#include <QMainWindow>
#include <QMessageBox>
#include "math.h"
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkAssembly.h>
#include <vtkNamedColors.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vector>
#include <QTimer>	// Start Stop Event
#include <QProgressBar>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkPlaneSource.h>
#include <string>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>

// Basics
using namespace std;
typedef vector<vector<double>> matrix;

// Forward Qt class declarations
class Ui_PendulumPainter;

class PendulumPainter : public QMainWindow
{
  Q_OBJECT

public:
  // Constructor
  PendulumPainter();

  // Destructor
  ~PendulumPainter() override;

public slots:
  //Public Functions
  virtual void slotOpenFile();	//Open PNG File
  virtual void slotExit();		//Close Program
  virtual void msgBox();		//Show Message Box
  virtual void pushButtonSim();
  virtual void timerslot();
  virtual void initialize();	// reads and assigns the values from the UI
  virtual void getSliderValue();
  virtual void saveImage();
  virtual void changeColor();
  virtual void changeColorDefault();

protected:

	// General Variables
	bool run;
	bool computing;
	int numIncr;
	int simSpeedms;
	int timerms;
	int linewidth;
	string imageFilePath;
	double lineColor[3];

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
	void setCalData(matrix& matCal);
	void runCalSphericalPendulum();
	vector<double> getDataGUI();

protected slots:

private:
  // Designer form
  Ui_PendulumPainter* ui;

};

#endif // PendulumPainter_H
