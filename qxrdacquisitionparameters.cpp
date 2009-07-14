/******************************************************************
*
*  $Id: qxrdacquisitionparameters.cpp,v 1.8 2009/07/14 20:07:00 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionparameters.h"

#include <QMutexLocker>
#include "qxrdsettings.h"
#include <QMetaProperty>
#include <QStringList>

QxrdAcquisitionParameters::QxrdAcquisitionParameters(QxrdDataProcessor *proc)
  : QObject(),
    m_Debug(this,"debug", 0),
    m_ExposureTime(this,"exposureTime",0),
    m_ReadoutMode(this,"readoutMode",0),
    m_SummedExposures(this,"summedExposures",1),
    m_FilesInSequence(this,"filesInSequence",1),
    m_FileIndex(this,"fileIndex",0),
    m_FilePattern(this,"filePattern",""),
    m_OutputDirectory(this,"outputDirectory", ""),
    m_DarkSummedExposures(this,"darkSummedExposures",1),
    m_CameraGain(this,"cameraGain",0),
    m_FileName(this,"fileName",""),
    m_FileBase(this,"fileBase",""),
    m_DarkImagePath(this, "darkImagePath", ""),
    m_BadPixelsPath(this, "badPixelsPath", ""),
    m_GainMapPath(this, "gainMapPath", ""),
    m_PerformDarkSubtraction(this, "performDarkSubtraction", true),
    m_SaveRawImages(this, "saveRawImages", true),
    m_PerformBadPixels(this, "performBadPixels", true),
    m_PerformGainCorrection(this, "performGainCorrection", true),
    m_SimulationMode(this, "simulationMode", false),
    m_NRows(this, "nRows", 2048),
    m_NCols(this, "nCols", 2048),
    m_AcquireDark(this, "acquireDark", 0),
    m_Cancelling(this, "cancelling", 0),
    m_ExposuresToSum(this, "exposuresToSum", 1),
    m_FilesInAcquiredSequence(this, "filesInAcquiredSequence", 1),
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qxrdacquisitionparameters.cpp,v 1.8 2009/07/14 20:07:00 jennings Exp $")
{
}

void QxrdAcquisitionParameters::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdAcquisitionParameters::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

/******************************************************************
*
*  $Log: qxrdacquisitionparameters.cpp,v $
*  Revision 1.8  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.7  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.6  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.5  2009/07/07 22:05:25  jennings
*  *** empty log message ***
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

