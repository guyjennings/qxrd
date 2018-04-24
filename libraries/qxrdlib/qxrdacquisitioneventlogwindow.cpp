#include "qxrdacquisitioneventlogwindow.h"
#include "ui_qxrdacquisitioneventlogwindow.h"
#include "qxrdacqcommon.h"
#include "qxrdacquisitioneventlogmodel.h"

QxrdAcquisitionEventLogWindow::QxrdAcquisitionEventLogWindow(QxrdAcqCommonWPtr a) :
  QMainWindow(),
  m_Acquisition(a)
{
  setupUi(this);

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionEventLogPtr log(acq->acquisitionEventLog());

    m_AcquisitionEventLogModel =
        QxrdAcquisitionEventLogModelPtr(
          new QxrdAcquisitionEventLogModel(log));

    m_EventLog -> setModel(m_AcquisitionEventLogModel.data());

    CONNECT_CHECK(connect(m_ClearButton,       &QAbstractButton::clicked, this, &QxrdAcquisitionEventLogWindow::doClearEventLog));
    CONNECT_CHECK(connect(m_PauseResumeButton, &QAbstractButton::clicked, this, &QxrdAcquisitionEventLogWindow::doPauseResume));

    updateButtonTitles();

    m_EventLog -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);
  }
}

QxrdAcquisitionEventLogWindow::~QxrdAcquisitionEventLogWindow()
{
}

void QxrdAcquisitionEventLogWindow::doClearEventLog()
{
  m_AcquisitionEventLogModel -> clearEventLog();

  updateButtonTitles();
}

void QxrdAcquisitionEventLogWindow::doPauseResume()
{
  if (m_AcquisitionEventLogModel -> isPaused()) {
    m_AcquisitionEventLogModel -> resumeEventLog();
  } else {
    m_AcquisitionEventLogModel -> pauseEventLog();
  }

  updateButtonTitles();
}

void QxrdAcquisitionEventLogWindow::updateButtonTitles()
{
  if (m_AcquisitionEventLogModel -> isPaused()) {
    m_PauseResumeButton -> setText("Resume Event Log");
  } else {
    m_PauseResumeButton -> setText("Pause Event Log");
  }
}
