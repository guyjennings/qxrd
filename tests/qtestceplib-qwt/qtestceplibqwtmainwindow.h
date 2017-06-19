#ifndef QTESTCEPLIBQWTMAINWINDOW_H
#define QTESTCEPLIBQWTMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibQwtMainWindow;
}

class QtestceplibQwtDocument;

class QtestceplibQwtMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibQwtMainWindow(QtestceplibQwtDocument *doc, QWidget *parent = 0);
  ~QtestceplibQwtMainWindow();
  
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
  Ui::QtestceplibQwtMainWindow *ui;
  QtestceplibQwtDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBQWTMAINWINDOW_H
