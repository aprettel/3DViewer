#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QSettings>
#include <QTimer>

#include "myglwidget.h"
#include "ui_mainwindow.h"

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
  void on_cleanModelButton_clicked();
  void on_translationBox_activated();
  void on_rotateBox_activated();

  void on_saveButton_img_clicked();
  void on_saveButton_gif_clicked();
  void recordGif();
  void makeGif();

  void on_proectionBox_currentIndexChanged(int index);

  QColor setColor(QPushButton *button);
  void on_lineColorButton_clicked();
  void on_dothColorButton_clicked();
  void on_backColorButton_clicked();

  void on_thicknessBox_valueChanged(int arg1);
  void on_sizeBox_valueChanged(int arg1);

  void on_typeLine_currentIndexChanged(int index);
  void on_typeDoth_currentIndexChanged(int index);
  void cleanPosRotScale();
  void save_settings();
  void load_settings();

private:
  QSettings *settings;
  Ui::MainWindow *ui;
  QTimer *timer;
  MyGLWidget *myGLWidget;
  QString fileName;
  Vertex OldRot = {0, 0, 0};
  Vertex OldPos = {0, 0, 0};
  struct Model *m_model = NULL;
};

#endif // MAINWINDOW_H
