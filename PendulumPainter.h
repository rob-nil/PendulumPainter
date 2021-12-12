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


// Forward Qt class declarations
class Ui_PendulumPainter;

// Forward VTK class declarations
class vtkQtTableView;

class PendulumPainter : public QMainWindow
{
  Q_OBJECT

public:
  // Constructor/Destructor
  PendulumPainter();
  ~PendulumPainter() override;

public slots:

  virtual void slotOpenFile();	//Open PNG File
  virtual void slotExit();		//Close Program
  virtual void msgBox();		//Show Message Box
  virtual void cC();			//Change Background Colour

protected:
protected slots:

private:
  vtkSmartPointer<vtkQtTableView> TableView;

  // Designer form
  Ui_PendulumPainter* ui;
};

#endif // PendulumPainter_H
