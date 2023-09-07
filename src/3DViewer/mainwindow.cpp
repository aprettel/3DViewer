#include "mainwindow.h"

#include "../viewer.c"
#include "gif.h"
#include "ui_mainwindow.h"

extern "C" {
#include "../viewer.h"
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  myGLWidget = new MyGLWidget(this);
  ui->visualLayout->addWidget(myGLWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_openButton_clicked() {
  on_clearZoom_clicked();
  fileName = QFileDialog::getOpenFileName(this, tr("Open .obj File"), ".",
                                          tr("OBJ Files (*.obj)"));
  if (!fileName.isEmpty()) {
    m_model = loadModelFromFile(fileName.toStdString().c_str());
    if (!m_model) {
      QMessageBox::warning(this, "Ошибка", "Ошибка открытия модели!\n");
      return;
    }

    normalizeModel(m_model, -0.1, 0.1);

    myGLWidget->setModel(m_model);

    QString info = "Файл: " + QFileInfo(fileName).fileName() + "\n";
    info +=
        "Количество вершин: " + QString::number(m_model->numVertices) + "\n";
    info += "Количество ребер: " + QString::number(m_model->numSurfaces) + "\n";
    ui->modelInfo_label->setText(info);

  } else {
    QMessageBox::warning(this, "Ошибка", "Файл пуст!\n");
    return;
  }
}

void MainWindow::on_rotateSlider_valueChanged() {
  static double oldX = 0.0;
  static double oldY = 0.0;
  static double oldZ = 0.0;
  static char axis = 'x';
  char setedAxis = ui->rotateBox->currentText().constData()[0].toLatin1();
  if (axis != setedAxis) {
    axis = setedAxis;
    switch (setedAxis) {
      case 'x':
        ui->rotateSlider->setValue(oldX / M_PI * 180.0);
        break;
      case 'y':
        ui->rotateSlider->setValue(oldY / M_PI * 180.0);
        break;
      case 'z':
        ui->rotateSlider->setValue(oldZ / M_PI * 180.0);
        break;
    }
  } else {
    if (fileName.isEmpty()) {
      ui->rotateSlider->isEnabled();
    } else {
      double rotationAngle = (double)ui->rotateSlider->value();
      double rotationAngleRad = rotationAngle * M_PI / 180.0;
      if (ui->rotateBox->currentText() == "x") {
        rotateModel_X(m_model, rotationAngleRad - oldX);
        oldX = rotationAngleRad;
      } else if (ui->rotateBox->currentText() == "y") {
        rotateModel_Y(m_model, rotationAngleRad - oldY);
        oldY = rotationAngleRad;
      } else if (ui->rotateBox->currentText() == "z") {
        rotateModel_Z(m_model, rotationAngleRad - oldZ);
        oldZ = rotationAngleRad;
      }
    }
    myGLWidget->repaint();
  }
}

void MainWindow::on_scaleSlider_valueChanged() {
  static double oldScale = 0.0;
  if (fileName.isEmpty()) {
    ui->scaleSlider->isEnabled();
  } else {
    const double scaleFactor = (double)ui->scaleSlider->value() / 100.0;
    scaleModel(m_model, scaleFactor - oldScale);
    oldScale = scaleFactor;

    myGLWidget->repaint();
  }
}

void MainWindow::on_translationSlider_valueChanged() {
  static double oldX = 0.0;
  static double oldY = 0.0;
  static double oldZ = 0.0;
  static char axis = 'x';
  char setedAxis = ui->translationBox->currentText().constData()[0].toLatin1();
  if (axis != setedAxis) {
    axis = setedAxis;
    switch (setedAxis) {
      case 'x':
        ui->translationSlider->setValue(oldX * 100);
        break;
      case 'y':
        ui->translationSlider->setValue(oldY * 100);
        break;
      case 'z':
        ui->translationSlider->setValue(oldZ * 100);
        break;
    }
  } else {
    if (fileName.isEmpty()) {
      ui->translationSlider->isEnabled();
    } else {
      double translationFactor = (double)ui->translationSlider->value() / 100.0;
      if (ui->translationBox->currentText() == "x") {
        translateModel(m_model, translationFactor - oldX, 0.0, 0.0);
        oldX = translationFactor;
      } else if (ui->translationBox->currentText() == "y") {
        translateModel(m_model, 0.0, translationFactor - oldY, 0.0);
        oldY = translationFactor;
      } else if (ui->translationBox->currentText() == "z") {
        translateModel(m_model, 0.0, 0.0, translationFactor - oldZ);
        oldZ = translationFactor;
      }
    }
    myGLWidget->repaint();
  }
}

void MainWindow::on_clearZoom_clicked() {
  ui->translationSlider->setValue(0);
  ui->scaleSlider->setValue(0);
  ui->rotateSlider->setValue(0);
  if (fileName.isEmpty()) {
    ui->clearZoom->isEnabled();
  } else {
    m_model = loadModelFromFile(fileName.toStdString().c_str());
    myGLWidget->setModel(m_model);

    normalizeModel(m_model, -1.0, 1.0);

    myGLWidget->repaint();
  }
}

void MainWindow::on_translationBox_activated() {
  on_translationSlider_valueChanged();
}

void MainWindow::on_rotateBox_activated() { on_rotateSlider_valueChanged(); }

void MainWindow::on_saveButton_gif_clicked() {
  ui->saveButton_gif->setText("REC...");
  ui->saveButton_gif->setDisabled(1);
  QDir* pathtmp = new QDir();
  pathtmp->mkdir("tmp");
  timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(recordGif()));
  timer->start(100);
}

void MainWindow::recordGif() {
  static int count = 0;
  if (count < 50) {
    QPixmap pixmap = ui->screen->grab();
    QImage image = pixmap.toImage().scaled(640, 480, Qt::IgnoreAspectRatio);
    image.save("tmp/" + QString::number(count) + ".bmp");
    count++;
  }

  if (count == 50) {
    timer->stop();
    count = 0;
    makeGif();
  }
}

void MainWindow::on_saveButton_img_clicked() {
  QString imagePath = QFileDialog::getSaveFileName(
      this, "Save Image", QString(), "BMP (*.bmp);;JPEG (*.jpg *.jpeg)");

  if (!imagePath.isEmpty()) {
    QImage image = ui->screen->grab().toImage();

    // Сохранение изображения в формате BMP
    if (imagePath.endsWith(".bmp", Qt::CaseInsensitive)) {
      if (!image.save(imagePath, "BMP")) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка сохранения изображения!\n");
      }
    }
    // Сохранение изображения в формате JPEG
    else if (imagePath.endsWith(".jpg", Qt::CaseInsensitive) ||
             imagePath.endsWith(".jpeg", Qt::CaseInsensitive)) {
      if (!image.save(imagePath, "JPEG")) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка сохранения изображения!\n");
      }
    }
  }
}

void MainWindow::makeGif() {
  QString qpath = QFileDialog::getSaveFileName(this, NULL, NULL, "GIF (*.gif)");

  if (!qpath.isNull()) {
    std::string tmp = qpath.toStdString();
    const char* path = tmp.c_str();
    GifWriter g = {};
    GifBegin(&g, path, 640, 480, 10);

    for (int i = 0; i < 50; ++i) {
      QImage img("tmp/" + QString::number(i) + ".bmp");
      GifWriteFrame(&g,
                    img.convertToFormat(QImage::Format_RGBA8888).constBits(),
                    640, 480, 10);
    }
    GifEnd(&g);
  }

  QDir("tmp").removeRecursively();
  ui->saveButton_gif->setDisabled(0);
  ui->saveButton_gif->setText("Сохранить GIF");
}

void MainWindow::on_proectionBox_currentIndexChanged(int index) {
  myGLWidget->proection_type = index;
}

QColor MainWindow::setColor(QPushButton* button) {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");

  if (color.isValid()) {
    QPalette palette = button->palette();
    palette.setColor(QPalette::Button, color);
    button->setPalette(palette);
    button->setAutoFillBackground(true);
    button->setFlat(true);
  }
  return color;
}

void MainWindow::on_lineColorButton_clicked() {
  QColor color = setColor(ui->lineColorButton);
  if (color.isValid()) {
    myGLWidget->lineColor = color;
    myGLWidget->setModel(m_model);
  }
}

void MainWindow::on_dothColorButton_clicked() {
  QColor color = setColor(ui->dothColorButton);
  if (color.isValid()) {
    myGLWidget->dothColor = color;
    myGLWidget->setModel(m_model);
  }
}

void MainWindow::on_backColorButton_clicked() {
  QColor color = setColor(ui->backColorButton);
  if (color.isValid()) {
    myGLWidget->backColor = color;
    myGLWidget->setModel(m_model);
    myGLWidget->paintGL();
  }
}
