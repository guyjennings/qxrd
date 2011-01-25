#include "qxrdsynchronizedacquisition.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisition *acq) :
  m_Acquisition(acq),
  m_NIDAQPlugin(NULL),
  QObject(NULL)
{
}

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int currentExposure, int currentFile)
{
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}
