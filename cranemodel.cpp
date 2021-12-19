#include "cranemodel.h"
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include <QDebug>
#include <QColor>

CraneModel::CraneModel(QWidget *parent):
    QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    omega = 0;
    theta = 15*3.14/180;
    l = 10;

//    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, [=](){
//                omega += 3.14/1000;
//                theta += 3.14/1000;
//                l += 1;
//                if(theta > 3.14 * 50/180)
//                {
//                    theta = -3.14 * 30/180;
//                }
//                if(l > 26){
//                    l = 0;
//                }
//                updateGL();
//            });  //一直旋转，主要看坐标设置部分
//    timer->start(40);
}


void CraneModel::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void CraneModel::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 0, -0.5);
    glRotatef(yRot, 1.0, 0.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);
//    drawCoordinate(6);
//    drawRec(omega);
    drawCylinder(6);
    drawFixedBracket(omega, 1.5);
    drawFixedBracket1(theta, omega, 1.5, 1.5, 1.75);
    drawStrenchBracket(theta, omega, 0.8, l);
    drawHook(theta, omega, l, 0.8, 4);
    glPopMatrix();
}

void CraneModel::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((width() - side)/2, (height() - side)/2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4, 4, -2, 4, 6.0, 70.0);
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
    float x,y;
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


void CraneModel::drawCoordinate(float r)
{
    drawXAxis(r);
    drawYAxis(r);
    drawZAxis(r);
}

void CraneModel::drawXAxis(float r)
{
    float d = 10;   //坐标轴原点起始位置
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(-r - d, -r - d, 0);
    glVertex3f(-r - d + 40, -r - d, 0);
    for(int i = 0;i < 40;i+=5)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(-r - d + i, - r - d, 0);
        glVertex3f(-r - d + i, -r - d - 1, 0);
        qglColor(QColor(192,192,192));
        glVertex3f(-r - d + i, - r - d, 0);
        glVertex3f(-r - d + i, -r - d + 40, 0);
        glVertex3f(-r - d + i, -r - d + 40, 0);
        glVertex3f(-r - d + i, -r - d + 40, 40);
    }
    glEnd();
    //添加文字
    for(int i = 0;i < 40;i+=5)
    {
        renderText(-r - d + i, -r - d - 2, 0, QString::number(i - d - r));
    }
}

void CraneModel::drawYAxis(float r)
{
    float d = 10;   //坐标轴原点起始位置
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(-r - d, -r - d, 0);
    glVertex3f(-r - d, -r - d + 40, 0);
    for(int i = 0;i < 40;i+=5)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(-r - d, - r - d + i, 0);
        glVertex3f(-r - d - 1, -r - d + i, 0);

        qglColor(QColor(192,192,192));
        glVertex3f(-r - d, - r - d + i, 0);
        glVertex3f(-r - d + 40, - r - d + i, 0);

        glVertex3f(-r - d, - r - d + i, 0);
        glVertex3f(-r - d, - r - d + i, 40);
    }
    glEnd();
    for(int i = 0;i < 40;i+=5)
    {
        renderText(-r - d - 2, -r - d + i, 0, QString::number(i - d - r));
    }
}

void CraneModel::drawZAxis(float r)
{
    float d = 10;   //坐标轴原点起始位置
    glBegin(GL_LINES);
    qglColor(Qt::black);
    glVertex3f(-r - d, -r - d, 0);
    glVertex3f(-r - d, -r - d, 40);
    for(int i = 0;i < 40;i += 5)
    {
        //绘制刻度
        qglColor(Qt::black);
        glVertex3f(-r - d, - r - d, i);
        glVertex3f(-r - d - 1, -r - d, i);

        qglColor(QColor(192,192,192));
        glVertex3f(-r - d, - r - d, i);
        glVertex3f(-r - d, - r - d + 40, i);

        glVertex3f(-r - d, - r - d + 40, i);
        glVertex3f(-r - d + 40, - r - d + 40, i);
    }
    glEnd();
    for(int i = 0;i < 40;i+=5)
    {
        if(i == 0){
            continue;
        }
        renderText(-r - d - 2, -r - d, i, QString::number(i));
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
