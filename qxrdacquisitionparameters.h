/******************************************************************
*
*  $Id: qxrdacquisitionparameters.h,v 1.8 2009/07/14 20:07:00 jennings Exp $
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

  Q_PROPERTY(int    filesInSequence  READ get_FilesInSequence WRITE set_FilesInSequence STORED false);
  QCEP_INTEGER_PROPERTY(FilesInSequence);

  Q_PROPERTY(int    fileIndex        READ get_FileIndex WRITE set_FileIndex);
  QCEP_INTEGER_PROPERTY(FileIndex);

  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern);
  QCEP_STRING_PROPERTY(FilePattern);

  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory);
  QCEP_STRING_PROPERTY(OutputDirectory);

  Q_PROPERTY(int     darkSummedExposures READ get_DarkSummedExposures WRITE set_DarkSummedExposures);
  QCEP_INTEGER_PROPERTY(DarkSummedExposures);

  Q_PROPERTY(int     cameraGain      READ get_CameraGain WRITE set_CameraGain);
  QCEP_INTEGER_PROPERTY(CameraGain);

  Q_PROPERTY(QString fileName        READ get_FileName WRITE set_FileName);
  QCEP_STRING_PROPERTY(FileName);

  Q_PROPERTY(QString fileBase        READ get_FileBase WRITE set_FileBase);
  QCEP_STRING_PROPERTY(FileBase);

  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath);
  QCEP_STRING_PROPERTY(DarkImagePath);

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath);
  QCEP_STRING_PROPERTY(BadPixelsPath);

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath);
  QCEP_STRING_PROPERTY(GainMapPath);

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction);
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction);

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages);
  QCEP_BOOLEAN_PROPERTY(SaveRawImages);

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels);
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels);

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection);
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection);

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

//  Q_PROPERTY(int     cameraMode      READ cameraMode WRITE setCameraMode);
//  Q_PROPERTY(int     frameSyncMode   READ frameSyncMode WRITE setFrameSyncMode);
//  Q_PROPERTY(int     timerSync       READ timerSync WRITE setTimerSync);

private:
  mutable QMutex         m_Mutex;
  HEADER_IDENT("$Id: qxrdacquisitionparameters.h,v 1.8 2009/07/14 20:07:00 jennings Exp $");
};

#endif // QXRDACQUISITIONPARAMETERS_H

/******************************************************************
*
*  $Log: qxrdacquisitionparameters.h,v $
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

