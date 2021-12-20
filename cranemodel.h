#ifndef CRANEMODEL_H
#define CRANEMODEL_H
#include <QGLWidget>

class CraneModel : public QGLWidget
{
    Q_OBJECT
public:
    explicit CraneModel(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawCylinder(float r);    //绘制底盘
    void drawCircle(float z, float r);
    void drawCircle(float xx, float yy, float zz, float r);
    void drawFixedBracket(float omega, float d);    //第一节固定
    void drawFixedBracket1(float theta, float omega, float d, float d1, float r);   //第二节固定,r表示圆柱半径
    void drawStrenchBracket(float theta, float omega, float d, float l);
    void drawHook(float theta, float omega, float l, float d, float d1);

    void drawObstacle(float alpha, float phi, float d, float r);
    void drawSphere(float xx, float yy, float zz,
                    float radius, float M, float N, float color[4]);

    void drawCoordinate(float xMin = -20, float xMax = 20, float xStep = 5,
                        float yMin = -20, float yMax = 20, float yStep = 5,
                        float zMin = 0, float zMax = 40, float zStep = 5);
    void drawXAxis(float xMin = -20, float xMax = 20, float xStep = 5,
                   float yMin = -20, float yMax = 20,
                   float zMin = 0, float zMax = 40);
    void drawYAxis(float yMin = -20, float yMax = 20, float yStep = 5,
                   float xMin = -20, float xMax = 20,
                   float zMin = 0, float zMax = 40);
    void drawZAxis(float zMin = 0, float zMax = 40, float zStep = 5,
                   float xMin = -20, float xMax = 20,
                   float yMin = -20, float yMax = 20);

    void drawRec(float omega);

    void normalizeAngle(int *angle);

    int xRot;
    int yRot;
    int zRot;

    float theta;
    float omega;
    float alpha;    //确定障碍物位置
    float phi;
    float d;
    float l;    //用于绘制伸缩杆,仅仅表示伸长的那一段
    float xv, yv, zv;

    boolean obstacleDraw = false;

    QPoint lastPos;

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
};

#endif // CRANEMODEL_H
