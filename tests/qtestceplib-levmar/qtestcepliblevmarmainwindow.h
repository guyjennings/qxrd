#ifndef QTESTCEPLIBLEVMARMAINWINDOW_H
#define QTESTCEPLIBLEVMARMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibLevMarMainWindow;
}

class QtestceplibLevMarDocument;

class QtestceplibLevMarMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibLevMarMainWindow(QtestceplibLevMarDocument *doc, QWidget *parent = 0);
  ~QtestceplibLevMarMainWindow();
  
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
  void doTestLevMar();

  void printMessage(QString msg);

private:
  Ui::QtestceplibLevMarMainWindow *ui;
  QtestceplibLevMarDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBLEVMARMAINWINDOW_H
