#ifndef QTESTCEPLIBFILEIOMAINWINDOW_H
#define QTESTCEPLIBFILEIOMAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMutex>

namespace Ui {
  class QtestceplibFileIOMainWindow;
}

class QtestceplibFileIODocument;

class QtestceplibFileIOMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit QtestceplibFileIOMainWindow(QtestceplibFileIODocument *doc, QWidget *parent = 0);
  ~QtestceplibFileIOMainWindow();
  
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

  void doReadDataObjects();
  void doWriteDataObjects();

  void readDataObjects(QString filePath);
  void writeDataObjects(QString filePath);

private:
  Ui::QtestceplibFileIOMainWindow *ui;
  QtestceplibFileIODocument *m_Document;
  mutable QMutex m_Mutex;
};

#endif // QTESTCEPLIBFILEIOMAINWINDOW_H
