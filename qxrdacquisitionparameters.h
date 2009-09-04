/******************************************************************
*
*  $Id: qxrdacquisitionparameters.h,v 1.13 2009/09/04 12:45:38 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONPARAMETERS_H
#define QXRDACQUISITIONPARAMETERS_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>

class QxrdSettings;
class QxrdDataProcessor;

#include "qcepproperty.h"

class QxrdAcquisitionParameters : public QObject
{
  Q_OBJECT;
public:
  QxrdAcquisitionParameters(QxrdDataProcessor *proc);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  Q_PROPERTY(int    debug            READ get_Debug WRITE set_Debug);
  QCEP_INTEGER_PROPERTY(Debug);

  Q_PROPERTY(double exposureTime     READ get_ExposureTime WRITE set_ExposureTime);
  QCEP_DOUBLE_PROPERTY(ExposureTime);

  Q_PROPERTY(int    readoutMode      READ get_ReadoutMode WRITE set_ReadoutMode);
  QCEP_INTEGER_PROPERTY(ReadoutMode);

  Q_PROPERTY(int    summedExposures  READ get_SummedExposures WRITE set_SummedExposures);
  QCEP_INTEGER_PROPERTY(SummedExposures);

  Q_PROPERTY(int     preTriggerFiles    READ get_PreTriggerFiles WRITE set_PreTriggerFiles);
  QCEP_INTEGER_PROPERTY(PreTriggerFiles);

  Q_PROPERTY(int    postTriggerFiles  READ get_PostTriggerFiles WRITE set_PostTriggerFiles);
  QCEP_INTEGER_PROPERTY(PostTriggerFiles);

  Q_PROPERTY(int    trigger  READ get_Trigger WRITE set_Trigger);
  QCEP_INTEGER_PROPERTY(Trigger);

  Q_PROPERTY(int    fileIndex        READ get_FileIndex WRITE set_FileIndex);
  QCEP_INTEGER_PROPERTY(FileIndex);

  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern);
  QCEP_STRING_PROPERTY(FilePattern);

  Q_PROPERTY(int     darkSummedExposures READ get_DarkSummedExposures WRITE set_DarkSummedExposures);
  QCEP_INTEGER_PROPERTY(DarkSummedExposures);

  Q_PROPERTY(int     cameraGain      READ get_CameraGain WRITE set_CameraGain);
  QCEP_INTEGER_PROPERTY(CameraGain);

  Q_PROPERTY(int     binningMode      READ get_BinningMode WRITE set_BinningMode);
  QCEP_INTEGER_PROPERTY(BinningMode);

  Q_PROPERTY(QString fileBase        READ get_FileBase WRITE set_FileBase);
  QCEP_STRING_PROPERTY(FileBase);

  Q_PROPERTY(int     simulationMode      READ get_SimulationMode WRITE set_SimulationMode STORED false);
  QCEP_INTEGER_PROPERTY(SimulationMode);

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows);
  QCEP_INTEGER_PROPERTY(NRows);

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols);
  QCEP_INTEGER_PROPERTY(NCols);

  Q_PROPERTY(int     acquireDark      READ get_AcquireDark WRITE set_AcquireDark STORED false);
  QCEP_INTEGER_PROPERTY(AcquireDark);

  Q_PROPERTY(int     cancelling      READ get_Cancelling WRITE set_Cancelling STORED false);
  QCEP_INTEGER_PROPERTY(Cancelling);

  Q_PROPERTY(int     exposuresToSum      READ get_ExposuresToSum WRITE set_ExposuresToSum STORED false);
  QCEP_INTEGER_PROPERTY(ExposuresToSum);

  Q_PROPERTY(int     filesInAcquiredSequence      READ get_FilesInAcquiredSequence WRITE set_FilesInAcquiredSequence STORED false);
  QCEP_INTEGER_PROPERTY(FilesInAcquiredSequence);

  Q_PROPERTY(int     totalBufferSize    READ get_TotalBufferSize WRITE set_TotalBufferSize);
  QCEP_INTEGER_PROPERTY(TotalBufferSize);

  Q_PROPERTY(double     raw16SaveTime    READ get_Raw16SaveTime WRITE set_Raw16SaveTime);
  QCEP_DOUBLE_PROPERTY(Raw16SaveTime);

  Q_PROPERTY(double     raw32SaveTime    READ get_Raw32SaveTime WRITE set_Raw32SaveTime);
  QCEP_DOUBLE_PROPERTY(Raw32SaveTime);

//  Q_PROPERTY(int     cameraMode      READ cameraMode WRITE setCameraMode);
//  Q_PROPERTY(int     frameSyncMode   READ frameSyncMode WRITE setFrameSyncMode);
//  Q_PROPERTY(int     timerSync       READ timerSync WRITE setTimerSync);

private:
  mutable QMutex         m_Mutex;
  HEADER_IDENT("$Id: qxrdacquisitionparameters.h,v 1.13 2009/09/04 12:45:38 jennings Exp $");
};

#endif // QXRDACQUISITIONPARAMETERS_H

/******************************************************************
*
*  $Log: qxrdacquisitionparameters.h,v $
*  Revision 1.13  2009/09/04 12:45:38  jennings
*  Added binning mode parameter
*
*  Revision 1.12  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.11  2009/08/26 16:58:13  jennings
*  Added TotalBufferSize property to specify the amount of memory to use for acquisition buffers
*
*  Revision 1.10  2009/07/20 00:29:49  jennings
*  filesInSequence property should be saved
*
*  Revision 1.9  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.8  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.7  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.6  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

