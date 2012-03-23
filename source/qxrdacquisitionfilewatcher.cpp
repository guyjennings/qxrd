#include "qxrdacquisitionfilewatcher.h"
#include "qxrdacquirefilewatcherdialog.h"
#include "qxrdapplication.h"
#include "qxrdwindow.h"

QxrdAcquisitionFileWatcher::QxrdAcquisitionFileWatcher(QxrdSettingsSaverPtr saver,
                                                       QxrdExperimentWPtr doc,
                                                       QxrdDataProcessorWPtr proc,
                                                       QxrdAllocatorWPtr allocator) :
    QxrdAcquisition(QxrdAcquisition::FileWatcherDetector, saver, doc, proc, allocator)
{
}

void QxrdAcquisitionFileWatcher::onExposureTimeChanged(double /*newTime*/)
{
}

void QxrdAcquisitionFileWatcher::onBinningModeChanged(int /*newMode*/)
{
}

void QxrdAcquisitionFileWatcher::onCameraGainChanged(int /*newGain*/)
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

void QxrdAcquisitionFileWatcher::setupExposureMenu(QDoubleSpinBox* /*cb*/)
{
}

void QxrdAcquisitionFileWatcher::setupCameraGainMenu(QComboBox* /*cb*/)
{
}

void QxrdAcquisitionFileWatcher::setupCameraBinningModeMenu(QComboBox* /*cb*/)
{
}

QxrdAcquireDialogBase *QxrdAcquisitionFileWatcher::controlPanel(QxrdWindow *win)
{
  if (win) {
    m_Window = win;

    m_ControlPanel = new QxrdAcquireFileWatcherDialog(m_Experiment,
                                                      m_Window,
                                                      this,
                                                      m_DataProcessor,
                                                      m_Window);

    return m_ControlPanel;
  } else {
    return NULL;
  }
}
