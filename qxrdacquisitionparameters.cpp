/******************************************************************
*
*  $Id: qxrdacquisitionparameters.cpp,v 1.16 2009/09/23 19:00:36 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionparameters.h"

#include <QMutexLocker>
#include "qxrdsettings.h"
#include <QMetaProperty>
#include <QStringList>
#include <QThread>

QxrdAcquisitionParameters::QxrdAcquisitionParameters(QxrdDataProcessor *proc)
  : QObject(),
    m_Debug(this,"debug", 0),
    m_ExposureTime(this,"exposureTime",0),
    m_ReadoutMode(this,"readoutMode",0),
    m_SummedExposures(this,"summedExposures",1),
    m_PreTriggerFiles(this,"preTriggerFiles", 0),
    m_PostTriggerFiles(this,"postTriggerFiles",1),
    m_Trigger(this,"triggered",0),
    m_FileIndex(this,"fileIndex",0),
    m_FilePattern(this,"filePattern",""),
    m_DarkSummedExposures(this,"darkSummedExposures",1),
    m_CameraGain(this,"cameraGain",0),
    m_BinningMode(this,"binningMode",0),
    m_FileBase(this,"fileBase",""),
    m_SimulationMode(this, "simulationMode", false),
    m_NRows(this, "nRows", 2048),
    m_NCols(this, "nCols", 2048),
    m_AcquireDark(this, "acquireDark", 0),
    m_Cancelling(this, "cancelling", 0),
    m_ExposuresToSum(this, "exposuresToSum", 1),
    m_FilesInAcquiredSequence(this, "filesInAcquiredSequence", 1),
    m_TotalBufferSize(this,"totalBufferSize", 1500000000),
    m_Raw16SaveTime(this,"raw16SaveTime", 0.1),
    m_Raw32SaveTime(this,"raw32SaveTime", 0.2),
    m_UserComment1(this,"userComment1",""),
    m_UserComment2(this,"userComment2",""),
    m_UserComment3(this,"userComment3",""),
    m_UserComment4(this,"userComment4",""),
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qxrdacquisitionparameters.cpp,v 1.16 2009/09/23 19:00:36 jennings Exp $")
{
}

void QxrdAcquisitionParameters::dynamicProperties()
{
  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    emit printMessage(tr("acquisition.%1\n").arg(name.data()));
  }
}

void QxrdAcquisitionParameters::copyDynamicProperties(QObject *dest)
{
  if (dest) {
    QByteArray name;

    foreach(name, dynamicPropertyNames()) {
      dest -> setProperty(name.data(), property(name.data()));
    }
  }
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

//void QxrdAcquisitionParameters::setDynamicProperty(QString name, QVariant value)
//{
//  if (QThread::currentThread() == thread()) {
//    setProperty(qPrintable(name), value);
//  } else {
//    QTimer::singleShot
//    QMetaObject::invokeMethod(this, "setDynamicProperty", Qt::QueuedConnection, Q_ARG(QString,name), Q_ARG(QVariant,value));
//  }
//}
//
//QVariant QxrdAcquisitionParameters::dynamicProperty(QString name)
//{
//  return property(qPrintable(name));
//}
//
/******************************************************************
*
*  $Log: qxrdacquisitionparameters.cpp,v $
*  Revision 1.16  2009/09/23 19:00:36  jennings
*  Removed dynamic property support - it's not thread-safe and doesn't work
*  under windows
*
*  Revision 1.15  2009/09/22 20:37:23  jennings
*  Dynamic properties attached to the acquisition object are propagated through to saved images
*
*  Revision 1.14  2009/09/22 13:48:17  jennings
*  Some support for dynamic properties during acquisition
*
*  Revision 1.13  2009/09/21 18:12:54  jennings
*  Added 'triggered', 'usercomment{1-4}' properties to data
*
*  Revision 1.12  2009/09/04 12:45:38  jennings
*  Added binning mode parameter
*
*  Revision 1.11  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.10  2009/08/26 16:58:13  jennings
*  Added TotalBufferSize property to specify the amount of memory to use for acquisition buffers
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
*  Revision 1.5  2009/07/07 22:05:25  jennings
*  *** empty log message ***
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

