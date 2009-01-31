#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserver.h"
#include "qxrdserverthread.h"
#include "qxrdacquisition.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QScriptEngine>
#include <QTime>
#include <QtConcurrentRun>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

Q_DECLARE_METATYPE(QxrdAcquisition*);
Q_DECLARE_METATYPE(QVector<double>);
Q_DECLARE_METATYPE(QVector<int>);
Q_DECLARE_METATYPE(QVector<QString>);
Q_DECLARE_METATYPE(QList<double>);
Q_DECLARE_METATYPE(QList<int>);

static QxrdApplication* g_Application = NULL;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Window(NULL),
    m_ServerThread(NULL),
    m_Server(NULL),
    m_Acquisition(NULL)
{
  setObjectName("qxrdapplication");

  printf("application thread %p\n", thread());

  g_Application = this;

  setOrganizationName("bessrc");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");

  m_Window = new QxrdWindow(this);
  m_Window -> show();

  m_Acquisition = new QxrdAcquisition(this);

  m_Window -> setAcquisition(m_Acquisition);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  connect(m_Window->acquireButton(), SIGNAL(clicked()), 
	  this, SLOT(acquire()));

  connect(m_Window->acquireAction(), SIGNAL(triggered()), 
	  this, SLOT(acquire()));

  //  connect(m_Window->m_SetReferenceButton, SIGNAL(clicked()),
  //  acquisition(), SLOT(setReferenceData()));

  // connect(m_Window->m_SetDarkButton, SIGNAL(clicked()),
  //  acquisition(), SLOT(setDarkData()));

//   connect(this, SIGNAL(doAcquire(double)),
// 	  m_Acquisition, SLOT(acquire(double)));

//   connect(m_Window->sequenceButton(), SIGNAL(clicked()), 
// 	  this, SLOT(sequence()));

//   connect(m_Window->sequenceAction(), SIGNAL(triggered()), 
// 	  this, SLOT(sequence()));

  connect(m_Acquisition, SIGNAL(newDataAvailable()),
	  this, SLOT(newDataAvailable()));

  connect(m_Window->m_ActionPreferences, SIGNAL(triggered()),
	  this, SLOT(doPreferences()));

  connect(m_Window->m_ActionEvaluateScript, SIGNAL(triggered()),
	  this, SLOT(doScript()));

  connect(m_Window->m_ActionSetReferenceData, SIGNAL(triggered()),
	  acquisition(), SLOT(setReferenceData()));

  connect(m_Window->m_ActionSetDarkData, SIGNAL(triggered()),
	  acquisition(), SLOT(setDarkData()));

  connect(m_Window->m_EvaluateScript, SIGNAL(clicked()),
	  this, SLOT(doScript()));

  connect(m_Acquisition, SIGNAL(resultsChanged()),
	  model, SLOT(resultsChanged()));

  m_ServerThread = new QxrdServerThread(this, m_Acquisition);
  connect(m_ServerThread, SIGNAL(serverRunning()), this, SLOT(serverRunning()));
  m_ServerThread -> start();

  qScriptRegisterSequenceMetaType< QList<double> >(&m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<double> >(&m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<int> >(&m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QVector<QString> >(&m_ScriptEngine);
  qScriptRegisterSequenceMetaType< QList<int> >(&m_ScriptEngine);

  m_ScriptEngine.globalObject().setProperty("acquire",m_ScriptEngine.newFunction(QxrdApplication::acquireFunc));
  m_ScriptEngine.globalObject().setProperty("newfile",m_ScriptEngine.newFunction(QxrdApplication::newFileFunc));
  m_ScriptEngine.globalObject().setProperty("newscan",m_ScriptEngine.newFunction(QxrdApplication::newScanFunc));
  m_ScriptEngine.globalObject().setProperty("savedata",m_ScriptEngine.newFunction(QxrdApplication::saveDataFunc));
  m_ScriptEngine.globalObject().setProperty("readdata",m_ScriptEngine.newFunction(QxrdApplication::readDataFunc));
  m_ScriptEngine.globalObject().setProperty("channel",m_ScriptEngine.newFunction(QxrdApplication::fitterFunc));
  m_ScriptEngine.globalObject().setProperty("acquisition",m_ScriptEngine.newFunction(QxrdApplication::acquisitionFunc));
  m_ScriptEngine.globalObject().setProperty("parameters",m_ScriptEngine.newFunction(QxrdApplication::parametersFunc));
}

QxrdApplication::~QxrdApplication()
{
  delete m_AcquisitionParms;
  delete m_Acquisition;
  delete m_ServerThread;
  delete m_EpicsInterface;
}

void QxrdApplication::serverRunning()
{
  m_Server = m_ServerThread->server();

  connect(m_Server, SIGNAL(executeCommand(QString)),
	  this, SLOT(executeCommand(QString)));

  connect(this, SIGNAL(finishedCommand(QString)),
	  m_Server, SLOT(finishedCommand(QString)));
}

void QxrdApplication::executeCommand(QString cmd)
{
  QScriptValue result = m_ScriptEngine.evaluate(cmd);

  if (m_ScriptEngine.hasUncaughtException()) {
    emit finishedCommand(tr("Error: ") + m_ScriptEngine.uncaughtException().toString());
  } else {
    emit finishedCommand(result.toString());
  }
}

QScriptValue QxrdApplication::evaluate(QString cmd)
{
  return m_ScriptEngine.evaluate(cmd);
}

void QxrdApplication::shutdownThreads()
{
  m_ServerThread -> shutdown();
  m_EpicsInterface -> shutdown();
}

QxrdAcquisition *QxrdApplication::acquisition()
{
  return m_Acquisition;
}

QxrdAcquisitionParms *QxrdApplication::acquisitionParms()
{
  return m_AcquisitionParms;
}

void QxrdApplication::printMessage(QString msg)
{
  printf("%s\n", qPrintable(msg));
}

QxrdFillPattern *QxrdApplication::fillPattern()
{
  return m_EpicsInterface->fillPattern();
}

void QxrdApplication::acquire()
{
  double integ = m_Window -> integrationTime();
  int    nreps = m_Window -> nRepetitions();

  QxrdAcquisition *acq = acquisition();

  acquire(integ, nreps);
}

void QxrdApplication::newDataAvailable()
{
  QxrdAcquisition *acq = acquisition();

  int nchan = acq->parms()->nChannels();
  int nsamp = acq->parms()->nSamples();
  
  printf("nchan %d, nsamp %d\n", nchan, nsamp);

  m_Data.resize(nchan+1);

  for (int j=0; j<nchan; j++) {
    m_Data[j+1].resize(nsamp);
    m_Data[j+1]=acq->readData(j);
  }

  m_Data[0].resize(nsamp);

  for (int i=0; i<nsamp; i++) {
    m_Data[0][i] = i;
  }

  m_Window -> newDataAvailable();
}

void QxrdApplication::loadData()
{
  QString loadName = QFileDialog::getOpenFileName(NULL, tr("Load Acquisitions"));

  if (!loadName.isNull()) {
    readData(loadName);
  }
}

void QxrdApplication::readData(QString loadName)
{
  if (!loadName.isNull()) {
    QFile file(loadName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    int ncols=0, nrows=0;

    QByteArray buffer(20000, 0);

    while (!file.atEnd()) {
      long n = file.readLine(buffer.data(), buffer.count());

      char* p=buffer.data();
      int nf = 0;
      while(1) {
	char *endp;
	double val = strtod(p, &endp);
	if (p != endp) {
	  nf++;
	  p = endp;
	} else {
	  break;
	}
      }

      if (nf) {
	nrows ++;
      }

      if (nf>ncols) {
	ncols = nf;
      }
    }
    
    printf("%d rows, %d cols\n", nrows, ncols);

    file.seek(0);

    m_Data.resize(ncols);
    for (int i=0; i<ncols; i++) { // skip column 0
      m_Data[i].resize(nrows);
    }

    for (int r=0; r<nrows; r++) {
      long n = file.readLine(buffer.data(), buffer.count());

      char* p=buffer.data();
      
      for (int c=0; c<ncols; c++) {
	char *endp;
	double val = strtod(p, &endp);
	if (p != endp) {
	  m_Data[c][r] = val;
	  p = endp;
	} else {
	  break;
	}
      }
    }

    m_Window -> newDataAvailable();
  }
}

void QxrdApplication::saveData()
{
  QString saveName = QFileDialog::getSaveFileName(NULL, tr("Save Acquisitions"));

  if (!saveName.isNull()) {
    writeData(saveName);
  }
}

void QxrdApplication::writeData(QString saveName)
{
  if (!saveName.isNull()) {
    FILE *f = fopen(qPrintable(saveName),"w");

    int nacq = m_Data.count();
    int n = m_Data[0].count();

    for (int r=0; r<n; r++) {
      fprintf(f, "%d\t", r);

      for (int i=0; i<nacq*2; i++) {
	fprintf(f, "%g\t", m_Data[i][r]);
      }
      fprintf(f,"\n");
    }
      
    fclose(f);
  }
}

void QxrdApplication::selectData(int iter)
{
  if (iter >= 0 && iter < (m_Data.count()/2)) {
    int ch0 = iter*2;
    int ch1 = iter*2+1;

    acquisition()->writeData(0, m_Data[ch0]);
    acquisition()->writeData(1, m_Data[ch1]);
  }
}

void QxrdApplication::doScript()
{
  QString script = m_Window -> m_InputScript -> toPlainText();

  QScriptValue result = m_ScriptEngine.evaluate(script);

  if (m_ScriptEngine.hasUncaughtException()) {
    int line = m_ScriptEngine.uncaughtExceptionLineNumber();
    m_Window -> m_OutputResult -> append(tr("Error in line %1").arg(line));
    m_Window -> m_OutputResult -> append(m_ScriptEngine.uncaughtException().toString());
  } else {
    m_Window -> m_OutputResult -> append(result.toString());
  }
}

void QxrdApplication::doPreferences()
{
  QxrdPreviewDialog d(this);
  d.exec();
}

double QxrdApplication::data(int chan, int bin)
{
  if (chan >= 0 && chan < m_Data.count()) {
    if (bin >= 0 && bin < m_Data[chan].count()) {
      return m_Data[chan][bin];
    }
  }

  return 0;
}

double *QxrdApplication::data(int chan)
{
  if (chan >= 0 && chan < m_Data.count()) {
    return m_Data[chan].data();
  } else {
    return NULL;
  }
}

QVector< QVector<double> > QxrdApplication::data()
{
  return m_Data;
}

QVector< QVector<double> > QxrdApplication::referenceData()
{
  return acquisition()->readReference();
}

QVector< QVector<double> > QxrdApplication::darkData()
{
  return acquisition()->readDark();
}

int QxrdApplication::referenceAvailable() const
{
  return m_Acquisition->referenceAvailable();
}

int QxrdApplication::darkAvailable() const
{
  return m_Acquisition->darkAvailable();
}

int QxrdApplication::nSamples(int chan)
{
  if (chan >= 0 && chan < m_Data.count()) {
    return m_Data[chan].count();
  } else {
    return 0;
  }
}

void QxrdApplication::loadBinaryData()
{
}

void QxrdApplication::readBinaryData(QString file)
{
}

void QxrdApplication::saveBinaryData()
{
}

void QxrdApplication::writeBinaryData(QString file)
{
}

void QxrdApplication::printAcquisitionStatus(QString msg)
{
  printf("%s : ", qPrintable(msg));

  if (m_AcquisitionFuture.isRunning()) printf("running");
  if (m_AcquisitionFuture.isFinished()) printf("finished");
  if (m_AcquisitionFuture.isCanceled()) printf("canceled");
  if (m_AcquisitionFuture.isPaused()) printf("paused");

  printf("\n");
}

void QxrdApplication::acquire(double time)
{
  acquire(time,1);
}

void QxrdApplication::acquire(double time, int nreps)
{
  if (acquisition() -> concurrent() & 2) {
    printAcquisitionStatus(tr("QxrdApplication::acquire(%1) [wait]{").arg(time));

    m_AcquisitionFuture.waitForFinished();

    m_AcquisitionFuture = 
      QtConcurrent::run(acquisition(),
			&QxrdAcquisition::acquire, time, nreps);

    printAcquisitionStatus(tr("QxrdApplication::acquire(%1) [wait]}").arg(time));
  } else {
    printAcquisitionStatus(tr("QxrdApplication::acquire(%1) [nowait]{").arg(time));

    acquisition() -> acquire(time, nreps);

    printAcquisitionStatus(tr("QxrdApplication::acquire(%1) [nowait]}").arg(time));
  }
}

void QxrdApplication::saveData(QVector<double> vals)
{
  printAcquisitionStatus(tr("QxrdApplication::saveData {"));

  if (acquisition() -> concurrent() & 2) {
    m_AcquisitionFuture.waitForFinished();
  }

  printAcquisitionStatus(tr("QxrdApplication::saveData  .."));

  acquisition() -> saveAcquisitionResult(vals);

  printAcquisitionStatus(tr("QxrdApplication::saveData }"));
}

QScriptValue QxrdApplication::newFileFunc(QScriptContext *context, QScriptEngine *engine)
{
  printf("newfile()\n");
  int nargs = context->argumentCount();

  if (nargs >= 1) {
    QString name = context->argument(0).toString();
    int nscan = 0;

    if (context->argumentCount() >= 1) {
      nscan = context->argument(1).toInteger();
    }

    g_Application -> acquisition() -> newSaveFile(name, nscan);
  }

  return QScriptValue(engine, nargs);
}

QScriptValue QxrdApplication::newScanFunc(QScriptContext *context, QScriptEngine *engine)
{
  printf("newscan()\n");

  QVector<QString> hdr;

  int nargs = context->argumentCount();

  for (int i = 0; i<nargs; i++) {
    hdr.append(context->argument(i).toString());
  }

  g_Application -> acquisition() -> startNewScan(hdr);

  return QScriptValue(engine, nargs);
}

QScriptValue QxrdApplication::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  printf("acquire()\n");
  
  if (context->argumentCount() == 1) {
    double time = context->argument(0).toNumber();

    g_Application -> acquire(time);
  } else if (context->argumentCount() == 2) {
    double time = context->argument(0).toNumber();
    int nreps = context->argument(0).toInteger();

    g_Application -> acquire(time,nreps);
  }    

  return QScriptValue(engine,0);
}

QScriptValue QxrdApplication::saveDataFunc(QScriptContext *context, QScriptEngine *engine)
{
  printf("savedata()\n");

  QVector<double> vals;


  int nargs = context->argumentCount();

  for (int i = 0; i<nargs; i++) {
    vals.append(context->argument(i).toNumber());
  }

  g_Application -> saveData(vals);

  return QScriptValue(engine, nargs);
}

QScriptValue QxrdApplication::readDataFunc(QScriptContext *context, QScriptEngine *engine)
{
  int nargs = context->argumentCount();
  
  switch (nargs) {
  case 1:
    {
      int n1 = context->argument(0).toInteger();
      QVector <double> res = g_Application -> data().value(n1);
      return qScriptValueFromSequence(engine, res);
    }
  case 2:
    {
      int n1 = context->argument(0).toInteger();
      int n2 = context->argument(1).toInteger();
      double res = g_Application -> data().value(n1).value(n2);
      return QScriptValue(engine, res);
    }
  case 3:
    {
      int n1 = context->argument(0).toInteger();
      int n2 = context->argument(1).toInteger();
      int n3 = context->argument(2).toInteger();
      QVector <double> res = g_Application -> data().value(n1).mid(n2,n3);
      return qScriptValueFromSequence(engine, res);
    }
  default:
    return QScriptValue(engine, 0);
  }
}

QScriptValue QxrdApplication::fitterFunc(QScriptContext *context, QScriptEngine *engine)
{
  int nargs = context->argumentCount();

  QxrdFitter *f = NULL;

  if (nargs == 1) {
    int n = context->argument(0).toInteger();

    f = g_Application -> acquisition()->fitter(n);
  }

  return engine->newQObject(f);
}

QScriptValue QxrdApplication::acquisitionFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine->newQObject(g_Application -> acquisition());
}

QScriptValue QxrdApplication::parametersFunc(QScriptContext *context, QScriptEngine *engine)
{
  return engine->newQObject(g_Application -> acquisitionParms());
}

