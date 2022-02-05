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
using namespace std;

// Data Types
struct vec3D {
	double x;
	double y;
	double z;
};

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

  // Variables:
  //vtkNew<vtkAssembly> assembly;
  vtkAssembly* assembly;

  vtkNew<vtkLineSource> line3D;
  vtkNew<vtkPoints> points3D;

  vtkNew<vtkLineSource> line2D;
  vtkNew<vtkPoints> points2D;

  vector<vec3D> vecDiffAngleP;
  vector<vec3D> vecTimeP;
  vector<vec3D> vecDrawP;

  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderer> ren2D;

  vtkNew<vtkNamedColors> colors;
  QTimer* timer;
  vtkActor* line3DActor;
  vtkPolyDataMapper* line3DMapper;

  vtkActor* line2DActor;
  vtkPolyDataMapper* line2DMapper;

 
  vtkActor* coneActor ;
  vtkActor* planeActor ;
  vtkConeSource* cone ;
  vtkCylinderSource* cylinder ;
  vtkPolyDataMapper* cylinderMapper ;
  vtkActor* cylinderActor;
  vtkActor* cylinderActor2;
  vtkPolyDataMapper* coneMapper;
  vtkPlaneSource* plane;
  vtkPolyDataMapper* planeMapper;
  vtkSphereSource* sphere;
  vtkPolyDataMapper* sphereMapper;
  vtkActor* sphereActor;

  vtkSTLReader* stlReaderCup;
  vtkPolyDataMapper* cupMapper;
  vtkActor* cupActor;

  //QProgressBar* progressBar;

  bool run;
  int numIncr;
  int simSpeedms;
  int timerms;
  int linewidth;
  string imageFilePath;
 
  double vStart; 
  vec3D xyzStart;

  double initPosAss[3];
  double initOriAss[3];
  double* initPosAssbly;
  double* initOrAssbly;
  double lineColor[3];

  // Geometry parameter
  int resolution;
  int ConeGroundDist;
  double CylRadius;
  double pendulumLength;
  double ConeRadius;
  double ConeHeight;
  double cupRadius;
  double cupHeight;
  double cupscale;

  // Functions
  vtkActor* makeCylinder(double radius, double heigth, double position[3], double rotation[3], int resolution);
  vtkActor* makeCone(double radius, double heigth, double position[3], double rotation[3], int resolution);
  vtkActor* makeSphere(double radius, double position[3]);

  void rotUpdate3D(int i);
  void setData(vector<vec3D>, vector<vec3D>);
  void SimUpdate3D();
  void SimUpdate2D();
  vec3D getStartVec();
  double getStartV();
  double getPendulumLenght();
  void initSim();
  void init3DActors();

public slots:

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

protected slots:

private:
  // Designer form
  Ui_PendulumPainter* ui;

};



#endif // PendulumPainter_H
