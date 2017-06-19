#ifndef QTESTCEPLIBCOMBINEDMAINWINDOW_H
#define QTESTCEPLIBCOMBINEDMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibCombinedMainWindow;
}

class QtestceplibCombinedDocument;

class QtestceplibCombinedMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibCombinedMainWindow(QtestceplibCombinedDocument *doc,
                                         QWidget *parent = 0);
  ~QtestceplibCombinedMainWindow();
  
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
  void doLoadCBFImage();

  void doTestHDF5Library();
  void doTestHDF5SlabOutput();

  void doTestNexusLibrary();

  void doTestCBFLibrary();
  int cbfCheck(int status,const char *file, int line);

  void printMessage(QString msg);

private:
  Ui::QtestceplibCombinedMainWindow *ui;
  QtestceplibCombinedDocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBCOMBINEDMAINWINDOW_H
