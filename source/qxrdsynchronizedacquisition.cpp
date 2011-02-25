#include "qxrdsynchronizedacquisition.h"
#include "qxrdnidaqplugininterface.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisition *acq) :
  m_Acquisition(acq),
  m_NIDAQPlugin(NULL),
  QObject(NULL)
{
}

void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int currentExposure, int currentFile)
{
//  printf("Sync acquired frame available %d, %d\n", currentExposure, currentFile);

  if (m_NIDAQPlugin) {
    m_NIDAQPlugin->setAnalogOut(0, currentExposure);
  }
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}
