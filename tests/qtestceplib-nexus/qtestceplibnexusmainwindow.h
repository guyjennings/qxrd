#ifndef QTESTCEPLIBNEXUSMAINWINDOW_H
#define QTESTCEPLIBNEXUSMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibNexusMainWindow;
}

class QtestceplibNexusDocument;

class QtestceplibNexusMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibNexusMainWindow(QtestceplibNexusDocument *doc, QWidget *parent = 0);
  ~QtestceplibNexusMainWindow();
  
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

  void doTestNexusLibrary();

  void printMessage(QString msg);

private:
  Ui::QtestceplibNexusMainWindow *ui;
  QtestceplibNexusDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBNEXUSMAINWINDOW_H
