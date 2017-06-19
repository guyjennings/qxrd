#ifndef QTESTCEPLIBMAINWINDOW_H
#define QTESTCEPLIBMAINWINDOW_H

#include <QMainWindow>
#include "qcepproperty.h"
#include "qcepvector3dproperty.h"
#include "qcepmatrix3x3property.h"
#include "qtestimagedata.h"

namespace Ui {
  class QtestceplibMainWindow;
}

class QtestceplibDocument;

class QtestceplibMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibMainWindow(QtestceplibDocument *doc, QWidget *parent = 0);
  ~QtestceplibMainWindow();
  
public slots:
  void doReadSettings();
  void doWriteSettings();

  void readSettings();
  void writeSettings();

  void readSettings(QString filePath);
  void writeSettings(QString filePath);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void doLoadImage();
  void doSaveImage();
  void doNewImage();

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

private:
  Ui::QtestceplibMainWindow *ui;
  QtestceplibDocument *m_Document;
  mutable QMutex m_Mutex;

  QTestImageData *m_ImageData;
};

#endif // QTESTCEPLIBMAINWINDOW_H
