#include "glwidget.h"
#include "GL/glu.h"
#include <QDebug>




GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    xRot=0;
    yRot=0;
    zRot=0;
    diodesCounter=0;

    //Viewing options
    orthoLeft=-25;
    orthoRight=25;
    orthoBottom=-25;
    orthoTop=25;
    orthoNear=-25;
    ortoFar=25;

    for(int i=0;i<512;i++)
        ledsOn[i]=0;

    for(int i=0;i<8;i++)
        plane[i]=1;

    for(int i=0;i<64;i++)
        outBuffer[i]='@';
    for(int i=0;i<8;i++)
        outBuffer[i]='h';

    initSerialPort();

}


void GLWidget::initSerialPort()
{
    port = new QextSerialPort("/dev/ttyUSB0");
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);
    //port->setQueryMode(QextSerialPort::Polling);
    port->close();
    if (port->open(QIODevice::ReadWrite) == true)
    {
        qDebug("port is opened");
    }
    else
    {
        qDebug("port is not open");
    }
}


void GLWidget::allDiodesOn()
{
    for(int i=0;i<512;i++)
        ledsOn[i]=1;
    updateGL();
}

void GLWidget::allDiodesOff()
{
    for(int i=0;i<512;i++)
        ledsOn[i]=0;
    updateGL();

}



void GLWidget::initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_LIGHTING );
   // glEnable( GL_LIGHT0 );
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::paintGL()
{
    //Czyszczenie tła

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //Macierz modelu załadowanie
    glMatrixMode(GL_MODELVIEW);

    //Macierz modelu załadowanie jednostkowej macierzy
    glLoadIdentity();


    //Obszar renderingu
    int side = qMin(width(), height());
    glViewport((width() - side) / 2, (height() - side) / 2, side, side);

    glInitNames();
    glPushName(0);

    glColor3f(0,0,0.45);
    for(int y=0;y<8;y++)
        for(int x=0;x<8;x++)
            for(int z=0;z<8;z++)
            {
                if(plane[y]==1)
                {
                    glLoadIdentity();

                    glRotated(xRot, 1.0, 0.0, 0.0);
                    glRotated(yRot, 0.0, 1.0, 0.0);
                    glRotated(zRot, 0.0, 0.0, 1.0);
                    glTranslatef(-14+4*x, -14+4*y,-14+4*z);


                    glLoadName(diodesCounter);

                    if(ledsOn[z+8*x+64*y]!=0)
                        glColor3f(0.45,1,1);

                    glutSolidSphere(0.6,10,10);
                    glColor3f(0,0,0.5);
                }

                diodesCounter++;

            }


    glFlush();
    diodesCounter=0;


    glMatrixMode( GL_PROJECTION );

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();

    //Widok orto
    glOrtho(orthoLeft,orthoRight,orthoBottom,orthoTop,orthoNear,ortoFar);



    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //updateCubeTable();
    //QByteArray array;
    //for(int i=0;i<64;i++)
        //array[i]=0x45;
   // array[0]=0x00;
   // array[1]=0x01;
    //array[2]=0x01;
   // array[3]=0x05;

   // if(port->isOpen())
    //{
       // updateCubeTable();
        //port->write(outBuffer,64);
   // }







}
void GLWidget::toggleBackgroundColor(bool toBlack)
{
    if(toBlack)
    {
        glClearColor(0,0,0,1);

    }
    else
    {
        glClearColor(1,1,1,1);
    }
    updateGL();

}
//Rotation Functions
void GLWidget::setXRotation(int angle)
{

    if (angle-50 != xRot)
    {
        xRot = angle-50;

        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{

    if (angle-50 != yRot)
    {
        yRot = angle-50;

        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{

    if (angle-50 != zRot)
    {
        zRot = angle-50;

        updateGL();
    }
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    {
        int diodeNumber;
        int mouseX= event->x();
        int mouseY= event->y();

        diodeNumber= selection(mouseX, mouseY);
        //If any diode was clicked
        if(diodeNumber>=0)
        {
            if(ledsOn[diodeNumber]==0)
            {
                ledsOn[diodeNumber]=1;
                paintingValue=1;
            }
            else
            {
                paintingValue=0;
                ledsOn[diodeNumber]=0;
            }
        }

        if(port->isOpen())
        {
            updateCubeTable();
            port->write(outBuffer,64);
        }
        updateGL();

    }
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {

        int mouseX= event->x();
        int mouseY= event->y();

        int diodeNumber= selection(mouseX, mouseY);

        if(diodeNumber>=0)
        {

            ledsOn[diodeNumber]=paintingValue;
            if(port->isOpen())
            {
                updateCubeTable();
                port->write(outBuffer,64);
            }
        }




        updateGL();

    }
}

int GLWidget::selection( int x, int y )
{
    // wielkość bufora selekcji
    const int BUFFER_LENGTH = 2048;

    // bufor selekcji
    GLuint select_buffer[ BUFFER_LENGTH ];

    // przygotowanie bufora selekcji
    glSelectBuffer( BUFFER_LENGTH, select_buffer );

    // pobranie obszaru rozmiaru renderingu
    //glGetIntegerv ( z tego bierze, do tego zapisuje)
    //viewport[0]-x
    //viewport[1]-y
    //viewport[2]-width
    //wiewport[3]-hight
    int viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );

    // szerokość i wysokość obszaru renderingu
    int width = viewport[ 2 ];
    int height = viewport[ 3 ];

    // wybór macierzy rzutowania
    glMatrixMode( GL_PROJECTION );

    // odłożenie macierzy rzutowania na stos
    glPushMatrix();

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();

    // parametry bryły obcinania - jednostkowa kostka dookoła punktu wskaźnika
    // myszy (x,y) rozciągającej się na dwa piksele w poziomie i w pionie
    //GL(0,0) is bottom left, window (0,0) is top left (thats why height-y)

    gluPickMatrix( x, height-y, 2, 2, viewport );

    // parametry bryły obcinania

    glOrtho(orthoLeft,orthoRight,orthoBottom,orthoTop,orthoNear,ortoFar);

    // włączenie trybu selekcji
    glRenderMode( GL_SELECT );

    // generowanie sceny 3D


    paintGL();

    // zliczenie ilości rekordów trafień, powrót do domyślnego trybu renderingu
    GLint hits = glRenderMode( GL_RENDER );

    // wybór macierzy rzutowania
    glMatrixMode( GL_PROJECTION );

    // zdjęcie macierzy rzutowania ze stosu
    glPopMatrix();

    // w wyniku slekcji nie wybrano żadnego obiektu
    if(hits !=0 )
    {
        int temp=1;

        for(int i=1;i<hits;i++)
            if(select_buffer[temp]>select_buffer[i*4+1])
                temp=i*4+1;
        return select_buffer[temp+2];
    }
    else
        return -1;
}

//Set which planes are visible
void GLWidget::updatePlane0(bool value)
{
    plane[0]=value;
    updateGL();
}

void GLWidget::updatePlane1(bool value)
{
     plane[1]=value;
     updateGL();
}

void GLWidget::updatePlane2(bool value)
{
     plane[2]=value;
     updateGL();
}

void GLWidget::updatePlane3(bool value)
{
     plane[3]=value;
     updateGL();
}

void GLWidget::updatePlane4(bool value)
{
     plane[4]=value;
     updateGL();
}

void GLWidget::updatePlane5(bool value)
{
     plane[5]=value;
     updateGL();
}

void GLWidget::updatePlane6(bool value)
{
     plane[6]=value;
     updateGL();
}

void GLWidget::updatePlane7(bool value)
{
     plane[7]=value;
     updateGL();
}


void GLWidget::updateCubeTable()
{


   for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            for(int k=0;k<8;k++)
                if(ledsOn[64*i+8*j+k]==1)
                    outBuffer[8*i+j] |= (1<<k);
                else
                    outBuffer[8*i+j] &= ~(1<<k);

    //For debuging
//    for(int i=0;i<8;i++)
//        for(int j=0;j<8;j++)
//            if(cubeTable[i][j]==0)
//                qDebug()<<"0";
//            else
//                qDebug()<<cubeTable[i][j];

}
