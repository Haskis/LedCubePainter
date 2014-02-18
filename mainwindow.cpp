#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serialPortWidget(NULL)
{
    ui->setupUi(this);

    //Check box to switch background color between black and white
    connect(ui->checkBox,SIGNAL(toggled(bool)),ui->widget,SLOT(toggleBackgroundColor(bool)));

    //Connect Sliders with functions for changing view point
    connect(ui->sliderX,SIGNAL(valueChanged(int)),ui->widget,SLOT(setXRotation(int)));
    connect(ui->sliderY,SIGNAL(valueChanged(int)),ui->widget,SLOT(setYRotation(int)));
    connect(ui->sliderZ,SIGNAL(valueChanged(int)),ui->widget,SLOT(setZRotation(int)));

    //Connect Sliders with functions for displaying slider values
    connect(ui->sliderX,SIGNAL(valueChanged(int)),ui->labelX,SLOT(setNum(int)));
    connect(ui->sliderY,SIGNAL(valueChanged(int)),ui->labelY,SLOT(setNum(int)));
    connect(ui->sliderZ,SIGNAL(valueChanged(int)),ui->labelZ,SLOT(setNum(int)));

    //Connect check boxes with functions for displaying planes on cube model
    connect(ui->checkBoxPlane0,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane0(bool)));
    connect(ui->checkBoxPlane1,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane1(bool)));
    connect(ui->checkBoxPlane2,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane2(bool)));
    connect(ui->checkBoxPlane3,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane3(bool)));
    connect(ui->checkBoxPlane4,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane4(bool)));
    connect(ui->checkBoxPlane5,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane5(bool)));
    connect(ui->checkBoxPlane6,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane6(bool)));
    connect(ui->checkBoxPlane7,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane7(bool)));

    //Special buttons to switch on/off all diodes at once
    connect(ui->pushButtonAllOn,SIGNAL(clicked()),ui->widget,SLOT(allDiodesOn()));
    connect(ui->pushButtonAllOff,SIGNAL(clicked()),ui->widget,SLOT(allDiodesOff()));

    //Shows SerialPortWidget after action click
    connect(ui->actionInitializeSerialPort,SIGNAL(triggered()),this,SLOT(showInitSerialPortWidget()));
}

void MainWindow::showInitSerialPortWidget(){
   if(serialPortWidget==NULL)
       serialPortWidget=new SerialPortOptionsWidget;
   serialPortWidget->show();
   connect(this->serialPortWidget,SIGNAL(openSerialPort(QString)),this->ui->widget,SLOT(initSerialPort(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
