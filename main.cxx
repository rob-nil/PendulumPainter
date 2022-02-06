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

typedef vector<vector<double>> matrix;

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
	
  return app.exec();
 }