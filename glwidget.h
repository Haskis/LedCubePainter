#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QDebug>
#include <GL/glut.h>
#include <QtCore>
#include <qextserialport.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    int normalizeAngle(int);

    char outBuffer[64];
    QextSerialPort * port;
void updateCubeTable();
signals:
    
public slots:
    void toggleBackgroundColor(bool toBlack);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void updatePlane0(bool value);
    void updatePlane1(bool value);
    void updatePlane2(bool value);
    void updatePlane3(bool value);
    void updatePlane4(bool value);
    void updatePlane5(bool value);
    void updatePlane6(bool value);
    void updatePlane7(bool value);

    void allDiodesOn();
    void allDiodesOff();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



private:


    void initSerialPort();




    int selection(int x, int y);

    int xRot;
    int yRot;
    int zRot;

    int diodesCounter;
    bool paintingValue;

    GLdouble orthoLeft;
    GLdouble orthoRight;
    GLdouble orthoBottom;
    GLdouble orthoTop;
    GLdouble orthoNear;
    GLdouble ortoFar;

    bool plane[8];
    int ledsOn[512];


};

#endif // GLWIDGET_H
