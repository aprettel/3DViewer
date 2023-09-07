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
  int proection_type;
  explicit MyGLWidget(QWidget *parent = nullptr);
  void setModel(struct Model *model);
  void drawModel(struct Model *model);
  void update_proection_GL(GLdouble aspect);
  void resizeGL(int width, int height) override;
  void paintGL() override;
  QColor lineColor, dothColor, backColor;
  int lineType, dothType;
  double lineSize, dothSize;

 protected:
  void initializeGL() override;

 private:
  struct Model *m_model;
};

#endif  // MYGLWIDGET_H
