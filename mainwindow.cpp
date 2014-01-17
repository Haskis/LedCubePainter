#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    connect(ui->checkBox,SIGNAL(toggled(bool)),ui->widget,SLOT(toggleBackgroundColor(bool)));

    //Connect Sliders with functions for changing view point
    connect(ui->sliderX,SIGNAL(valueChanged(int)),ui->widget,SLOT(setXRotation(int)));
    connect(ui->sliderY,SIGNAL(valueChanged(int)),ui->widget,SLOT(setYRotation(int)));
    connect(ui->sliderZ,SIGNAL(valueChanged(int)),ui->widget,SLOT(setZRotation(int)));

    connect(ui->sliderX,SIGNAL(valueChanged(int)),ui->labelX,SLOT(setNum(int)));
    connect(ui->sliderY,SIGNAL(valueChanged(int)),ui->labelY,SLOT(setNum(int)));
    connect(ui->sliderZ,SIGNAL(valueChanged(int)),ui->labelZ,SLOT(setNum(int)));

    connect(ui->checkBoxPlane0,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane0(bool)));
    connect(ui->checkBoxPlane1,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane1(bool)));
    connect(ui->checkBoxPlane2,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane2(bool)));
    connect(ui->checkBoxPlane3,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane3(bool)));
    connect(ui->checkBoxPlane4,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane4(bool)));
    connect(ui->checkBoxPlane5,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane5(bool)));
    connect(ui->checkBoxPlane6,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane6(bool)));
    connect(ui->checkBoxPlane7,SIGNAL(toggled(bool)),ui->widget,SLOT(updatePlane7(bool)));

    connect(ui->pushButtonAllOn,SIGNAL(clicked()),ui->widget,SLOT(allDiodesOn()));
    connect(ui->pushButtonAllOff,SIGNAL(clicked()),ui->widget,SLOT(allDiodesOff()));




}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_SEND_clicked()
{
    if(ui->widget->port->isOpen())
    {
        ui->widget->updateCubeTable();


        ui->widget->port->write(ui->widget->outBuffer,64);
        //if (ui->widget->port->bytesAvailable())
       // qDebug()<<ui->widget->port->readAll();

        //ui->widget->port->write("0000000011111111222222223333333344444444555555556666666677777777");
        //port->write(array);
        //qDebug()<<ui->widget->port->readAll();

        //qDebug()<<outBuffer[0];
        //qDebug()<<outBuffer[1];
        //qDebug()<<outBuffer[2];
    }
}
