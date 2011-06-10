#include "qxrdacquisitionfilewatcher.h"
#include "qxrdacquirefilewatcherdialog.h"
#include "qxrdapplication.h"
#include "qxrdwindow.h"

QxrdAcquisitionFileWatcher::QxrdAcquisitionFileWatcher(QxrdDataProcessor *proc, QxrdAllocator *allocator) :
    QxrdAcquisition(proc, allocator)
{
}

void QxrdAcquisitionFileWatcher::initialize()
{
}

void QxrdAcquisitionFileWatcher::onExposureTimeChanged(double newTime)
{
}

void QxrdAcquisitionFileWatcher::onBinningModeChanged(int newMode)
{
}

void QxrdAcquisitionFileWatcher::onCameraGainChanged(int newGain)
{
}

void QxrdAcquisitionFileWatcher::beginAcquisition()
{
}

void QxrdAcquisitionFileWatcher::endAcquisition()
{
}

void QxrdAcquisitionFileWatcher::shutdownAcquisition()
{
}

void QxrdAcquisitionFileWatcher::setupExposureMenu(QDoubleSpinBox* cb)
{
}

void QxrdAcquisitionFileWatcher::setupCameraGainMenu(QComboBox* cb)
{
}

void QxrdAcquisitionFileWatcher::setupCameraBinningModeMenu(QComboBox *cb)
{
}

QxrdAcquireDialogBase *QxrdAcquisitionFileWatcher::controlPanel(QxrdWindow *win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireFileWatcherDialog(m_Window, this, m_DataProcessor, m_Window);

    return m_ControlPanel;
  } else {
    return NULL;
  }
}
