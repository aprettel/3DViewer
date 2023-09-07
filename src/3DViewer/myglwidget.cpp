#include "myglwidget.h"

#include "mainwindow.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_model(nullptr) {}

void MyGLWidget::initializeGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  // proection_type = ;
}

void MyGLWidget::resizeGL(int width, int height) {
  update_proection_GL((double)width / (double)height);
  //  widthMem = width;
  //  heightMem = height;
}

// void MyGLWidget::forse_resizeGL() {
//   resizeGL(widthMem,heightMem);
// }

void MyGLWidget::update_proection_GL(GLdouble aspect) {

  // fH = tan((fovY / 2) / 180 * pi) * zNear;
  if (aspect != 0) {
    aspectMem = aspect;
  }
  fH = proection_type == PERSP ? tan(fovY / 360 * pi) * zNear : 1.5;
  fW = fH * aspectMem;

  repaint();

  /*  glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      if (proection_type == ORTO)
        glOrtho(-fW, fW, -fH, fH, zNear, zFar);
      if (proection_type == PERSP)
        glFrustum(-fW, fW, -fH, fH, zNear, zFar);

      glMatrixMode(GL_MODELVIEW); // устанавливаем матрицу
      glLoadIdentity();*/ // загружаем матрицу
}

void MyGLWidget::paintGL() {
  if (m_model) {
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawModel(m_model);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (proection_type == ORTO)
      glOrtho(-fW, fW, -fH, fH, zNear, zFar);
    if (proection_type == PERSP)
      glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    glMatrixMode(GL_MODELVIEW); // устанавливаем матрицу
    glLoadIdentity();           // загружаем матрицу
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
