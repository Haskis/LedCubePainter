#ifndef SERIALPORTOPTIONSWIDGET_H
#define SERIALPORTOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
class SerialPortOptionsWidget;
}

class SerialPortOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortOptionsWidget(QWidget *parent = 0);
    ~SerialPortOptionsWidget();
signals:
    void openSerialPort(QString portURL);
private slots:
    void pushButtonInitSerialPortClicked();
private:
    Ui::SerialPortOptionsWidget *ui;
};

#endif //SERIALPORTOPTIONSWIDGET_H
