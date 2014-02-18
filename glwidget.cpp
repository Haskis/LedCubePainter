#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    xRot(0), yRot(0), zRot(0),
    orthoLeft(-25), orthoRight(25),
    orthoBottom(-25), orthoTop(25),
    orthoNear(-25), ortoFar(25),
    diodesCounter(0)
{
    //All diodes off at start
    for(int i=0;i<512;i++)
        ledsOn[i]=false;

    //All planes visible at start
    for(int i=0;i<8;i++)
        plane[i]=1;

    //Initialize port
    port=new QextSerialPort(QextSerialPort::EventDriven,this);
    port->close();
}

//Initialize serial Port after clicking button in initSerialPortWidget
void GLWidget::initSerialPort(QString portURL){
    port = new QextSerialPort(portURL);
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);
    port->close();
    if (port->open(QIODevice::ReadWrite) == true){
        qDebug("Port Opened");
    }
    else{
        qDebug("Port Not Opened");
    }
}
void GLWidget::allDiodesOn(){
    for(int i=0;i<512;i++)
        ledsOn[i]=true;

    if(port->isOpen())
    {
        updateCubeTable();
        port->write(outBuffer,64);
    }
    else
        qDebug()<<"Not opened";

    updateGL();
}
void GLWidget::allDiodesOff(){
    for(int i=0;i<512;i++)
        ledsOn[i]=false;

    if(port->isOpen())
    {
        updateCubeTable();
        port->write(outBuffer,64);
    }
    else
        qDebug()<<"Not opened";

    updateGL();
}

void GLWidget::initializeGL(){
    glEnable( GL_DEPTH_TEST );
    //glEnable( GL_LIGHTING );
   // glEnable( GL_LIGHT0 );
}

void GLWidget::resizeGL(int width, int height){
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

//Paints 3D cube model
void GLWidget::paintGL(){
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
            for(int z=0;z<8;z++){
                if(plane[y]==1){
                    glLoadIdentity();

                    glRotated(xRot, 1.0, 0.0, 0.0);
                    glRotated(yRot, 0.0, 1.0, 0.0);
                    glRotated(zRot, 0.0, 0.0, 1.0);
                    glTranslatef(-14+4*x, -14+4*y,-14+4*z);

                    glLoadName(diodesCounter);

                    if(ledsOn[z+8*x+64*y]==true)
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
}
//Background color change
void GLWidget::toggleBackgroundColor(bool toBlack)
{
    if(toBlack){
        glClearColor(0,0,0,1);
    }
    else{
        glClearColor(1,1,1,1);
    }
    updateGL();
}
//Rotation Functions
void GLWidget::setXRotation(int angle){
    if (angle-50 != xRot){
        xRot = angle-50;
        updateGL();
    }
}

void GLWidget::setYRotation(int angle){
    if (angle-50 != yRot){
        yRot = angle-50;
        updateGL();
    }
}

void GLWidget::setZRotation(int angle){
    if (angle-50 != zRot){
        zRot = angle-50;
        updateGL();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    int diodeNumber;
    int mouseX= event->x();
    int mouseY= event->y();

    diodeNumber= selection(mouseX, mouseY);
    //If any diode was clicked
    if(diodeNumber>=0){
        if(ledsOn[diodeNumber]==false){
            ledsOn[diodeNumber]=true;
            paintingValue=true;
        }
        else{
            paintingValue=false;
            ledsOn[diodeNumber]=false;
        }
    }
    if(port->isOpen())
    {
        updateCubeTable();
        port->write(outBuffer,64);
    }
    else
        qDebug()<<"Not opened";
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        int mouseX= event->x();
        int mouseY= event->y();

        int diodeNumber= selection(mouseX, mouseY);

        if(diodeNumber>=0){
            ledsOn[diodeNumber]=paintingValue;
            if(port->isOpen()){
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

    // Badanie wyniku selekcji
    if(hits !=0 ){
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
void GLWidget::updatePlane0(bool value){
    plane[0]=value;
    updateGL();
}

void GLWidget::updatePlane1(bool value){
     plane[1]=value;
     updateGL();
}

void GLWidget::updatePlane2(bool value){
     plane[2]=value;
     updateGL();
}

void GLWidget::updatePlane3(bool value){
     plane[3]=value;
     updateGL();
}

void GLWidget::updatePlane4(bool value){
     plane[4]=value;
     updateGL();
}

void GLWidget::updatePlane5(bool value){
     plane[5]=value;
     updateGL();
}

void GLWidget::updatePlane6(bool value){
     plane[6]=value;
     updateGL();
}

void GLWidget::updatePlane7(bool value){
     plane[7]=value;
     updateGL();
}

//Update USART buffer
void GLWidget::updateCubeTable(){
   for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            for(int k=0;k<8;k++)
                if(ledsOn[64*i+8*j+k]==true)
                    outBuffer[8*i+j] |= (1<<k);
                else
                    outBuffer[8*i+j] &= ~(1<<k);

}
