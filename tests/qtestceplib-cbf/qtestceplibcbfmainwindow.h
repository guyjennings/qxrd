#ifndef QTESTCEPLIBCBFMAINWINDOW_H
#define QTESTCEPLIBCBFMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibCbfMainWindow;
}

class QtestceplibCbfDocument;

class QtestceplibCbfMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibCbfMainWindow(QtestceplibCbfDocument *doc, QWidget *parent = 0);
  ~QtestceplibCbfMainWindow();
  
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
  void doLoadCBFImage();

  void doTestCBFLibrary();
  int cbfCheck(int status,const char *file, int line);

  void printMessage(QString msg);

private:
  Ui::QtestceplibCbfMainWindow *ui;
  QtestceplibCbfDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBCBFMAINWINDOW_H
