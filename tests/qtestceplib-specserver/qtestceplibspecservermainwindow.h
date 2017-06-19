#ifndef QTESTCEPLIBSPECSERVERMAINWINDOW_H
#define QTESTCEPLIBSPECSERVERMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibSpecServerMainWindow;
}

class QtestceplibSpecServerDocument;

class QtestceplibSpecServerMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibSpecServerMainWindow(QtestceplibSpecServerDocument *doc, QWidget *parent = 0);
  ~QtestceplibSpecServerMainWindow();
  
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

  void printMessage(QString msg);

private:
  Ui::QtestceplibSpecServerMainWindow *ui;
  QtestceplibSpecServerDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBSPECSERVERMAINWINDOW_H
