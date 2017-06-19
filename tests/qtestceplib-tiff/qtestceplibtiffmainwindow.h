#ifndef QTESTCEPLIBTIFFMAINWINDOW_H
#define QTESTCEPLIBTIFFMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibTiffMainWindow;
}

class QtestceplibTiffDocument;

class QtestceplibTiffMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibTiffMainWindow(QtestceplibTiffDocument *doc, QWidget *parent = 0);
  ~QtestceplibTiffMainWindow();
  
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
  void doLoadTIFFImage();

  void printMessage(QString msg);

private:
  Ui::QtestceplibTiffMainWindow *ui;
  QtestceplibTiffDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBMAINWINDOW_H
