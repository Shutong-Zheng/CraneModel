#include "cranemodel.h"
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include <QDebug>
#include <QColor>

const float PI = 3.1415926;

CraneModel::CraneModel(QWidget *parent):
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    omega = 0;
    theta = 15*3.14/180;
    l = 10;

    alpha = 10*3.14/180;
    phi = 30*3.14/180;
    d = 20;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
                omega += 3.14/10000;
                theta += 3.14/10000;
                l += 1;
                if(theta > 3.14 * 50/180)
                {
                    theta = -3.14 * 30/180;
                }
                if(l > 26){
                    l = 0;
                }
                updateGL();
            });  //一直旋转，主要看坐标设置部分
    timer->start(40);
}


void CraneModel::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
}

void CraneModel::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    glRotatef(yRot, 1.0, 0.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);
    drawCoordinate(-30, 50);
//    drawRec(omega);
    drawCylinder(6);
    drawFixedBracket(omega, 1.5);
    drawFixedBracket1(theta, omega, 1.5, 1.5, 1.75);
    drawStrenchBracket(theta, omega, 0.8, l);
    drawHook(theta, omega, l, 0.8, 4);
    drawObstacle(alpha, phi, d, 10);
    glPopMatrix();
}

void CraneModel::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((width() - side)/2, (height() - side)/2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4, 4, -2, 4, 3.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -50.0);
    glRotatef(-70, 1.0, 0.0, 0.0);
}

void CraneModel::drawCircle(float z, float r)
{
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glVertex3f(0.0f, 0.0f, z);
    int i = 0;
    for (i = 0; i <= 360; i += 1)
    {
        float p = i * 3.14 / 180;
        qglColor(QColor(128,128,0));
        glVertex3f(r*sin(p), r*cos(p), z);
    }
    glEnd();
}

// 绘制底座
void CraneModel::drawCylinder(float r)
{
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 1)
    {
        float p = i * 3.14 / 180;
        qglColor(QColor(85,107,47));
        glVertex3f(r*sin(p), r*cos(p), 1.0f);
        qglColor(QColor(128,128,0));
        glVertex3f(r*sin(p), r*cos(p), 0.0f);
    }
    glEnd();
    //bottom circle
    qglColor(QColor(85,107,47));
    drawCircle(0, r);
    //top circle
    qglColor(QColor(85,107,47));
    drawCircle(1, r);
}

//绘制不动杆
void CraneModel::drawFixedBracket(float omega, float d)
{
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    qglColor(QColor(128,128,0));
//    x = d/2;
//    y = d/2;
//    x = x*cos(omega) - y*sin(omega);
//    y = x*sin(omega) + y*cos(omega);
    glVertex3f(d/2*cos(omega) - d/2*sin(omega), d/2*sin(omega) + d/2*cos(omega), 1);
    qglColor(QColor(85,107,47));
    //中心点(-6.75*cos(omega),-6.75*sin(omega))
    glVertex3f(d/2*cos(omega) - d/2*sin(omega) -6.75*cos(omega),
               d/2*sin(omega) + d/2*cos(omega) - 6.75*sin(omega), 13);
    qglColor(QColor(128,128,0));
    glVertex3f(d/2*cos(omega) + d/2*sin(omega), d/2*sin(omega) -d/2*cos(omega), 1);
    qglColor(QColor(85,107,47));
    glVertex3f(d/2*cos(omega) + d/2*sin(omega) -6.75*cos(omega),
               d/2*sin(omega) - d/2*cos(omega) - 6.75*sin(omega), 13);
    qglColor(QColor(128,128,0));
    glVertex3f(-d/2*cos(omega) + d/2*sin(omega), -d/2*sin(omega) - d/2*cos(omega), 1);
    qglColor(QColor(85,107,47));
    glVertex3f(-d/2*cos(omega) + d/2*sin(omega) - 6.75*cos(omega),
               -d/2*sin(omega) - d/2*cos(omega) - 6.75*sin(omega), 13);
    qglColor(QColor(128,128,0));
    glVertex3f(-d/2*cos(omega) - d/2*sin(omega), -d/2*sin(omega) + d/2*cos(omega), 1);
    qglColor(QColor(85,107,47));
    glVertex3f(- d/2*cos(omega) - d/2*sin(omega)-6.75*cos(omega),
               - d/2*sin(omega) + d/2*cos(omega)-6.75*sin(omega), 13);
    qglColor(QColor(128,128,0));
    glVertex3f(d/2*cos(omega) - d/2*sin(omega), d/2*sin(omega) + d/2*cos(omega), 1);
    qglColor(QColor(85,107,47));
    glVertex3f(d/2*cos(omega) - d/2*sin(omega)-6.75*cos(omega),
               d/2*sin(omega) + d/2*cos(omega)-6.75*sin(omega), 13);
    glEnd();

//    glBegin(GL_LINES);
//    qglColor(Qt::black);
//    glVertex3f(d/2*cos(omega) - d/2*sin(omega), d/2*sin(omega) + d/2*cos(omega), 1);
//    glVertex3f(d/2*cos(omega) - d/2*sin(omega) -6.75*cos(omega),
//               d/2*sin(omega) + d/2*cos(omega) - 6.75*sin(omega), 13);
//    glVertex3f(d/2*cos(omega) + d/2*sin(omega), d/2*sin(omega) -d/2*cos(omega), 1);
//    glVertex3f(d/2*cos(omega) + d/2*sin(omega) -6.75*cos(omega),
//               d/2*sin(omega) - d/2*cos(omega) - 6.75*sin(omega), 13);
//    glVertex3f(-d/2*cos(omega) + d/2*sin(omega), -d/2*sin(omega) - d/2*cos(omega), 1);
//    glVertex3f(-d/2*cos(omega) + d/2*sin(omega) - 6.75*cos(omega),
//               -d/2*sin(omega) - d/2*cos(omega) - 6.75*sin(omega), 13);
//    glVertex3f(-d/2*cos(omega) - d/2*sin(omega), -d/2*sin(omega) + d/2*cos(omega), 1);
//    glVertex3f(- d/2*cos(omega) - d/2*sin(omega)-6.75*cos(omega),
//               - d/2*sin(omega) + d/2*cos(omega)-6.75*sin(omega), 13);
//    glEnd();
}

void CraneModel::drawFixedBracket1(float theta, float omega, float d, float d1, float r)
{
//    x = d/2;
//    y = d/2;
//    x = x*cos(omega) - y*sin(omega);
//    y = x*sin(omega) + y*cos(omega);
    //绘制一个圆柱
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    int i = 0;
    for (i = 0; i <= 360; i += 1)
    {
        float p = i * 3.14 / 180;
        qglColor(QColor(85,107,47));
        //(-6.75*cos(omega) + r*sin(p), d1 - 6.75*sin(omega), r*cos(p) + 13)
        glVertex3f(r*sin(p)*cos(omega) - d1*sin(omega) - 6.75*cos(omega),
                   r*sin(p)*sin(omega) + d1*cos(omega) - 6.75*sin(omega), r*cos(p) + 13);
        qglColor(QColor(128,128,0));
        //-6.75*cos(omega) + r*sin(p), -d1 - 6.75*sin(omega)
        glVertex3f(r*sin(p)*cos(omega) + d1*sin(omega) - 6.75*cos(omega),
                   r*sin(p)*sin(omega) -d1*cos(omega) - 6.75*sin(omega), r*cos(p) + 13);
    }
    glEnd();
    //bottom circle
    qglColor(QColor(85,107,47));
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glVertex3f(-6.75*cos(omega), -d1 - 6.75*sin(omega), 13);
    for (i = 0; i <= 360; i += 1)
    {
        float p = i * 3.14 / 180;
        qglColor(QColor(128,128,0));
        //-6.75*cos(omega) + r*sin(p), -d1 - 6.75*sin(omega)
        glVertex3f(r*sin(p)*cos(omega) + d1*sin(omega) - 6.75*cos(omega),
                   r*sin(p)*sin(omega) - d1*cos(omega)- 6.75*sin(omega), r*cos(p) + 13);
    }
    glEnd();
    //top circle
    qglColor(QColor(85,107,47));
    glBegin(GL_TRIANGLE_FAN);           //扇形连续填充三角形串
    glVertex3f(-6.75*cos(omega), d1 - 6.75*sin(omega), 13);
    for (i = 0; i <= 360; i += 1)
    {
        float p = i * 3.14 / 180;
        qglColor(QColor(128,128,0));
        //-6.75*cos(omega) + r*sin(p), d1 - 6.75*sin(omega)
        glVertex3f(r*sin(p)*cos(omega) - d1*sin(omega)-6.75*cos(omega),
                   r*sin(p)*sin(omega) + d1*cos(omega)- 6.75*sin(omega), r*cos(p) + 13);
    }
    glEnd();

    //相当于L=24
    float xp = (24*cos(theta) - 6.75)*cos(omega);
    float yp = (24*cos(theta) - 6.75)*sin(omega);
    float zp = 24*sin(theta) + 13;
    //    x = x*cos(omega) - y*sin(omega);
    //    y = x*sin(omega) + y*cos(omega);
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    //绕着-6.75*cos(omega)，-6.75*sin(omega)
    qglColor(QColor(128,128,0));
    //-6.75*cos(omega) + d/2, -6.75*sin(omega) + d/2, 13
    glVertex3f(-6.75*cos(omega) + d/2*sin(omega), -6.75*sin(omega) - d/2*cos(omega) , 13);  //a
    qglColor(QColor(85,107,47));
    //xp + d/2*sin(theta), yp + d/2, zp - d/2*cos(theta)
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    qglColor(QColor(128,128,0));
    //-6.75*cos(omega) + d/2, -6.75*sin(omega) - d/2, 13
    glVertex3f(-6.75*cos(omega) - d/2*sin(omega), -6.75*sin(omega) + d/2*cos(omega), 13);   //b
    qglColor(QColor(85,107,47));
    //xp + d/2*sin(theta), yp - d/2, zp - d/2*cos(theta)
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    qglColor(QColor(128,128,0));
    //-6.75*cos(omega) + d/2 - d/2*sin(theta), -6.75*sin(omega) - d/2, 13 + d/2*cos(theta)
    glVertex3f(-6.75*cos(omega) - d*cos(omega)*sin(theta) - d/2*sin(omega),
               -6.75*sin(omega) - d*sin(omega)*sin(theta) + d/2*cos(omega), 13 + d*cos(theta));//c
    qglColor(QColor(85,107,47));
    //xp - d/2*sin(theta), yp - d/2, zp + d/2*cos(theta)
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    qglColor(QColor(128,128,0));
    //-6.75*cos(omega) + d/2 - d/2*sin(theta), -6.75*sin(omega) + d/2, 13 + d/2*cos(theta)
    glVertex3f(-6.75*cos(omega) - d*sin(theta)*cos(omega) + d/2*sin(omega),
               -6.75*sin(omega) -  d*sin(theta)*sin(omega) - d/2*cos(omega), 13 + d*cos(theta)); //d
    qglColor(QColor(85,107,47));
    //xp - d/2*sin(theta), yp + d/2, zp + d/2*cos(theta)
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    qglColor(QColor(128,128,0));
    //-6.75*cos(omega) + d/2, -6.75*sin(omega) + d/2, 13
    glVertex3f(-6.75*cos(omega) + d/2*sin(omega), -6.75*sin(omega) - d/2*cos(omega) , 13);
    qglColor(QColor(85,107,47));
    //xp + d/2*sin(theta), yp + d/2, zp - d/2*cos(theta)
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta));
    glEnd();

    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(-6.75*cos(omega) + d/2*sin(omega), -6.75*sin(omega) - d/2*cos(omega) , 13);
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    glVertex3f(-6.75*cos(omega) - d/2*sin(omega), -6.75*sin(omega) + d/2*cos(omega), 13);   //b
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    glVertex3f(-6.75*cos(omega) - d*cos(omega)*sin(theta) - d/2*sin(omega),
               -6.75*sin(omega) - d*sin(omega)*sin(theta) + d/2*cos(omega), 13 + d*cos(theta));//c
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    glVertex3f(-6.75*cos(omega) - d*sin(theta)*cos(omega) + d/2*sin(omega),
               -6.75*sin(omega) -  d*sin(theta)*sin(omega) - d/2*cos(omega), 13 + d*cos(theta)); //d
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h

    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    glEnd();

    glBegin(GL_QUADS);
    qglColor(QColor(128,128,0));
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    qglColor(QColor(85,107,47));
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    qglColor(QColor(128,128,0));
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    qglColor(QColor(85,107,47));
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    glEnd();
}

void CraneModel::drawStrenchBracket(float theta, float omega, float d, float l)
{
    float totalL = l + 24;
    float xp = (23.9*cos(theta) - 6.75)*cos(omega);     //比24小一点不然会产生缝隙
    float yp = (23.9*cos(theta) - 6.75)*sin(omega);
    float zp = 23.9*sin(theta) + 13;
    float xp1 = (totalL*cos(theta) - 6.75)*cos(omega);
    float yp1 = (totalL*cos(theta) - 6.75)*sin(omega);
    float zp1 = totalL*sin(theta) + 13;
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    qglColor(QColor(128,128,128));
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 - d/2*cos(theta)); //e1
    qglColor(QColor(128,128,128));
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 - d/2*cos(theta)); //f1
    qglColor(QColor(128,128,128));
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 + d/2*cos(theta));//g1
    qglColor(QColor(128,128,128));
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 + d/2*cos(theta)); //h1
    qglColor(QColor(128,128,128));
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 - d/2*cos(theta)); //e1
    glEnd();


    glBegin(GL_QUADS);
     qglColor(QColor(169,169,169));
     glVertex3f(xp1 - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
                yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 + d/2*cos(theta)); //h1
    qglColor(QColor(128,128,128));
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 - d/2*cos(theta)); //e1
    qglColor(QColor(169,169,169));
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 - d/2*cos(theta)); //f1
    qglColor(QColor(128,128,128));
    glVertex3f(xp1 - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 + d/2*cos(theta));//g1
    glEnd();

    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(xp + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp - d/2*cos(theta)); //e
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 - d/2*cos(theta)); //e1
    glVertex3f(xp + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp - d/2*cos(theta)); //f
    glVertex3f(xp1 + d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 - d/2*cos(theta)); //f1
    glVertex3f(xp - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp + d/2*cos(theta));//g
    glVertex3f(xp1 - d/2*sin(theta)*cos(omega) - d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) + d/2*cos(omega), zp1 + d/2*cos(theta));//g1
    glVertex3f(xp - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp + d/2*cos(theta)); //h
    glVertex3f(xp1 - d/2*sin(theta)*cos(omega) + d/2*sin(omega),
               yp1 - d/2*sin(theta)*sin(omega) - d/2*cos(omega), zp1 + d/2*cos(theta)); //h1
    glEnd();

}

void CraneModel::drawHook(float theta, float omega, float l, float d, float d1)
{
    //d杆子粗细 d1线长
    float totalL = l + 24;
    float xp = (totalL*cos(theta) - 6.75)*cos(omega);
    float yp = (totalL*cos(theta) - 6.75)*sin(omega);
    float zp = totalL*sin(theta) + 13;
    //线加一个正方体
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(xp, yp, zp - d/2/cos(theta));
    glVertex3f(xp, yp, zp - d1);
    glEnd();

    float len = 2;
    glBegin(GL_QUAD_STRIP);//连续填充四边形串
    qglColor(QColor(255,0,0));
    glVertex3f(xp - len/2, yp + len/2, zp - d1);
    qglColor(QColor(165,42,42));
    glVertex3f(xp - len/2, yp - len/2, zp - d1);
    qglColor(QColor(255,0,0));
    glVertex3f(xp + len/2, yp + len/2, zp - d1);
    qglColor(QColor(165,42,42));
    glVertex3f(xp + len/2, yp - len/2, zp - d1);
    qglColor(QColor(255,0,0));
    glVertex3f(xp + len/2, yp + len/2, zp - d1 - len/2);
    qglColor(QColor(165,42,42));
    glVertex3f(xp + len/2, yp - len/2, zp - d1 - len/2);
    qglColor(QColor(255,0,0));
    glVertex3f(xp - len/2, yp + len/2, zp - d1 - len/2);
    qglColor(QColor(165,42,42));
    glVertex3f(xp - len/2, yp - len/2, zp - d1 - len/2);
    qglColor(QColor(255,0,0));
    glVertex3f(xp - len/2, yp + len/2, zp - d1);
    glEnd();

    glBegin(GL_QUADS);//连续填充四边形串
    qglColor(QColor(255,0,0));
    glVertex3f(xp - len/2, yp + len/2, zp - d1 - len/2);
    qglColor(QColor(165,42,42));
    glVertex3f(xp - len/2, yp + len/2, zp - d1);
    qglColor(QColor(255,0,0));
    glVertex3f(xp + len/2, yp + len/2, zp - d1);
    qglColor(QColor(165,42,42));
    glVertex3f(xp + len/2, yp + len/2, zp - d1 - len/2);
    glEnd();

    glBegin(GL_QUADS);//连续填充四边形串
    qglColor(QColor(255,0,0));
    glVertex3f(xp - len/2, yp - len/2, zp - d1 - len/2);
    qglColor(QColor(165,42,42));
    glVertex3f(xp - len/2, yp - len/2, zp - d1);
    qglColor(QColor(255,0,0));
    glVertex3f(xp + len/2, yp - len/2, zp - d1);
    qglColor(QColor(165,42,42));
    glVertex3f(xp + len/2, yp - len/2, zp - d1 - len/2);
    glEnd();
}


void CraneModel::drawCoordinate(float xMin, float xMax, float xStep,
                                float yMin, float yMax, float yStep,
                                float zMin, float zMax, float zStep)
{
    drawXAxis(xMin, xMax, xStep, yMin, yMax, zMin, zMax);
    drawYAxis(yMin, yMax, yStep, xMin, xMax, zMin, zMax);
    drawZAxis(zMin, zMax, zStep, xMin, xMax, yMin, yMax);
}

void CraneModel::drawXAxis(float xMin, float xMax, float xStep,
                           float yMin, float yMax,
                           float zMin, float zMax)
{
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(xMin, yMin, 0);
    glVertex3f(xMax, yMin, 0);
    for(int i = 0;i < xMax - xMin;i += xStep)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(xMin + i, yMin, 0);
        glVertex3f(xMin + i, yMin - 1, 0);
        qglColor(QColor(192,192,192));
        glVertex3f(xMin + i, yMin, 0);
        glVertex3f(xMin + i, yMax, 0);
        glVertex3f(xMin + i, yMax, zMin);
        glVertex3f(xMin + i, yMax, zMax);
    }
    glEnd();
    //添加文字
    for(int i = 0;i < xMax - xMin;i += xStep)
    {
        renderText(xMin + i, yMin - 2, 0, QString::number(i + xMin));
    }
}

void CraneModel::drawYAxis(float yMin, float yMax, float yStep,
                           float xMin, float xMax,
                           float zMin, float zMax)
{
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(xMin, yMin, 0);
    glVertex3f(xMin, yMax, 0);
    for(int i = 0;i < yMax - yMin;i += yStep)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(xMin, yMin + i, 0);
        glVertex3f(xMin - 1, yMin + i, 0);
        qglColor(QColor(192,192,192));
        glVertex3f(xMin, yMin + i, 0);
        glVertex3f(xMax, yMin + i, 0);
        glVertex3f(xMin, yMin + i, zMin);
        glVertex3f(xMin, yMin + i, zMax);
    }
    glEnd();
    for(int i = 0;i < yMax - yMin;i += yStep)
    {
        renderText(xMin - 2, yMin + i, 0, QString::number(yMin + i));
    }
}

void CraneModel::drawZAxis(float zMin, float zMax, float zStep,
                           float xMin, float xMax,
                           float yMin, float yMax)
{
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(xMin, yMin, zMin);
    glVertex3f(xMin, yMin, zMax);
    for(int i = 0;i < zMax - zMin;i += zStep)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(xMin, yMin, zMin + i);
        glVertex3f(xMin - 1, yMin, zMin + i);

        qglColor(QColor(192,192,192));
        glVertex3f(xMin, yMin, i);
        glVertex3f(xMin, yMax, i);
        glVertex3f(xMin, yMax, i);
        glVertex3f(xMax, yMax, i);
    }
    glEnd();
    for(int i = 0;i < zMax - zMin;i += zStep)
    {
        if(i == 0){
            continue;
        }
        renderText(xMin - 2, yMin, zMin + i, QString::number(zMin + i));
    }
}

void CraneModel::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void CraneModel::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    //qDebug() << dx << " " <<dy;
    if (event->buttons() & Qt::LeftButton) {
        yRot += 4 * dy;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        zRot += 4 * dx;
        updateGL();
    }

    lastPos = event->pos();
}

void CraneModel::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if ( angle != xRot ) {
        xRot = angle;
        //emit xRotationChanged(angle);
        updateGL();
    }
}

void CraneModel::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if ( angle != yRot ) {
        yRot = angle;
        //emit yRotationChanged(angle);
        updateGL();
    }
}

void CraneModel::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if ( angle != zRot ) {
        zRot = angle;
        //emit zRotationChanged(angle);
        updateGL();
    }
}

void CraneModel::normalizeAngle(int *angle)
{
    while (*angle < 0)
        angle += 360 * 16;
    while (*angle > 360 *16)
        angle -= 360 *16;
}


void CraneModel::drawRec(float omega){
    glBegin(GL_QUADS);
    qglColor(QColor(128,128,0));
    float x = 2;
    float y = 1;
    qDebug()<<"---------------------------";
    x = x*cos(omega) - y*sin(omega);
    y = x*sin(omega) + y*cos(omega);
    qDebug()<<"("<<x<<","<<y<<")";
    glVertex3f(cos(omega) - sin(omega),sin(omega) + cos(omega),1);
    qglColor(QColor(85,107,47));
    x = 1;
    y = 1;
    x = x*cos(omega) - y*sin(omega);
    y = x*sin(omega) + y*cos(omega);
    qDebug()<<"("<<x<<","<<y<<")";
    glVertex3f(-cos(omega) - sin(omega),-sin(omega) + cos(omega),1);
    qglColor(QColor(128,128,0));
    x = 1;
    y = -1;
    x = x*cos(omega) - y*sin(omega);
    y = x*sin(omega) + y*cos(omega);
    qDebug()<<"("<<x<<","<<y<<")";
    glVertex3f(-cos(omega) + sin(omega),-sin(omega) - cos(omega),0);
    qglColor(QColor(85,107,47));
    x = 2;
    y = -1;
    x = x*cos(omega) - y*sin(omega);
    y = x*sin(omega) + y*cos(omega);
    qDebug()<<"("<<x<<","<<y<<")";
    glVertex3f(cos(omega) + sin(omega),sin(omega) - cos(omega),0);
    glEnd();
    qDebug()<<sin(3.14159/180*45);
}

void CraneModel::drawObstacle(float alpha, float phi, float d, float r)
{
    float totalL = l + 24;
    //顶点坐标
    float xp1 = (totalL*cos(theta) - 6.75)*cos(omega);
    float yp1 = (totalL*cos(theta) - 6.75)*sin(omega);
    float zp1 = totalL*sin(theta) + 13;
    //计算球心坐标
    if(!obstacleDraw)
    {
        xv = xp1 + d*cos(alpha)*cos(phi);
        yv = yp1 + d*cos(alpha)*sin(phi);
        zv = zp1 + d*sin(alpha);
        obstacleDraw = true;
//        GLfloat LightAmbient[] = {253/255,245/255,230/255,0.5f};  //环境光参数
//        GLfloat LightDiffuse[] = {0.3f, 0.5f, 0.2f, 0.5f};  //漫散光参数
//        GLfloat LightPosition[] = {xv, yv, zv + 100, 0.6f}; //光源位置
//        glEnable(GL_LIGHTING);
//        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光
//        glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置
//        glEnable(GL_LIGHT1);                                //启动一号光源
    }
    glEnable(GL_BLEND); // 打开混合
    glDisable(GL_DEPTH_TEST); // 关闭深度测试
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float color[4] = {220, 20, 60, 0.8};
    drawSphere(xv, yv, zv, r, 100, 100, color);
//    float color1[4] = {255, 99, 71, 0.6};
//    drawSphere(xv, yv, zv, 10, 100, 100, color1);
//    float color2[4] = {255, 250, 205, 0.5};
//    drawSphere(xv, yv, zv, 13, 100, 100, color2);
    glDisable(GL_BLEND); // 打开混合
    glEnable(GL_DEPTH_TEST); // 关闭深度测试

    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

void CraneModel::drawSphere(float xx, float yy, float zz,
                            float radius, float M, float N, float color[4])
{
    float step_z = PI / M;
    float step_xy = 2 * PI / N;
    float x[4], y[4], z[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glBegin(GL_QUADS);
    glColor4f (color[0]/255, color[1]/255, color[2]/255, color[3]);
    for(i = 0; i < M; i++)
    {
        angle_z = i * step_z;
        for(j = 0; j < N; j++)
        {
            angle_xy = j * step_xy;

            x[0] = radius * sin(angle_z) * cos(angle_xy);
            y[0] = radius * sin(angle_z) * sin(angle_xy);
            z[0] = radius * cos(angle_z);

            x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
            y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = radius * cos(angle_z + step_z);

            x[2] = radius * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            y[2] = radius * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            z[2] = radius * cos(angle_z + step_z);

            x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
            y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
            z[3] = radius * cos(angle_z);
            for(int k = 0; k < 4; k++)
            {
                glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
            }
        }
    }
    glEnd();
}
