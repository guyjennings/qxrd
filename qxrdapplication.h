#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include <QApplication>
#include <QVector>
#include <QScriptEngine>
#include <QFuture>

class QxrdWindow;
class QxrdServerThread;
class QxrdServer;
class QxrdAcquisition;
class QDoubleSpinBox;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

 public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisition *acquisition();
  void acquire(double time);
  void acquire(double time, int nreps);
  void saveData(QVector<double> vals);

  static QScriptValue newFileFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newScanFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue saveDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue readDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fitterFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquisitionFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue parametersFunc(QScriptContext *context, QScriptEngine *engine);

  QScriptValue evaluate(QString cmd);

 public slots:
  void shutdownThreads();
  void serverRunning();
/*   void acquisitionRunning(); */
  void printMessage(QString msg);
  void loadData();
  void saveData();
  void acquire();
  void doScript();
  void doPreferences();
  void executeCommand(QString cmd);
  void newDataAvailable();

  void readData(QString file);
  void writeData(QString file);

  void loadBinaryData();
  void saveBinaryData();
  void readBinaryData(QString file);
  void writeBinaryData(QString file);

  void selectData(int iter);
  double *data(int chan);
  double data(int chan, int bin);
  int nSamples(int chan=0);
  QVector< QVector<double> > data();
  QVector< QVector<double> > referenceData();
  QVector< QVector<double> > darkData();
  int darkAvailable() const;
  int referenceAvailable() const;

 signals:
  void finishedCommand(QString cmd);

 private:
  void printAcquisitionStatus(QString msg);

 private:
  QxrdWindow                    *m_Window;
  QxrdServerThread              *m_ServerThread;
  QxrdServer                    *m_Server;
  QxrdAcquisition               *m_Acquisition;
  QVector< QVector<double> >     m_Data;
  QScriptEngine                  m_ScriptEngine;
  QFuture<void>                  m_AcquisitionFuture;
  QFuture<void>                  m_SaveDataFuture;
};

#endif
