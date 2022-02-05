/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */
// QT includes
#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QApplication>

#include "QVTKOpenGLStereoWidget.h"
#include "PendulumPainter.h"
#include "SphericalPendulum.h"

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
extern int qInitResources_icons();

int main(int argc, char** argv)
{
	// needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLStereoWidget::defaultFormat());

	// QT Stuff
	QApplication app(argc, argv);

	QApplication::setStyle("fusion");

	qInitResources_icons();

	PendulumPainter myPendulumPainter;
	myPendulumPainter.show();


  // TESTING: _______________________________________________________________

	// Seas,
	// du kannst dir mit meine get methoden die Startewerte holen:
	// -getStartVec()
	// -getStartV()
	// -getPendulumLenght()
	// 
	// dann bitte die Vektoren "vecDiffAngle" und "vecDraw" berechnen und die meine Klasse dann
	// über setData() aufnimmt. 
	
	vector<double> initStateVector = { 1.0, 0.0, 1.0, 0.1 }; // austauschen bzw. Umrechnungsfunktion anwenden
	vector<double> dampingCoefficients = { 0.1, 0.1 };
	vector<double> timeSettings = { 0, 10, 0.1 };
	SphericalPendulum mySphericalPendulum(myPendulumPainter.getPendulumLenght(), initStateVector, dampingCoefficients, timeSettings);
	//mySphericalPendulum.setPendulumLength(myPendulumPainter.getPendulumLenght());
	//mySphericalPendulum.setInitState(initStateVector);
	mySphericalPendulum.integrateODE();
	vector<vec3D> vecDiffAngle;
	vector<vec3D> vecDraw;

	myPendulumPainter.setData(vecDiffAngle, vecDraw);

  /*
  // Additional Testing Class
  PendulumPainter_Test PendulumPainter_Test;
  PendulumPainter_Test.printTest(5);


  // Generate test data () Time Vector and vektor3D of angles
  // struct to save angle of Pendulum in 3D


  const int n = 500;
  vector<double> vecTime(n,0);
  vector<vec3D> vecAngle;
  vector<vec3D> vecDiffAngle;
  vector<vec3D> vecDraw;
  
  
  vecTime[0] = 0;

  // Initialize Time Vector
  for (int i = 0; i < n-2; i++)
  {
	  vecTime[i+1]= vecTime[i] + 0.1;
  }

  // Initialize Angle Vector
  for (int i = 0; i < n-1; i++)
  {
	  vecAngle.push_back(vec3D());	// element hinzufügen
	  vecAngle[i].x = sin(vecTime[i])*180/3.1415;
	  vecAngle[i].y = 0;
	  vecAngle[i].z = cos(vecTime[i]) * 180 / 3.1415;
  }

  // Initialize Gradient Angle Vector
  for (int i = 0; i < n - 2; i++)
  {
	  vecDiffAngle.push_back(vec3D());	// element hinzufügen
	  vecDiffAngle[i].x = vecAngle[i + 1].x - vecAngle[i].x;
	  vecDiffAngle[i].y = vecAngle[i + 1].y - vecAngle[i].y;
	  vecDiffAngle[i].z = vecAngle[i + 1].z - vecAngle[i].z;
	  //cout << arrDiffAngle[i].x << endl;
  }

 // Points on Vector to Draw on Surface
  for (int i = 0; i < n - 1; i++)
  {
	  vecDraw.push_back(vec3D());	// element hinzufügen
	  vecDraw[i].x = vecTime[i] * sin(vecTime[i]);
	  vecDraw[i].y = 0;
	  vecDraw[i].z = vecTime[i] * cos(vecTime[i]);
  }
  
  std::cout << "Length of vecDiffAngle: " << vecDiffAngle.size() << endl;
  std::cout << "Length of vecDraw: " << vecDraw.size() << endl;
  std::cout << "Length of vecAngle: " << vecDraw.size() << endl;


  myPendulumPainter.setData(vecDiffAngle, vecDraw);
  myPendulumPainter.initialize();


  // _________________________________________________________________________

  return app.exec();
}
*/


