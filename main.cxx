/*----------------------------------------------------------------------------------------------

 Name:		Julian Kastenhuber
			Patrick Holzer
 Date:		02-2022
 Project:	VIS3IL - Pendulum Painter

 ---------------------------------------------------------------------------------------------*/

// Qt includes
#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QApplication>
#include "QVTKOpenGLStereoWidget.h"

// Other includes
#include "PendulumPainter.h"

// Basics
using namespace std;
typedef vector<vector<double>> matrix;

extern int qInitResources_icons();

// Main
int main(int argc, char** argv)
{
	// needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLStereoWidget::defaultFormat());

	// QT Stuff
	QApplication app(argc, argv);
	QApplication::setStyle("fusion");
	qInitResources_icons();

	// PendulumPainter Obeject
	PendulumPainter myPendulumPainter;
	myPendulumPainter.show();
	
	return app.exec();
 }