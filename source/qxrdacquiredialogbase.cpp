#include "qxrdacquiredialogbase.h"

QxrdAcquireDialogBase::QxrdAcquireDialogBase(QxrdExperiment *doc,
                                             QxrdWindow *win,
                                             QxrdAcquisition *acq,
                                             QxrdDataProcessorPtr proc,
                                             QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(doc),
  m_Window(win),
  m_Acquisition(acq),
  m_DataProcessor(proc)
{
}
