#include "myglwidget.h"

#include "mainwindow.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_model(nullptr) {}

void MyGLWidget::initializeGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

  if (proection_type == ORTO)
    glOrtho(-fW, fW, -fH, fH, zNear, zFar);
  if (proection_type == PERSP)
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
  paintGL();

}

void MyGLWidget::paintGL() {
  if (m_model) {
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  glColor3f(0.0f, 0.0f, 1.0f);
  glPointSize(3.0f);
  glBegin(GL_POINTS);
  for (int i = 0; i < model->numVertices; i++) {
    glVertex3f(model->vertices[i].x, model->vertices[i].y,
               model->vertices[i].z);
  }
  glEnd();

  // Рисование по линиям
  glColor3f(1.0f, 0.0f, 0.0f);
  glLineWidth(2.0f);
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
