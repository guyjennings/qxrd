/******************************************************************
*
*  $Id: qxrdacquisitionparameters.cpp,v 1.7 2009/07/13 23:19:37 jennings Exp $
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
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qxrdacquisitionparameters.cpp,v 1.7 2009/07/13 23:19:37 jennings Exp $")
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

