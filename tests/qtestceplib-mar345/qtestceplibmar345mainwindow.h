#ifndef QTESTCEPLIBMAR345MAINWINDOW_H
#define QTESTCEPLIBMAR345MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibMar345MainWindow;
}

class QtestceplibMar345Document;
class QtestceplibMar345MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibMar345MainWindow(QtestceplibMar345Document *doc, QWidget *parent = 0);
  ~QtestceplibMar345MainWindow();
  
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
  Ui::QtestceplibMar345MainWindow *ui;
  QtestceplibMar345Document *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBMAR345MAINWINDOW_H
