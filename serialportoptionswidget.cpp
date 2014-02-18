#include "serialportoptionswidget.h"
#include "ui_serialportoptionswidget.h"

SerialPortOptionsWidget::SerialPortOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortOptionsWidget)
{
    ui->setupUi(this);
    connect(this->ui->pushButtonOpenSerialPort,SIGNAL(clicked()),this,SLOT(pushButtonInitSerialPortClicked()));
}

SerialPortOptionsWidget::~SerialPortOptionsWidget()
{
    delete ui;
}
void SerialPortOptionsWidget::pushButtonInitSerialPortClicked(){
    emit openSerialPort(this->ui->lineEditOpenSerialPort->text());
}

