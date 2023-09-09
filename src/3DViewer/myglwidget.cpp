#include "myglwidget.h"

#include "mainwindow.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_model(nullptr) {
  lineColor = QColor(Qt::red);
  dothColor = QColor(Qt::blue);
  backColor = QColor(50, 50, 50);
  dothSize = 3.0f;
  lineSize = 2.0f;
  dothType = 0;
}

void MyGLWidget::initializeGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->initializeOpenGLFunctions();
  glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
               dothColor.alphaF());
}

void MyGLWidget::resizeGL(int width, int height) {
  update_proection_GL((double)width / (double)height);
}

void MyGLWidget::update_proection_GL(GLdouble aspect) {
  if (aspect != 0) {
    aspectMem = aspect;
  }
  fH = proection_type == PERSP ? tan(fovY / 360 * pi) * zNear : 1.5;
  fW = fH * aspectMem;

  repaint();
}

void MyGLWidget::paintGL() {
  glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
               dothColor.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (m_model) {
    glEnable(GL_DEPTH_TEST);

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
