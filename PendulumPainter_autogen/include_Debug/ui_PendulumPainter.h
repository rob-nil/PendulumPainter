/********************************************************************************
** Form generated from reading UI file 'PendulumPainter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PENDULUMPAINTER_H
#define UI_PENDULUMPAINTER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLStereoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_PendulumPainter
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QAction *actionMsgBox;
    QWidget *centralwidget;
    QGroupBox *groupBoxGraph;
    QVBoxLayout *vboxLayout;
    QSplitter *splitter;
    QFrame *tableFrame;
    QVBoxLayout *vboxLayout1;
    QVTKOpenGLStereoWidget *qvtkWidget;
    QPushButton *pushButtonClose;
    QPushButton *pushButtonMsgBox;
    QGroupBox *groupBoxGraph_2;
    QVBoxLayout *_2;
    QSplitter *splitter_2;
    QVTKOpenGLStereoWidget *qvtkWidget2;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *PendulumPainter)
    {
        if (PendulumPainter->objectName().isEmpty())
            PendulumPainter->setObjectName(QString::fromUtf8("PendulumPainter"));
        PendulumPainter->resize(957, 516);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        PendulumPainter->setWindowIcon(icon);
        PendulumPainter->setIconSize(QSize(22, 22));
        actionOpenFile = new QAction(PendulumPainter);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon1);
        actionExit = new QAction(PendulumPainter);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionPrint = new QAction(PendulumPainter);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon2);
        actionHelp = new QAction(PendulumPainter);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setIcon(icon);
        actionSave = new QAction(PendulumPainter);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionMsgBox = new QAction(PendulumPainter);
        actionMsgBox->setObjectName(QString::fromUtf8("actionMsgBox"));
        centralwidget = new QWidget(PendulumPainter);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBoxGraph = new QGroupBox(centralwidget);
        groupBoxGraph->setObjectName(QString::fromUtf8("groupBoxGraph"));
        groupBoxGraph->setGeometry(QRect(30, 20, 441, 381));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBoxGraph->sizePolicy().hasHeightForWidth());
        groupBoxGraph->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(groupBoxGraph);
        vboxLayout->setSpacing(0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(groupBoxGraph);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tableFrame = new QFrame(splitter);
        tableFrame->setObjectName(QString::fromUtf8("tableFrame"));
        vboxLayout1 = new QVBoxLayout(tableFrame);
        vboxLayout1->setSpacing(0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        splitter->addWidget(tableFrame);
        qvtkWidget = new QVTKOpenGLStereoWidget(splitter);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
        sizePolicy.setHeightForWidth(qvtkWidget->sizePolicy().hasHeightForWidth());
        qvtkWidget->setSizePolicy(sizePolicy);
        qvtkWidget->setMinimumSize(QSize(300, 300));
        splitter->addWidget(qvtkWidget);

        vboxLayout->addWidget(splitter);

        pushButtonClose = new QPushButton(centralwidget);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(610, 410, 131, 31));
        pushButtonMsgBox = new QPushButton(centralwidget);
        pushButtonMsgBox->setObjectName(QString::fromUtf8("pushButtonMsgBox"));
        pushButtonMsgBox->setGeometry(QRect(470, 410, 141, 31));
        groupBoxGraph_2 = new QGroupBox(centralwidget);
        groupBoxGraph_2->setObjectName(QString::fromUtf8("groupBoxGraph_2"));
        groupBoxGraph_2->setGeometry(QRect(490, 20, 441, 381));
        sizePolicy.setHeightForWidth(groupBoxGraph_2->sizePolicy().hasHeightForWidth());
        groupBoxGraph_2->setSizePolicy(sizePolicy);
        _2 = new QVBoxLayout(groupBoxGraph_2);
        _2->setSpacing(0);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(groupBoxGraph_2);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        qvtkWidget2 = new QVTKOpenGLStereoWidget(splitter_2);
        qvtkWidget2->setObjectName(QString::fromUtf8("qvtkWidget2"));
        sizePolicy.setHeightForWidth(qvtkWidget2->sizePolicy().hasHeightForWidth());
        qvtkWidget2->setSizePolicy(sizePolicy);
        qvtkWidget2->setMinimumSize(QSize(300, 300));
        splitter_2->addWidget(qvtkWidget2);

        _2->addWidget(splitter_2);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(332, 410, 131, 31));
        PendulumPainter->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PendulumPainter);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 957, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        PendulumPainter->setMenuBar(menubar);
        statusbar = new QStatusBar(PendulumPainter);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        PendulumPainter->setStatusBar(statusbar);
        toolBar = new QToolBar(PendulumPainter);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setOrientation(Qt::Horizontal);
        toolBar->setIconSize(QSize(22, 22));
        PendulumPainter->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpenFile);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addAction(actionHelp);
        menuFile->addAction(actionExit);
        menuFile->addSeparator();
        menuFile->addAction(actionMsgBox);
        toolBar->addAction(actionOpenFile);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionPrint);
        toolBar->addAction(actionHelp);

        retranslateUi(PendulumPainter);

        QMetaObject::connectSlotsByName(PendulumPainter);
    } // setupUi

    void retranslateUi(QMainWindow *PendulumPainter)
    {
        PendulumPainter->setWindowTitle(QCoreApplication::translate("PendulumPainter", "PendulumPainter", nullptr));
        actionOpenFile->setText(QCoreApplication::translate("PendulumPainter", "Open File...", nullptr));
        actionExit->setText(QCoreApplication::translate("PendulumPainter", "Exit", nullptr));
        actionPrint->setText(QCoreApplication::translate("PendulumPainter", "Print", nullptr));
        actionHelp->setText(QCoreApplication::translate("PendulumPainter", "Help", nullptr));
        actionSave->setText(QCoreApplication::translate("PendulumPainter", "Save", nullptr));
        actionMsgBox->setText(QCoreApplication::translate("PendulumPainter", "MsgBox", nullptr));
        groupBoxGraph->setTitle(QCoreApplication::translate("PendulumPainter", "Views", nullptr));
        pushButtonClose->setText(QCoreApplication::translate("PendulumPainter", "Close", nullptr));
        pushButtonMsgBox->setText(QCoreApplication::translate("PendulumPainter", "MessageBox", nullptr));
        groupBoxGraph_2->setTitle(QCoreApplication::translate("PendulumPainter", "Views", nullptr));
        pushButton->setText(QCoreApplication::translate("PendulumPainter", "Darkmode", nullptr));
        menuFile->setTitle(QCoreApplication::translate("PendulumPainter", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PendulumPainter: public Ui_PendulumPainter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PENDULUMPAINTER_H
