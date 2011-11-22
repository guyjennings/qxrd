#include "qxrdacquiredialogbase.h"

QxrdAcquireDialogBase::QxrdAcquireDialogBase(QxrdDocument *doc,
                                             QxrdWindow *win,
                                             QxrdAcquisition *acq,
                                             QxrdDataProcessor *proc,
                                             QWidget *parent) :
  QDockWidget(parent),
  m_Document(doc),
  m_Window(win),
  m_Acquisition(acq),
  m_DataProcessor(proc)
{
}
