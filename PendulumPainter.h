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

 
// Forward Qt class declarations
class Ui_PendulumPainter;


class PendulumPainter : public QMainWindow
{
  Q_OBJECT

public:
  // Constructor/Destructor
  PendulumPainter();
  ~PendulumPainter() override;

  // functions
  vtkActor* makeCylinder(double radius, double heigth, double position[3], double rotation[3], int resolution);
  vtkActor* makeCone(double radius, double heigth, double position[3], double rotation[3], int resolution);
  vtkActor* makeSphere(double radius, double position[3]);

public slots:

  virtual void slotOpenFile();	//Open PNG File
  virtual void slotExit();		//Close Program
  virtual void msgBox();		//Show Message Box

protected:
protected slots:

private:
  // Designer form
  Ui_PendulumPainter* ui;
};

#endif // PendulumPainter_H
