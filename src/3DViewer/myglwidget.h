#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

// #include <GL/glu.h>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../viewer.h"
#define ORTO 0
#define PERSP 1

class MyGLWidget : public QOpenGLWidget {
  Q_OBJECT
public:
  int proection_type = ORTO;
  explicit MyGLWidget(QWidget *parent = nullptr);
  void setModel(struct Model *model);
  void drawModel(struct Model *model);
  //  void forse_resizeGL();
  void update_proection_GL(GLdouble aspect);
  QColor lineColor, dothColor, backColor;
  int lineType = 0, dothType = 1;
  double lineSize, dothSize;

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

private:
  const GLdouble fovY = 45;
  const GLdouble zNear = 0.01;
  const GLdouble zFar = 20;
  GLdouble aspectMem;
  GLdouble fW, fH;
  int widthMem;
  int heightMem;

  const GLdouble pi = 3.1415926535897932384626433832795;
  struct Model *m_model;
};

#endif // MYGLWIDGET_H
