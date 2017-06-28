#ifndef QTESTCEPLIBHDF5MAINWINDOW_H
#define QTESTCEPLIBHDF5MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibHdf5MainWindow;
}

class QtestceplibHdf5Document;

class QtestceplibHdf5MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibHdf5MainWindow(QtestceplibHdf5Document *doc, QWidget *parent = 0);
  ~QtestceplibHdf5MainWindow();
  
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

  void doTestHDF5Library();
  void doTestHDF5SlabOutput();
  void doIterateHDF5File();
  void doIterateHDF5File2();

  void printMessage(QString msg);

private:
  Ui::QtestceplibHdf5MainWindow *ui;
  QtestceplibHdf5Document *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBHDF5MAINWINDOW_H
