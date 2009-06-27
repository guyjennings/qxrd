/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.h,v 1.4 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

#include "qcepmacros.h"

#include "qxrdacquisitionoperations.h"

#include <QMutex>
#include <QWaitCondition>
#include <QVector>

class QxrdAcquisitionThread;

class QxrdAcquisitionPerkinElmer : public QxrdAcquisitionOperations
{
  Q_OBJECT;
public:
  QxrdAcquisitionPerkinElmer(QxrdAcquisitionThread *thread);
  ~QxrdAcquisitionPerkinElmer();

public slots:
  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();
  void initialize();

signals:
  void acquireStarted(int dark);
  void acquireComplete(int dark);
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void oneReadoutModeChanged(int mode, double value);

public:
//  bool onEndFrame();
  void onEndAcquisition();
  QVector<double> readoutTimes();
  double  readoutTime() const;
  int acquisitionStatus(double time);
  void onEndFrameCallback();

protected:
  void acquisition(int isDark);
  void haltAcquire();

private slots:
  void onEndFrame();

private:
  void acquisitionError(int n);
  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);

private:
  mutable QMutex         m_Mutex;
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;
  int                    m_Cancelling;
  int                    m_AcquireDark;
  int                    m_NRows;
  int                    m_NCols;
  int                    m_ExposuresToSum;
  int                    m_FilesInSequence;
  int                    m_CurrentExposure;
  int                    m_CurrentFile;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
  QxrdImageData         *m_AcquiredData;
  QVector<double>        m_ReadoutTimes;
  HEADER_IDENT("$Id: qxrdacquisitionperkinelmer.h,v 1.4 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDACQUISITIONPERKINELMER_H

/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.h,v $
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

