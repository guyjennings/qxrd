#ifndef QAVRGACQUISITION_H
#define QAVRGACQUISITION_H

#include <QTime>
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QSemaphore>
#include <QFuture>
#include "AcqirisD1Import.h"

#include "qavrgacquisitionparms.h"

class QavrgApplication;
class QavrgFitter;

class QavrgAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QavrgAcquisition(QavrgApplication *app);
  ~QavrgAcquisition();

 public slots:
  void acquire(double integ, int nreps);
  void resultsAvailable(int chan);
  void savingComplete(int chan);

  QVector< QVector<double> > readData();
  QVector<double> readData(int chan, int start, int nbins);
  QVector<double> readData(int chan);
  double readData(int chan, int bin);

  QVector< QVector<double> > readReference();
  QVector<double> readReference(int chan, int start, int nbins);
  QVector<double> readReference(int chan);
  double readReference(int chan, int bin);

  QVector< QVector<double> > readDark();
  QVector<double> readDark(int chan, int start, int nbins);
  QVector<double> readDark(int chan);
  double readDark(int chan, int bin);

  void writeData(int chan, QVector<double> data);
  void setReferenceData();
  void referenceloaded(int f);
  void darkloaded(int f);

  void setDarkData();

  void newSaveFile(QString f, int n);
  void startNewScan(QVector<QString> header);
  void saveAcquisitionResult(QVector<double> data);

  QVector<double> readResult(int chan, int parm, int start, int nbins);
  QVector<double> readResult(int chan, int parm);
  double readResult(int chan, int parm, int bin);
  QString resultHeader(int chan, int parm, int bin);
  double readResultAverage(int chan, int parm, int start, int nbins);
  double readResultBunchAverage(int chan, int parm, int bunch, int start, int norbits);

  void fillPatternChanged();

  void parametersChanged();

 public:
  QavrgAcquisitionParms *parms();
  void setParms(QavrgAcquisitionParms *parms);
/*   QMutex* mutex(); */
  QavrgFitter  *fitter(int chan);
  int resultSize();
  int totalChannels();
  int concurrent() const;

  int referenceAvailable() const;
  int darkAvailable() const;

  void loadParameters();
  void saveParameters();

  void loadReferenceData();
  void saveReferenceData();

  void loadDarkData();
  void saveDarkData();

 signals:
  void newDataAvailable();
  void resultsChanged();

 private:
  void findDevices();
  void configureAverager();
  void startAcquisition(int rep);
  void waitAcquisition(int rep);
  void readoutAcquisition(int rep);
  void checkStatus(ViStatus status);

  QString parametersPath(int chan);
  QString referenceDataPath(int chan);
  QString darkDataPath(int chan);

 private:
  QMutex            m_Mutex;
  QavrgApplication *m_Application;
  long              m_NInstruments;
  ViSession        *m_InstrumentID;
  QavrgAcquisitionParms *m_AcquisitionParms;
  long             *m_NChannels;
  long              m_TotalChannels;
  QVector< QVector<double> > m_Accumulator;
  QVector< QVector<double> > m_Data;
  QVector< QVector<double> > m_DarkData;
  QVector< QVector<double> > m_ReferenceData;
  QVector< QavrgFitter* > m_Fitters;
  QVector< QFuture<void> > m_Futures;
  QTime             m_Time;
  FILE             *m_SaveFile;
  QString           m_SaveFileName;
  int               m_ScanNumber;
  int               m_ParametersChanged;
  int               m_ReferenceChanged;
  int               m_DarkChanged;
  int               m_ReferenceAvailable;
  int               m_DarkAvailable;
};

#endif
