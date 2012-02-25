#include "qxrdacquiredialogbase.h"

QxrdAcquireDialogBase::QxrdAcquireDialogBase(QxrdExperimentWPtr doc,
                                             QxrdWindow *win,
                                             QxrdAcquisitionWPtr acq,
                                             QxrdDataProcessorWPtr proc,
                                             QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(doc),
  m_Window(win),
  m_Acquisition(acq),
  m_DataProcessor(proc)
{
}
