#include "myglwidget.h"

#include "mainwindow.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_model(nullptr) {
  lineColor = QColor(Qt::red);
  dothColor = QColor(Qt::blue);
  backColor = QColor(Qt::white);
  dothSize = 3.0f;
  lineSize = 2.0f;
  dothType = 0;
}

void MyGLWidget::initializeGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->initializeOpenGLFunctions();
  glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
               dothColor.alphaF());

  proection_type = PERSP;
}

void MyGLWidget::resizeGL(int width, int height) {
  update_proection_GL((double)width / (double)height);
}

void MyGLWidget::update_proection_GL(GLdouble aspect) {
  GLdouble fovY = 45;
  GLdouble zNear = 0.01;
  GLdouble zFar = 100;

  const GLdouble pi = 3.1415926535897932384626433832795;
  static GLdouble fW, fH;

  // fH = tan((fovY / 2) / 180 * pi) * zNear;
  if (aspect != 0) {
    fH = tan(fovY / 360 * pi) * zNear * 10;
    fW = fH * aspect;
  }

  glLoadIdentity();

  if (proection_type == ORTO) glOrtho(-fW, fW, -fH, fH, zNear, zFar);
  if (proection_type == PERSP) glFrustum(-fW, fW, -fH, fH, zNear, zFar);
  paintGL();
}

void MyGLWidget::paintGL() {
  glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
               dothColor.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (m_model) {
    glEnable(GL_DEPTH_TEST);

    drawModel(m_model);
    glDisable(GL_DEPTH_TEST);
  }
}
void MyGLWidget::setModel(struct Model *model) {
  m_model = model;
  repaint();
}

void MyGLWidget::drawModel(struct Model *model) {
  // Рисование по точкам
  if (dothType == 1) {
    glEnable(GL_POINT_SMOOTH);
  } else if (dothType == 2) {
    glDisable(GL_POINT_SMOOTH);
  }

  if (dothType != 0) {
    glColor4f(dothColor.redF(), dothColor.greenF(), dothColor.blueF(),
              dothColor.alphaF());
    glPointSize(dothSize);
    glBegin(GL_POINTS);
    for (int i = 0; i < model->numVertices; i++) {
      glVertex3f(model->vertices[i].x, model->vertices[i].y,
                 model->vertices[i].z);
    }
    glEnd();
  }

  // Рисование по линиям
  glColor4f(lineColor.redF(), lineColor.greenF(), lineColor.blueF(),
            lineColor.alphaF());

  if (lineType == 0) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xFFFF);
  } else if (lineType == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xAAAA);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(lineSize);
  glBegin(GL_LINES);

  for (int i = 0; i < model->numSurfaces; i++) {
    glVertex3f(model->vertices[model->surfaces[i].v1 - 1].x,
               model->vertices[model->surfaces[i].v1 - 1].y,
               model->vertices[model->surfaces[i].v1 - 1].z);

    glVertex3f(model->vertices[model->surfaces[i].v2 - 1].x,
               model->vertices[model->surfaces[i].v2 - 1].y,
               model->vertices[model->surfaces[i].v2 - 1].z);

    glVertex3f(model->vertices[model->surfaces[i].v2 - 1].x,
               model->vertices[model->surfaces[i].v2 - 1].y,
               model->vertices[model->surfaces[i].v2 - 1].z);

    glVertex3f(model->vertices[model->surfaces[i].v3 - 1].x,
               model->vertices[model->surfaces[i].v3 - 1].y,
               model->vertices[model->surfaces[i].v3 - 1].z);

    glVertex3f(model->vertices[model->surfaces[i].v3 - 1].x,
               model->vertices[model->surfaces[i].v3 - 1].y,
               model->vertices[model->surfaces[i].v3 - 1].z);

    glVertex3f(model->vertices[model->surfaces[i].v1 - 1].x,
               model->vertices[model->surfaces[i].v1 - 1].y,
               model->vertices[model->surfaces[i].v1 - 1].z);
  }
  glEnd();
}
