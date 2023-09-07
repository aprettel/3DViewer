#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>

#include "myglwidget.h"
#include "ui_mainwindow.h"
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_openButton_clicked();
  void on_scaleSlider_valueChanged();
  void on_translationSlider_valueChanged();
  void on_rotateSlider_valueChanged();
  void on_clearZoom_clicked();
  void on_translationBox_activated();
  void on_rotateBox_activated();

  void on_colorButton_clicked();

  void on_saveButton_img_clicked();

  void on_saveButton_gif_clicked();
  void recordGif();
  void makeGif();


  void on_proectionBox_currentIndexChanged(int index);

private:
  Ui::MainWindow *ui;
  QTimer* timer;
  MyGLWidget *myGLWidget;
  QString fileName;
  QMovie movie;
  struct Model *m_model;
};

#endif  // MAINWINDOW_H
