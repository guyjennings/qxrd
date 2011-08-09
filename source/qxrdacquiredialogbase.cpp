#include "qxrdacquiredialogbase.h"

QxrdAcquireDialogBase::QxrdAcquireDialogBase(QxrdWindow *win, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent) :
  QDockWidget(parent),
  m_Window(win),
  m_Acquisition(acq),
  m_DataProcessor(proc)
{
}
