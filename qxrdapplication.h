#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include <QApplication>
#include <QScriptEngine>

class QxrdWindow;
class QxrdServerThread;
class QxrdAcquisitionThread;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

 public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisitionThread *acquisitionThread();
  QxrdWindow *window();

  void executeScript(QString cmd);

 public slots:
  void shutdownThreads();
  void possiblyQuit();

 signals:
  void printMessage(QString msg);

 public:
  bool wantToQuit();

 private:
  QxrdWindow                    *m_Window;
  QxrdServerThread              *m_ServerThread;
  QxrdAcquisitionThread         *m_AcquisitionThread;
};

#endif
